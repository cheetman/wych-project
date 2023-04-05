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
#include "base/VulkanTools.h"
#include "base/VulkanSwapChain.h"
#include "base/VulkanBuffer.h"
#include "base/VulkanDevice.h"
#include "base/VulkanTexture.h"


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#define TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#endif
#include "tiny_gltf.h"
#include "base/camera.hpp"



 // Contains everything required to render a basic glTF scene in Vulkan
 // This class is heavily simplified (compared to glTF's feature set) but retains the basic glTF structure
class VulkanglTFScene
{
public:
	// The class requires some Vulkan objects so it can create it's own resources
	vks::VulkanDevice* vulkanDevice;
	VkQueue copyQueue;

	// The vertex layout for the samples' model
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 color;
		glm::vec4 tangent;
	};

	// Single vertex buffer for all primitives
	struct {
		VkBuffer buffer;
		VkDeviceMemory memory;
	} vertices;

	// Single index buffer for all primitives
	struct {
		int count;
		VkBuffer buffer;
		VkDeviceMemory memory;
	} indices;

	// The following structures roughly represent the glTF scene structure
	// To keep things simple, they only contain those properties that are required for this sample
	struct Node;

	// A primitive contains the data for a single draw call
	struct Primitive {
		uint32_t firstIndex;
		uint32_t indexCount;
		int32_t materialIndex;
	};

	// Contains the node's (optional) geometry and can be made up of an arbitrary number of primitives
	struct Mesh {
		std::vector<Primitive> primitives;
	};

	// A node represents an object in the glTF scene graph
	struct Node {
		Node* parent;
		std::vector<Node*> children;
		Mesh mesh;
		glm::mat4 matrix;
		std::string name;
		bool visible = true;
		~Node() {
			for (auto& child : children) {
				delete child;
			}
		}
	};

	// A glTF material stores information in e.g. the texture that is attached to it and colors
	struct Material {
		glm::vec4 baseColorFactor = glm::vec4(1.0f);
		uint32_t baseColorTextureIndex;
		uint32_t normalTextureIndex;
		std::string alphaMode = "OPAQUE";
		float alphaCutOff;
		bool doubleSided = false;
		VkDescriptorSet descriptorSet;
		VkPipeline pipeline;
	};

	// Contains the texture for a single glTF image
	// Images may be reused by texture objects and are as such separated
	struct Image {
		vks::Texture2D texture;
	};

	// A glTF texture stores a reference to the image and a sampler
	// In this sample, we are only interested in the image
	struct Texture {
		int32_t imageIndex;
	};

	/*
		Model data
	*/
	std::vector<Image> images;
	std::vector<Texture> textures;
	std::vector<Material> materials;
	std::vector<Node*> nodes;

	std::string path;

	~VulkanglTFScene();
	VkDescriptorImageInfo getTextureDescriptor(const size_t index);
	void loadImages(tinygltf::Model& input);
	void loadTextures(tinygltf::Model& input);
	void loadMaterials(tinygltf::Model& input);
	void loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, VulkanglTFScene::Node* parent, std::vector<uint32_t>& indexBuffer, std::vector<VulkanglTFScene::Vertex>& vertexBuffer);
	void drawNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VulkanglTFScene::Node* node);
	void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
};


class VulkanExample
{

public:
	// 私有
	VulkanglTFScene glTFScene;

	struct ShaderData {
		vks::Buffer buffer;
		struct Values {
			glm::mat4 projection;
			glm::mat4 view;
			//glm::vec4 lightPos = glm::vec4(0.0f, 2.5f, 0.0f, 1.0f);
			glm::vec4 lightPos = glm::vec4(5.0f, 5.0f, -5.0f, 1.0f);

			glm::vec4 viewPos;
		} values;
	} shaderData;


	struct DescriptorSetLayouts {
		VkDescriptorSetLayout matrices;
		VkDescriptorSetLayout textures;
	} descriptorSetLayouts;


	VkPipelineLayout pipelineLayout;
	VkDescriptorSet descriptorSet;

		VulkanExample();

		void handleMouseMove(int32_t x, int32_t y);
		/** @brief (Virtual) Called after the mouse cursor moved and before internal events (like camera rotation) is handled */
		virtual void mouseMoved(double x, double y, bool& handled);


		VkClearColorValue defaultClearColor = { { 0.025f, 0.025f, 0.025f, 1.0f } };



	CommandLineParser commandLineParser;

	// OS specific
#if defined(_WIN32)
	HWND window;
	HINSTANCE windowInstance;

#endif

	bool viewUpdated = false;
	std::string title = "Vulkan-Project";
	std::string name = "vulkanProject";
	uint32_t apiVersion = VK_API_VERSION_1_0;

	uint32_t width = 1280;
	uint32_t height = 720;

	/** @brief Last frame time measured using a high performance timer (if available) */
	float frameTimer = 1.0f;

	bool prepared = false;
	bool resized = false;
	bool resizing = false;
	bool paused = false;
	uint32_t frameCounter = 0;

	uint32_t lastFPS = 0;

	// Defines a frame rate independent timer value clamped from -1.0...1.0
	// For use in animations, rotations, etc.
	float timer = 0.0f;
	// Multiplier for speeding up (or slowing down) the global timer
	float timerSpeed = 0.25f;

	struct Settings {
		/** @brief Activates validation layers (and message output) when set to true */
		bool validation = false;
		/** @brief Set to true if fullscreen mode has been requested via command line */
		bool fullscreen = false;
		/** @brief Set to true if v-sync will be forced for the swapchain */
		bool vsync = false;
		/** @brief Enable UI overlay */
		bool overlay = true;
	} settings;


	/** @brief Encapsulated physical and logical vulkan device */
	vks::VulkanDevice* vulkanDevice;


	struct {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} depthStencil;

	Camera camera;

	glm::vec2 mousePos;

	struct {
		bool left = false;
		bool right = false;
		bool middle = false;
	} mouseButtons;

	bool initVulkan();
	void renderLoop();

	void prepare();
	std::string getWindowTitle();

	VkResult createInstance(bool enableValidation);

#if defined(_WIN32)
	void setupConsole(std::string title);
	//void setupDPIAwareness();
	HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif


	/** @brief Adds the drawing commands for the ImGui overlay to the given command buffer */
	void drawUI(const VkCommandBuffer commandBuffer);

	void buildCommandBuffers();
	void loadglTFFile(std::string filename);
	void loadAssets();
	void setupDescriptors();
	void preparePipelines();
	void prepareUniformBuffers();
	void updateUniformBuffers();

	virtual void viewChanged();
	virtual void render();
	/** @brief (Virtual) Default image acquire + submission and command buffer submission function */
	virtual void renderFrame();
	virtual void prepareFrame();
	virtual void submitFrame();
	

private:


	void nextFrame();

	uint32_t destWidth;
	uint32_t destHeight;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp, tPrevEnd;

	void initSwapchain();
	void createCommandPool();
	void setupSwapChain();
	void createCommandBuffers();
	void createSynchronizationPrimitives();
	void setupDepthStencil();
	void setupRenderPass();
	void createPipelineCache();
	void setupFrameBuffer();


	void windowResize();
	void destroyCommandBuffers();

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



	// Command buffer pool
	VkCommandPool cmdPool;
	// Command buffers used for rendering
	std::vector<VkCommandBuffer> drawCmdBuffers;

	std::vector<VkFence> waitFences;

	// Global render pass for frame buffer writes
	VkRenderPass renderPass = VK_NULL_HANDLE;

	// Pipeline cache object
	VkPipelineCache pipelineCache;

	// List of available frame buffers (same as number of swap chain images)
	std::vector<VkFramebuffer>frameBuffers;



	// Descriptor set pool
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;



	std::string shaderDir = "glsl";


	// Returns the path to the root of the glsl or hlsl shader directory.
	std::string getShadersPath() const;


	// List of shader modules created (stored for cleanup)
	std::vector<VkShaderModule> shaderModules;

	/** @brief Loads a SPIR-V shader file for the given shader stage */
	VkPipelineShaderStageCreateInfo loadShader(std::string fileName, VkShaderStageFlagBits stage);


	// Active frame buffer index
	uint32_t currentBuffer = 0;

};



// TODO: 在此处引用程序需要的其他标头。
