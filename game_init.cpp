#ifdef _WIN32
#include <windows.h>
#endi
// Global GLFW window, Vulkan instance, and surface
GLFWwindow* window;
VkInstance instance;
VkSurfaceKHR surface;

// Xbox controller state
XINPUT_STATE controllerState;
bool isControllerConnected = false;

void initializeGame() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "Shooter Mini Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Initialize Vulkan (Your Vulkan setup here)

    // Example Vulkan initialization, please replace with actual Vulkan setup
    // vkCreateInstance and vkCreateSurface logic goes here.
}

void cleanup() {
    // Clean up Vulkan resources and GLFW
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Input handler for keyboard, mouse, and Xbox controller
void gameInput() {
    // Handle keyboard and mouse input via GLFW (already provided by GLFW)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        std::cout << "W key pressed!" << std::endl;  // Example: Player moves forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        std::cout << "S key pressed!" << std::endl;  // Example: Player moves backward
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        std::cout << "Space key pressed!" << std::endl;  // Example: Jump or shoot
    }

    // Update Xbox controller input
    updateControllerInput();
}

// Update Xbox controller input using XInput
void updateControllerInput() {
    ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(0, &controllerState);  // Get the state of the first controller

    if (dwResult == ERROR_SUCCESS) {
        isControllerConnected = true;

        // Example: Move the player with the left thumbstick
        float leftThumbX = controllerState.Gamepad.sThumbLX / 32767.0f;  // Normalize thumbstick X-axis
        float leftThumbY = controllerState.Gamepad.sThumbLY / 32767.0f;  // Normalize thumbstick Y-axis

        // Example: Pressing A button to jump
        if (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
            std::cout << "A button pressed!" << std::endl;
        }

        // Example: Use the right thumbstick for aiming (or shooting)
        float rightThumbX = controllerState.Gamepad.sThumbRX / 32767.0f; // Right thumbstick X-axis
        float rightThumbY = controllerState.Gamepad.sThumbRY / 32767.0f; // Right thumbstick Y-axis

        // Example: Detect trigger presses (for shooting or actions)
        if (controllerState.Gamepad.bLeftTrigger > 30) {
            std::cout << "Left trigger pressed!" << std::endl;  // Action, like shoot
        }
        if (controllerState.Gamepad.bRightTrigger > 30) {
            std::cout << "Right trigger pressed!" << std::endl;  // Action, like shoot
        }

        // Handle movement based on thumbstick input
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