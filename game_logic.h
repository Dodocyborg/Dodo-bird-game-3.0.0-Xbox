#include <iostream>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Xinput.h>  // Xbox controller input
#include <vector>

// Link DirectX and XInput libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "Xinput.lib")  // Link XInput library

// Global DirectX variables
IDXGISwapChain* swapChain = nullptr;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* deviceContext = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;
ID3D11InputLayout* inputLayout = nullptr;

// Xbox Controller state
XINPUT_STATE controllerState;
bool isControllerConnected = false;

// Game specific variables (these could come from your game_init.h, game_logic.h, etc.)
float deltaTime = 0.016f;  // 60 FPS
bool isRunning = true;

// Forward declarations for functions
void initializeDirectX(HWND hwnd);
void cleanupDirectX();
void renderFrame();
void gameLogic(float deltaTime);
void gameInput();
void updateControllerInput();

// Main window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        isRunning = false;
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// DirectX Initialization
void initializeDirectX(HWND hwnd) {
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
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scDesc, &swapChain, &device, &featureLevel, &deviceContext);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize DirectX" << std::endl;
        exit(-1);
    }

    // Create Render Target View
    ID3D11Texture2D* pBackBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    pBackBuffer->Release();

    // Set Render Target
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
}

// Cleanup DirectX resources
void cleanupDirectX() {
    if (renderTargetView) renderTargetView->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
    if (swapChain) swapChain->Release();
}

// Render the frame
void renderFrame() {
    // Clear the back buffer
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    // Rendering game objects goes here (currently just clearing screen)

    // Present the frame
    swapChain->Present(1, 0);  // Present with vsync
}

// Handle game logic
void gameLogic(float deltaTime) {
    // Game update logic (e.g., player movement, AI)
    // Example: Move player or update game objects based on deltaTime
    // Replace with actual game logic
}

// Handle input
void gameInput() {
    updateControllerInput();  // Check Xbox controller input
}

// Update Xbox controller input
void updateControllerInput() {
    // Check controller state
    ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(0, &controllerState);  // Get state of the first controller

    if (dwResult == ERROR_SUCCESS) {
        isControllerConnected = true;

        // Get the controller's thumbsticks and buttons
        float leftThumbX = controllerState.Gamepad.sThumbLX / 32767.0f;  // Normalize thumbstick X-axis
        float leftThumbY = controllerState.Gamepad.sThumbLY / 32767.0f;  // Normalize thumbstick Y-axis
        float rightThumbX = controllerState.Gamepad.sThumbRX / 32767.0f; // Normalize right thumbstick X-axis
        float rightThumbY = controllerState.Gamepad.sThumbRY / 32767.0f; // Normalize right thumbstick Y-axis

        // Example: Detect button presses
        if (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
            std::cout << "A button pressed!" << std::endl;
        }
        if (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
            std::cout << "B button pressed!" << std::endl;
        }
        if (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
            std::cout << "X button pressed!" << std::endl;
        }
        if (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
            std::cout << "Y button pressed!" << std::endl;
        }

        // Handle thumbstick movement (e.g., move player)
        if (leftThumbX > 0.5f) {
            std::cout << "Move right!" << std::endl;
        }
        if (leftThumbX < -0.5f) {
            std::cout << "Move left!" << std::endl;
        }
        if (leftThumbY > 0.5f) {
            std::cout << "Move up!" << std::endl;
        }
        if (leftThumbY < -0.5f) {
            std::cout << "Move down!" << std::endl;
        }

    } else {
        isControllerConnected = false;
        std::cout << "Controller not connected" << std::endl;
    }
}

int main() {
    // Setup Win32 Window
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, _T("DirectXGameWindow"), nullptr };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, _T("DirectX Game with Xbox Controller"), WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize DirectX (passing hwnd from the window)
    initializeDirectX(hwnd);

    // Initialize game resources (custom functions from your game_init.h)
    // initializeGame(); // Call your custom game initialization code

    // Game Loop
    MSG msg = {};
    while (isRunning) {
        // Handle window messages
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Handle Input (gameInput for Xbox controller input)
        gameInput();

        // Update Game Logic
        gameLogic(deltaTime);

        // Render the Frame
        renderFrame();
    }

    // Cleanup DirectX resources
    cleanupDirectX();

    return 0;
}