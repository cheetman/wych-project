// c-project-cmake.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#if defined(_WIN32)
#define KEY_ESCAPE VK_ESCAPE 
#define KEY_F1 VK_F1
#define KEY_F2 VK_F2
#define KEY_F3 VK_F3
#define KEY_F4 VK_F4
#define KEY_F5 VK_F5
#define KEY_W 0x57
#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_D 0x44
#define KEY_P 0x50
#define KEY_SPACE 0x20
#define KEY_KPADD 0x6B
#define KEY_KPSUB 0x6D
#define KEY_B 0x42
#define KEY_F 0x46
#define KEY_L 0x4C
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_T 0x54
#endif



#include <iostream>

#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingAPI.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include <ctime>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <sys/stat.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <numeric>
#include <array>

#include "vulkan/vulkan.h"
#include <vulkan/vulkan_win32.h>

#include "base/CommandLineParser.hpp"
#include "base/VulkanTools.h"
#include "base/VulkanSwapChain.h"
#include "base/VulkanBuffer.h"
#include "base/VulkanDevice.h"


class VulkanExample
{

public:
	VulkanExample();


	CommandLineParser commandLineParser;

	// OS specific
#if defined(_WIN32)
	HWND window;
	HINSTANCE windowInstance;

#endif

	std::string title = "Vulkan-Project";
	std::string name = "vulkanProject";
	uint32_t apiVersion = VK_API_VERSION_1_0;

	uint32_t width = 1280;
	uint32_t height = 720;


	bool prepared = false;
	bool resizing = false;
	bool paused = false;
	uint32_t frameCounter = 0;


	struct Settings {
		/** @brief Activates validation layers (and message output) when set to true */
		bool validation = true;
		/** @brief Set to true if fullscreen mode has been requested via command line */
		bool fullscreen = false;
		/** @brief Set to true if v-sync will be forced for the swapchain */
		bool vsync = false;
		/** @brief Enable UI overlay */
		bool overlay = true;
	} settings;


	/** @brief Encapsulated physical and logical vulkan device */
	vks::VulkanDevice* vulkanDevice;




	bool initVulkan();
	void renderLoop();
	std::string getWindowTitle();

	VkResult createInstance(bool enableValidation);

#if defined(_WIN32)
	void setupConsole(std::string title);
	//void setupDPIAwareness();
	HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif


protected:

	// Vulkan instance, stores all per-application states
	VkInstance instance;
	// Stores physical device properties (for e.g. checking device limits)
	VkPhysicalDeviceProperties deviceProperties;
	// Stores the features available on the selected physical device (for e.g. checking if a feature is available)
	VkPhysicalDeviceFeatures deviceFeatures;
	// Stores all available memory (type) properties for the physical device
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

	std::vector<std::string> supportedInstanceExtensions;


	/** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
	VkPhysicalDeviceFeatures enabledFeatures{};
	/** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
	std::vector<const char*> enabledDeviceExtensions;
	std::vector<const char*> enabledInstanceExtensions;
	/** @brief Optional pNext structure for passing extension structures to device creation */
	void* deviceCreatepNextChain = nullptr;

	/** @brief Logical device, application's view of the physical device (GPU) */
	VkDevice device;

	// Physical device (GPU) that Vulkan will use
	VkPhysicalDevice physicalDevice;


	// Handle to the device graphics queue that command buffers are submitted to
	VkQueue queue;

	/** @brief Pipeline stages used to wait at for graphics queue submissions */
	VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	// Depth buffer format (selected during Vulkan initialization)
	VkFormat depthFormat;

	// Contains command buffers and semaphores to be presented to the queue
	VkSubmitInfo submitInfo;

	// Wraps the swap chain to present images (framebuffers) to the windowing system
	VulkanSwapChain swapChain;
	// Synchronization semaphores
	struct {
		// Swap chain image presentation
		VkSemaphore presentComplete;
		// Command buffer submission and execution
		VkSemaphore renderComplete;
	} semaphores;
};



// TODO: 在此处引用程序需要的其他标头。
