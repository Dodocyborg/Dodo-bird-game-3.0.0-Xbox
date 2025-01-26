#include <iostream>
#include "DirectXInuit.h"Windows.h"
#include <d3d11.h>
#include "game_init.h"
#include "game_logic.h"
#include "render.h"
#include "ProceduralTerrain.h"
#include "Collision.h"
#include "AI.h"
#include "Matchmaking.h"
#include "Leaderboards.h"
#include "Networking.h"
#include "utils.h 
    #include #utils.cpp
    
// DirectX Headers
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Global variables for DirectX
IDXGISwapChain* swapChain = nullptr;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* deviceContext = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;

void initializeDirectX(HWND hwnd) {
    // DirectX initialization code here (setup device, swap chain, etc.)
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.Width = 800;
    scDesc.BufferDesc.Height = 600;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = hwnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scDesc, &swapChain, &device, &featureLevel, &deviceContext);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize DirectX" << std::endl;
        exit(-1);
    }

    // Create Render Target View
    ID3D11Texture2D* pBackBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    pBackBuffer->Release();
}

void cleanupDirectX() {
    if (renderTargetView) renderTargetView->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
    if (swapChain) swapChain->Release();
}

void renderFrame() {
    // Clear the back buffer
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    // Rendering code: Draw game objects here

    // Present the frame
    swapChain->Present(1, 0);  // Present with vsync
}

int main() {
    // Initialize game and resources
    initializeGame();

    // Initialize DirectX (assuming you have a valid HWND for your window)
    HWND hwnd = GetConsoleWindow();  // Replace with actual HWND from your windowing system
    initializeDirectX(hwnd);

    // Initialize Procedural Terrain
    ProceduralTerrain terrain;
    terrain.generateTerrain();

    // Initialize Networking
    Networking network;
    network.initialize();

    // Initialize Matchmaking
    Matchmaking matchmaking;
    matchmaking.initialize();

    // Initialize Leaderboards
    Leaderboards leaderboards;

    // Main Game Loop
    bool isRunning = true;
    while (isRunning) {
        float deltaTime = 0.016f;  // Assuming 60 FPS for simplicity

        // Handle Input
        glfwPollEvents();  // Replace with DirectX input handling (e.g., Win32 messages or raw input)

        // Update Game Logic (Player, Enemies, Bullets)
        gameLogic(deltaTime);

        // Render the Game
        renderFrame();

        // Handle AI Logic
        AI::updateAllAI();

        // Handle Networking
        network.syncState();

        // Example Quit Condition
        if (false) {  // Replace with actual exit condition
            isRunning = false;
        }
    }

    // Clean Up DirectX and resources
    cleanupDirectX();
    network.cleanup();
    return 0;
}