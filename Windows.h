#include <Windows.h>  // Ensure this is first to avoid conflicts with other headers
#include <iostream>
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
}