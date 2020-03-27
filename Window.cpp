#include "Window.h"
void Runtime::run() {
    Runtime::initVulkan();
    Runtime::createInstance();
    Runtime::pickPhysicalDevice();
    Runtime::mainLoop();
    Runtime::cleanup();
}
void Runtime::initVulkan() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
}
void Runtime::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Triangle test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "None";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }
}
bool Runtime::isDeviceSuitable(VkPhysicalDevice device) {
    return true;
}
void Runtime::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find a Vulkan capiable GPU!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            GPU = device;
            break;
        }
    }
    if (GPU == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find suitable GPU!");
    }
    VkPhysicalDeviceProperties GPUprop = {};
    VkPhysicalDeviceMemoryProperties GPUmemory = {};
    VkPhysicalDeviceFeatures GPUfeatures = {};
    vkGetPhysicalDeviceFeatures(GPU, &GPUfeatures);
    vkGetPhysicalDeviceProperties(GPU, &GPUprop);
    vkGetPhysicalDeviceMemoryProperties(GPU, &GPUmemory);
    std::cout << "Found: " << GPUprop.deviceName << std::endl;
}
void Runtime::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}
void Runtime::cleanup() {
    vkDestroyInstance(this->instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}