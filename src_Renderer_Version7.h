#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <string>
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();
    bool initialize(void* hwnd, int width, int height);
    void beginFrame();
    void endFrame();
    void shutdown();
    void drawProceduralChunk(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const float* modelMatrix, int textureID, float biomeLighting, float fogDensity);
    void setCamera(const float* viewMatrix, const float* projectionMatrix);
    bool shouldClose() const;
    // ...other high-end rendering utilities

private:
    void createDevice();
    void createSwapchain(void* hwnd, int width, int height);
    void createRenderTargets();
    void createDepthStencil();
    void createCommandObjects();
    void loadShadersAndPipelines();

    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> swapchain;
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[2];
    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvSrvHeap;
    UINT rtvDescriptorSize;
    UINT frameIndex;
    HANDLE fenceEvent;
    UINT64 fenceValue;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    // Camera, shader, constant buffer, texture, and lighting pipeline state...
};