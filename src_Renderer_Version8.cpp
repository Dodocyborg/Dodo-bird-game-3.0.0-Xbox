#include "Renderer.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <assert.h>

// Helper macro for checking HRESULTs
#ifndef HR
#define HR(x)                                                      \
    { HRESULT hr__ = (x); if(FAILED(hr__)) {                        \
        std::cerr << "HRESULT failed at " << __FILE__ << ":" << __LINE__ << " code 0x" << std::hex << hr__ << std::dec << std::endl; \
        throw std::runtime_error("HRESULT failed."); } }
#endif

using Microsoft::WRL::ComPtr;
using namespace DirectX;

// --- Internal Structs --- //
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};

struct alignas(256) CameraCB {
    XMMATRIX view;
    XMMATRIX proj;
    XMFLOAT4 camPos;
};

struct alignas(256) ModelCB {
    XMMATRIX model;
    XMFLOAT4 biomeColor;
    float biomeLighting;
    float fogDensity;
    float pad[2];
};

static const UINT FrameCount = 2;

Renderer::Renderer()
    : rtvDescriptorSize(0), frameIndex(0), fenceValue(0), fenceEvent(nullptr),
      width(1920), height(1080), hwnd(nullptr) {}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize(void* hwnd_, int width_, int height_) {
    hwnd = hwnd_;
    width = width_;
    height = height_;
    createDevice();
    createCommandObjects();
    createSwapchain(hwnd, width, height);
    createRenderTargets();
    createDepthStencil();
    createDescriptorHeaps();
    loadShadersAndPipelines();
    createConstantBuffers();
    // Set up camera, PBR textures, shadow maps, sampler states, etc.
    return true;
}

void Renderer::createDevice() {
    UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
    // Enable debug layer for development builds
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            debugController->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif
    ComPtr<IDXGIFactory4> factory;
    HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    // Create hardware device
    HR(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));
}

void Renderer::createCommandObjects() {
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    HR(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));

    for (UINT i = 0; i < FrameCount; ++i) {
        HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator[i])));
    }
    HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[0].Get(), nullptr, IID_PPV_ARGS(&commandList)));
    commandList->Close();

    HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
    fenceValue = 1;
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void Renderer::createSwapchain(void* hwnd_, int width_, int height_) {
    ComPtr<IDXGIFactory4> factory;
    HR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = width_;
    swapChainDesc.Height = height_;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain1;
    HR(factory->CreateSwapChainForHwnd(
        commandQueue.Get(),
        (HWND)hwnd_,
        &swapChainDesc,
        nullptr, nullptr,
        &swapChain1
    ));
    HR(factory->MakeWindowAssociation((HWND)hwnd_, DXGI_MWA_NO_ALT_ENTER));
    HR(swapChain1.As(&swapchain));
    frameIndex = swapchain->GetCurrentBackBufferIndex();
}

void Renderer::createRenderTargets() {
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HR(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)));
    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < FrameCount; i++) {
        HR(swapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])));
        device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, rtvDescriptorSize);
    }
}

void Renderer::createDepthStencil() {
    D3D12_RESOURCE_DESC depthDesc = {};
    depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthDesc.Alignment = 0;
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.DepthOrArraySize = 1;
    depthDesc.MipLevels = 1;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = DXGI_FORMAT_D32_FLOAT;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    HR(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(&depthStencilBuffer)
    ));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HR(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap)));

    device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void Renderer::createDescriptorHeaps() {
    // CBV/SRV/UAV heap
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
    cbvSrvHeapDesc.NumDescriptors = 64;
    cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    HR(device->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&cbvSrvHeap)));
}

void Renderer::loadShadersAndPipelines() {
    // Load/compile HLSL shaders (vertex, pixel) for PBR, terrain, sky, etc.
    // Create root signatures and pipeline state objects (PSO) for each type.
    // For brevity, shader code is not included, but should feature:
    // - Physically-based rendering
    // - Shadow mapping
    // - Volumetric fog
    // - Tiled lighting
    // - Biome coloring
    // - Atmospheric scattering for sky
    // - Instanced chunk rendering
    // - Post-processing (HDR, bloom, tone-mapping)
}

void Renderer::createConstantBuffers() {
    // Allocate upload heap buffers for CameraCB and ModelCB (per-frame and per-object)
}

void Renderer::beginFrame() {
    // Reset command allocator and command list
    HR(commandAllocator[frameIndex]->Reset());
    HR(commandList->Reset(commandAllocator[frameIndex].Get(), nullptr));

    // Resource barrier: Present -> RenderTarget
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = renderTargets[frameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);

    // Set render target and clear
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHeap->GetCPUDescriptorHandleForHeapStart());
    const float clearColor[] = {0.08f, 0.13f, 0.22f, 1.0f};
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void Renderer::endFrame() {
    // Resource barrier: RenderTarget -> Present
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = renderTargets[frameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);

    HR(commandList->Close());

    // Execute command list
    ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
    commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present
    HR(swapchain->Present(1, 0));

    // Signal and increment the fence value
    const UINT64 currFence = fenceValue;
    HR(commandQueue->Signal(fence.Get(), currFence));
    fenceValue++;

    // Wait until frame is ready
    if (fence->GetCompletedValue() < currFence) {
        HR(fence->SetEventOnCompletion(currFence, fenceEvent));
        WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
    }

    frameIndex = swapchain->GetCurrentBackBufferIndex();
}

void Renderer::drawProceduralChunk(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const float* modelMatrix, int textureID, float biomeLighting, float fogDensity) {
    // Upload vertex/index data to GPU (can use upload heap or ring buffer for streaming)
    // Set pipeline state for terrain/biome rendering
    // Set root signature, descriptor heaps, constant buffers (Camera, Model)
    // Bind PBR textures, set biome/fog uniforms
    // Issue draw call (DrawIndexedInstanced or DrawIndexed)
}

void Renderer::setCamera(const float* viewMatrix, const float* projectionMatrix) {
    // Update CameraCB buffer
}

bool Renderer::shouldClose() const {
    // You can implement this by handling WM_CLOSE or ESC input in your application
    return false;
}

void Renderer::shutdown() {
    if (device) { device->Release(); device = nullptr; }
    if (swapchain) { swapchain->Release(); swapchain = nullptr; }
    if (fenceEvent) { CloseHandle(fenceEvent); fenceEvent = nullptr; }
    // Release all D3D12 resources, heaps, buffers, PSOs, etc.
}