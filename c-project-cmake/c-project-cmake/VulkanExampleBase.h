
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
#include <iomanip>
#include <chrono>
#include <random>
#include <algorithm>
#include <sys/stat.h>


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
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
#include "base/VulkanDebug.h"
#include "base/VulkanTools.h"
#include "base/VulkanUIOverlay.h"
#include "base/VulkanSwapChain.h"
#include "base/VulkanBuffer.h"
#include "base/VulkanDevice.h"
#include "base/VulkanTexture.h"



#include "base/camera.hpp"




class VulkanExampleBase
{

private :

	uint32_t destWidth;
	uint32_t destHeight;
	std::string getWindowTitle();
	void handleMouseMove(int32_t x, int32_t y);
	void windowResize();
	void destroyCommandBuffers();
	void nextFrame();
	void updateOverlay();


public:

	VulkanExampleBase();

	std::string title = "Vulkan Project";
	std::string name = "vulkanProject";
	uint32_t apiVersion = VK_API_VERSION_1_0;
	uint32_t width = 1280;
	uint32_t height = 720;
	bool prepared = false;
	bool resized = false;
	bool resizing = false;
	bool paused = false;
	bool viewUpdated = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp, tPrevEnd;
	/** @brief Last frame time measured using a high performance timer (if available) */
	float frameTimer = 1.0f;
	uint32_t lastFPS = 0;
	// Defines a frame rate independent timer value clamped from -1.0...1.0
	// For use in animations, rotations, etc.
	float timer = 0.0f;
	// Multiplier for speeding up (or slowing down) the global timer
	float timerSpeed = 0.25f;


	// 摄像机
	Camera camera;
	// imgui
	vks::UIOverlay UIOverlay;
	// 命令解析器
	CommandLineParser commandLineParser;
	// 物理设备逻辑设备信息
	vks::VulkanDevice* vulkanDevice;
	// 鼠标坐标
	glm::vec2 mousePos;
	// 鼠标状态
	struct {
		bool left = false;
		bool right = false;
		bool middle = false;
	} mouseButtons;



	struct Settings {
		// validation layers
		bool validation = false;
		/** @brief Set to true if fullscreen mode has been requested via command line */
		bool fullscreen = false;
		/** @brief Set to true if v-sync will be forced for the swapchain */
		bool vsync = false;
		/** @brief Enable UI overlay */
		bool overlay = true;
	} settings;



#if defined(_WIN32)
	HWND window;
	HINSTANCE windowInstance;

	void setupConsole(std::string title);
	void setupDPIAwareness();
	HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif



	/** @brief (Virtual) Creates the application wide Vulkan instance */
	virtual VkResult createInstance(bool enableValidation);

	// 初始化  instance, enable required extensions and connect to the physical device (GPU) 
	bool initVulkan();
	// 初始化 Swapchain
	void initSwapchain();
	// 初始化 CommandPool
	void createCommandPool();

	void setupSwapChain();

	void createCommandBuffers();
	void createSynchronizationPrimitives();
	void setupDepthStencil();
	void setupRenderPass();
	void createPipelineCache();
	void setupFrameBuffer();

	// 主循环
	void renderLoop();

	void prepareFrame();
	void submitFrame();

	// 用来设置 device features
	virtual void getEnabledFeatures();
	// 用来设置 device Extensions
	virtual void getEnabledExtensions();


	// 虚拟 build命令
	virtual void buildCommandBuffers();
	// 虚拟 渲染函数
	virtual void render() = 0;
	// 虚拟 更新UI
	virtual void OnUpdateUIOverlay(vks::UIOverlay* overlay);

	// 事件 - 鼠标移动
	virtual void mouseMoved(double x, double y, bool& handled);
	// 事件 - 窗口Resize
	virtual void windowResized();
	// 事件 - 相机改变
	virtual void viewChanged();

protected:

	// Vulkan Instance, stores all per-application states
	VkInstance instance;
	// Vulkan 物理设备
	VkPhysicalDevice physicalDevice;
	// Vulkan 逻辑设备
	VkDevice device;


	// 支持的Instance扩展
	std::vector<std::string> supportedInstanceExtensions;
	// 启用的Instance扩展
	std::vector<const char*> enabledInstanceExtensions;

	// Stores physical device properties (for e.g. checking device limits)
	VkPhysicalDeviceProperties deviceProperties;
	// Stores the features available on the selected physical device (for e.g. checking if a feature is available)
	VkPhysicalDeviceFeatures deviceFeatures;
	// Stores all available memory (type) properties for the physical device
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

	VkPhysicalDeviceDriverProperties driverProperties = {};
	/** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
	VkPhysicalDeviceFeatures enabledFeatures{};
	/** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
	std::vector<const char*> enabledDeviceExtensions;
	/** @brief Optional pNext structure for passing extension structures to device creation */
	void* deviceCreatepNextChain = nullptr;


	// Handle to the device graphics queue that command buffers are submitted to
	VkQueue queue;
	// Depth buffer format (selected during Vulkan initialization)
	VkFormat depthFormat;

	// 深度图
	struct {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} depthStencil;

	// Contains command buffers and semaphores to be presented to the queue
	VkSubmitInfo submitInfo;
	/** @brief Pipeline stages used to wait at for graphics queue submissions */
	VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// Vulkan SwapChain
	VulkanSwapChain swapChain;

	// Synchronization semaphores
	struct {
		// Swap chain image presentation
		VkSemaphore presentComplete;
		// Command buffer submission and execution
		VkSemaphore renderComplete;
	} semaphores;

	// Command buffer pool
	VkCommandPool cmdPool;

	// Command buffers used for rendering
	std::vector<VkCommandBuffer> drawCmdBuffers;
	// Fence 用于同步
	std::vector<VkFence> waitFences;
	// Global render pass for frame buffer writes
	VkRenderPass renderPass = VK_NULL_HANDLE;
	// Pipeline cache object
	VkPipelineCache pipelineCache;
	// List of available frame buffers (same as number of swap chain images)
	std::vector<VkFramebuffer>frameBuffers;
	// Active frame buffer index
	uint32_t currentBuffer = 0;

	// Frame counter to display fps
	uint32_t frameCounter = 0;

};