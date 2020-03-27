#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <optional>
class Runtime {
public:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};
	Runtime(int width, int height);
	~Runtime();
	void initVulkan(int width, int height);
	void createInstance();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void pickPhysicalDevice();
	void createLogicalDevice();
	void mainLoop();
	void cleanup();
private:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice GPU = VK_NULL_HANDLE;  // Start with no GPU
	VkDevice device;
};