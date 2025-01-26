#ifdef _WIN32
#include <windows.h>
#include <d3d11.h>
#include <iostream>

// DirectX initialization for Windows
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
std::cerr << "Please use an alternative graphics API such as OpenGL or SDL." << std::endl;

// Optionally, you could add OpenGL or SDL initialization here if needed
#endif