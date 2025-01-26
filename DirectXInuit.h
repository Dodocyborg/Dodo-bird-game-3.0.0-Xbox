#ifdef _WIN32
#include <windows.h>// DirectXInuit.h
#ifndef DIRECTXINUIT_H"/link",
"d3d11.lib",
"dxgi.lib",
"d3dcompiler.lib",
"kernel32.lib",
"user32.lib",
"gdi32.lib",
"winmm.lib",
"advapi32.lib",
"shell32.lib",
"ole32.lib",
"oleaut32.lib",
"uuid.lib"
#define DIRECTXINUIT_H

#include 
#include 
#include 
#include <Windows.h>
#include <iostream> // Include if you use std::cerr in this header

// Declare DirectX variables as extern so they can be accessed from other files
extern IDXGISwapChain* swapChain;
extern ID3D11DeviceContext* deviceContext;
extern ID3D11Device* device;
extern ID3D11RenderTargetView* renderTargetView;

// Function prototypes
HRESULT InitializeDirectX(HWND hWnd);
void CleanupDirectX();

#endif
#include <d3d11.h>
#include <iostream>#ifndef DIRECTXINUIT_H
#define DIRECTXINUIT_H

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h> // Sometimes needed
#include <windows.h>  // Essential for DirectX and HWND
#include <iostream>  // For std::cerr (if used in the header)

// ... your DirectX variable declarations and function prototypes ...

#endif/ DirectX initialization for Windows
D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
HRESULT hr = D3D11CreateDeviceAndSwapChain(
    nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, ARRAYSIZE(featureLevels),
    D3D11_SDK_VERSION, &scDesc, &swapChain, &device, &featureLevel, &deviceContext
);

if (FAILED(hr)) {
    std::cerr << "Failed to initialize DirectX!" << std::endl;
    exit(1); // Handle the error
}

#else
// Placeholder or alternative initialization for non-Windows platforms
std::cerr << "DirectX is not supported on this platform." << std::endl;
std::cerr << "Ple
// Optionally, you could add OpenGL or SDL initialization here if needed
#endif#ifndef DIRECTXINUIT_H
#define DIRECTXINUIT_H

#include <d3d11.h>/
#include <dxgi.h>/
#include <d3dcommon.h>/ Sometimes needed
#include <windows.h>/ Essential for DirectX and HWND
#include <iostream>// For std::cerr (if used in the header)

// ... your DirectX variable declarations and function prototypes ...

#endif