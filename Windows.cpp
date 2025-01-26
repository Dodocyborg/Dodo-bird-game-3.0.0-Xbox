#ifdef _WIN32
#include "Windows.h"  // Windows-specific header
#endif

// Constructor that sets up the window's title, width, and height.
GameWindow::GameWindow(const std::string& windowTitle, int w, int h)
    : title(windowTitle), width(w), height(h), hwnd(NULL), hInstance(GetModuleHandle(NULL)) {}

// Destructor
GameWindow::~GameWindow() {
    closeWindow();
}

// Create the window using WinAPI
void GameWindow::createWindow() {
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DefWindowProc;  // Default window procedure
    wc.hInstance = hInstance;
    wc.lpszClassName = "GameWindowClass";

    // Register the window class
    RegisterClassEx(&wc);

    // Create the window
    hwnd = CreateWindowEx(
        0, "GameWindowClass", title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, NULL, NULL, hInstance, NULL
    );

    // Display the window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

// Run the message loop for the window
void GameWindow::messageLoop() {
    while (GetMessage(&msg, hwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Handle user input (this function can be expanded to handle specific input like keyboard/mouse)
void GameWindow::handleInput() {
    // Example of basic message handling (input events, window events, etc.)
    if (msg.message == WM_KEYDOWN) {
        // Handle key down event
    }
    // Other input handling logic can be added here
}

// Close the window gracefully
void GameWindow::closeWindow() {
    if (hwnd != NULL) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);  // Send close message to the window
    }
}

// Get the window handle
HWND GameWindow::getWindowHandle() const {
    return hwnd;
}