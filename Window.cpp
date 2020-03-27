#include "Window.h"
Runtime::Runtime(int width, int height) {
    Runtime::initVulkan(width, height);
    Runtime::createInstance();
    Runtime::pickPhysicalDevice();
    Runtime::createLogicalDevice();
    Runtime::mainLoop();
}
Runtime::~Runtime() {
    Runtime::cleanup();
}
void Runtime::initVulkan(int width, int height) {
    std::cout << "Initializing Vulkan" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Vulkan window", nullptr, nullptr);
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
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}


Runtime::QueueFamilyIndices Runtime::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }

        i++;
    }
    return indices;
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

void Runtime::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(GPU);

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(GPU, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
}

void Runtime::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}
void Runtime::cleanup() {
    std::cout << "Cleaning up" << std::endl;
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(this->instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}