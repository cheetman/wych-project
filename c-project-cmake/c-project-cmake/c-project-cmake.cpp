// c-project-cmake.cpp: 定义应用程序的入口点。
//

#include "c-project-cmake.h"
#include "base/VulkanDebug.h"


using namespace std;



VulkanExample::VulkanExample() {
#if defined(_WIN32)
	// Enable console if validation is active, debug message callback will output to it
	if (this->settings.validation)
	{
		setupConsole("Vulkan Project");
	}
	setupDPIAwareness();
#endif

	title = "glTF scene rendering";
	camera.type = Camera::CameraType::firstperson;
	camera.flipY = true;
	camera.setPosition(glm::vec3(-0.9f, 1.0f, 0.0f));
	camera.setRotation(glm::vec3(0.0f, -24.0f, 0.0f));
	camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);
}


void VulkanExample::mouseMoved(double x, double y, bool& handled) {}

void VulkanExample::handleMouseMove(int32_t x, int32_t y)
{
	int32_t dx = (int32_t)mousePos.x - x;
	int32_t dy = (int32_t)mousePos.y - y;

	bool handled = false;

	//if (settings.overlay) {
	//	ImGuiIO& io = ImGui::GetIO();
	//	handled = io.WantCaptureMouse && UIOverlay.visible;
	//}
	mouseMoved((float)x, (float)y, handled);

	if (handled) {
		mousePos = glm::vec2((float)x, (float)y);
		return;
	}

	if (mouseButtons.left) {
		camera.rotate(glm::vec3(dy * camera.rotationSpeed, -dx * camera.rotationSpeed, 0.0f));
		viewUpdated = true;
	}
	if (mouseButtons.right) {
		camera.translate(glm::vec3(-0.0f, 0.0f, dy * .005f));
		viewUpdated = true;
	}
	if (mouseButtons.middle) {
		camera.translate(glm::vec3(-dx * 0.005f, -dy * 0.005f, 0.0f));
		viewUpdated = true;
	}
	mousePos = glm::vec2((float)x, (float)y);
}


void VulkanExample::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		prepared = false;
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(window, NULL);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case KEY_P:
			paused = !paused;
			break;
			/*case KEY_F1:
				UIOverlay.visible = !UIOverlay.visible;
				UIOverlay.updated = true;
				break;*/
		case KEY_ESCAPE:
			PostQuitMessage(0);
			break;
		}

			if (camera.type == Camera::firstperson)
			{
				switch (wParam)
				{
				case KEY_W:
					camera.keys.up = true;
					break;
				case KEY_S:
					camera.keys.down = true;
					break;
				case KEY_A:
					camera.keys.left = true;
					break;
				case KEY_D:
					camera.keys.right = true;
					break;
				}
			}

			//keyPressed((uint32_t)wParam);
		break;
	case WM_KEYUP:
		if (camera.type == Camera::firstperson)
		{
			switch (wParam)
			{
			case KEY_W:
				camera.keys.up = false;
				break;
			case KEY_S:
				camera.keys.down = false;
				break;
			case KEY_A:
				camera.keys.left = false;
				break;
			case KEY_D:
				camera.keys.right = false;
				break;
			}
		}
		break;
		case WM_LBUTTONDOWN:
			mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			mouseButtons.left = true;
			break;
		case WM_RBUTTONDOWN:
			mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			mouseButtons.right = true;
			break;
		case WM_MBUTTONDOWN:
			mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			mouseButtons.middle = true;
			break;
		case WM_LBUTTONUP:
			mouseButtons.left = false;
			break;
		case WM_RBUTTONUP:
			mouseButtons.right = false;
			break;
		case WM_MBUTTONUP:
			mouseButtons.middle = false;
			break;
			case WM_MOUSEWHEEL:
			{
				short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				camera.translate(glm::vec3(0.0f, 0.0f, (float)wheelDelta * 0.005f));
				viewUpdated = true;
				break;
			}
			case WM_MOUSEMOVE:
			{
				handleMouseMove(LOWORD(lParam), HIWORD(lParam));
				break;
			}
	case WM_SIZE:
		if ((prepared) && (wParam != SIZE_MINIMIZED))
		{
			if ((resizing) || ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)))
			{
					destWidth = LOWORD(lParam);
					destHeight = HIWORD(lParam);
					windowResize();
			}
		}
		break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
		minMaxInfo->ptMinTrackSize.x = 64;
		minMaxInfo->ptMinTrackSize.y = 64;
		break;
	}
	case WM_ENTERSIZEMOVE:
		resizing = true;
		break;
	case WM_EXITSIZEMOVE:
		resizing = false;
		break;
	}

	//OnHandleMessage(hWnd, uMsg, wParam, lParam);
}

void VulkanExample::setupConsole(std::string title)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w+", stdout);
	freopen_s(&stream, "CONOUT$", "w+", stderr);
	SetConsoleTitle(TEXT(title.c_str()));
}

void VulkanExample::setupDPIAwareness()
{
	typedef HRESULT* (__stdcall* SetProcessDpiAwarenessFunc)(PROCESS_DPI_AWARENESS);

	HMODULE shCore = LoadLibraryA("Shcore.dll");
	if (shCore)
	{
		SetProcessDpiAwarenessFunc setProcessDpiAwareness =
			(SetProcessDpiAwarenessFunc)GetProcAddress(shCore, "SetProcessDpiAwareness");

		if (setProcessDpiAwareness != nullptr)
		{
			setProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		}

		FreeLibrary(shCore);
	}
}


std::string VulkanExample::getWindowTitle()
{
	std::string device(deviceProperties.deviceName);
	std::string windowTitle;
	windowTitle = title + " - " + device;
	if (!settings.overlay) {
		windowTitle += " - " + std::to_string(frameCounter) + " fps";
	}
	return windowTitle;
}


HWND VulkanExample::setupWindow(HINSTANCE hinstance, WNDPROC wndproc) {

	// 到时候要关联swapchain
	this->windowInstance = hinstance;


	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hinstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = name.c_str();
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wndClass))
	{
		std::cout << "Could not register window class!\n";
		fflush(stdout);
		exit(1);
	}

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	DWORD dwExStyle;
	DWORD dwStyle;

	if (settings.fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}


	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = settings.fullscreen ? (long)screenWidth : (long)width;
	windowRect.bottom = settings.fullscreen ? (long)screenHeight : (long)height;

	// 这里不调整，在创建swapchain时会根据实际窗口大小赋值
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	std::string windowTitle = getWindowTitle();
	window = CreateWindowEx(0,
		name.c_str(),
		windowTitle.c_str(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hinstance,
		NULL);


	if (!settings.fullscreen)
	{
		// Center on screen
		uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	if (!window)
	{
		printf("Could not create window!\n");
		fflush(stdout);
		return nullptr;
	}

	ShowWindow(window, SW_SHOW);
	SetForegroundWindow(window);
	SetFocus(window);

	return window;
}

bool VulkanExample::initVulkan()
{
	VkResult err;

	// Vulkan instance
	err = createInstance(settings.validation);
	if (err) {
		vks::tools::exitFatal("Could not create Vulkan instance : \n" + vks::tools::errorString(err), err);
		return false;
	}

	// If requested, we enable the default validation layers for debugging
	if (settings.validation)
	{
		vks::debug::setupDebugging(instance);
	}

	// Physical device
	uint32_t gpuCount = 0;
	// Get number of available physical devices
	VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
	if (gpuCount == 0) {
		vks::tools::exitFatal("No device with Vulkan support found", -1);
		return false;
	}

	// Enumerate devices
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	err = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());
	if (err) {
		vks::tools::exitFatal("Could not enumerate physical devices : \n" + vks::tools::errorString(err), err);
		return false;
	}

	// GPU selection

	// Select physical device to be used for the Vulkan example
	// Defaults to the first device unless specified by command line
	uint32_t selectedDevice = 0;

#if !defined(VK_USE_PLATFORM_ANDROID_KHR)
	// GPU selection via command line argument
	if (commandLineParser.isSet("gpuselection")) {
		uint32_t index = commandLineParser.getValueAsInt("gpuselection", 0);
		if (index > gpuCount - 1) {
			std::cerr << "Selected device index " << index << " is out of range, reverting to device 0 (use -listgpus to show available Vulkan devices)" << "\n";
		}
		else {
			selectedDevice = index;
		}
	}
	if (commandLineParser.isSet("gpulist") || 1 == 1) {
		std::cout << "可用显卡设备：" << "\n";
		for (uint32_t i = 0; i < gpuCount; i++) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
			std::cout << "Device [" << i << "] : " << deviceProperties.deviceName << std::endl;
			std::cout << " Type: " << vks::tools::physicalDeviceTypeString(deviceProperties.deviceType) << "\n";
			//std::cout << " API: " << (deviceProperties.apiVersion >> 22) << "." << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion & 0xfff) << "\n";
		}
	}
#endif

	physicalDevice = physicalDevices[selectedDevice];


	// Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

	// 打印属性
	std::cout << "当前显卡属性：" << "\n";
	std::cout << "  Device Name: " << deviceProperties.deviceName << std::endl;
	std::cout << "  Device Type: " << deviceProperties.deviceType << std::endl;
	std::cout << "  API Version: " << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "." << VK_VERSION_MINOR(deviceProperties.apiVersion) << "." << VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
	std::cout << "  Driver Version: " << deviceProperties.driverVersion << std::endl;
	std::cout << "  Vendor ID: " << deviceProperties.vendorID << std::endl;
	std::cout << "  Device ID: " << deviceProperties.deviceID << std::endl;
	for (int i = 0; i < VK_UUID_SIZE; i++) {
		std::cout << "  " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(deviceProperties.pipelineCacheUUID[i]);
	}
	std::cout << std::dec << std::endl;


	std::cout << "当前显卡特性：" << "\n";
	std::cout << "  robustBufferAccess: " << deviceFeatures.robustBufferAccess << "\n";
	std::cout << "  fullDrawIndexUint32: " << deviceFeatures.fullDrawIndexUint32 << "\n";
	std::cout << "  imageCubeArray: " << deviceFeatures.imageCubeArray << "\n";
	std::cout << "  independentBlend: " << deviceFeatures.independentBlend << "\n";
	std::cout << "  geometryShader: " << deviceFeatures.geometryShader << "\n";
	std::cout << "  tessellationShader: " << deviceFeatures.tessellationShader << "\n";
	std::cout << "  sampleRateShading: " << deviceFeatures.sampleRateShading << "\n";
	std::cout << "  dualSrcBlend: " << deviceFeatures.dualSrcBlend << "\n";
	std::cout << "  logicOp: " << deviceFeatures.logicOp << "\n";
	std::cout << "  multiDrawIndirect: " << deviceFeatures.multiDrawIndirect << "\n";
	std::cout << "  drawIndirectFirstInstance: " << deviceFeatures.drawIndirectFirstInstance << "\n";
	std::cout << "  depthClamp: " << deviceFeatures.depthClamp << "\n";
	std::cout << "  depthBiasClamp: " << deviceFeatures.depthBiasClamp << "\n";
	std::cout << "  fillModeNonSolid: " << deviceFeatures.fillModeNonSolid << "\n";
	std::cout << "  depthBounds: " << deviceFeatures.depthBounds << "\n";
	std::cout << "  wideLines: " << deviceFeatures.wideLines << "\n";
	std::cout << "  largePoints: " << deviceFeatures.largePoints << "\n";
	std::cout << "  alphaToOne: " << deviceFeatures.alphaToOne << "\n";
	std::cout << "  multiViewport: " << deviceFeatures.multiViewport << "\n";
	std::cout << "  samplerAnisotropy: " << deviceFeatures.samplerAnisotropy << "\n";
	std::cout << "  textureCompressionETC2: " << deviceFeatures.textureCompressionETC2 << "\n";
	std::cout << "  textureCompressionASTC_LDR: " << deviceFeatures.textureCompressionASTC_LDR << "\n";
	std::cout << "  textureCompressionBC: " << deviceFeatures.textureCompressionBC << "\n";
	std::cout << "  occlusionQueryPrecise: " << deviceFeatures.occlusionQueryPrecise << "\n";
	std::cout << "  pipelineStatisticsQuery: " << deviceFeatures.pipelineStatisticsQuery << "\n";
	std::cout << "  vertexPipelineStoresAndAtomics: " << deviceFeatures.vertexPipelineStoresAndAtomics << "\n";
	std::cout << "  fragmentStoresAndAtomics: " << deviceFeatures.fragmentStoresAndAtomics << "\n";
	std::cout << "  shaderTessellationAndGeometryPointSize: " << deviceFeatures.shaderTessellationAndGeometryPointSize << "\n";
	std::cout << "  shaderImageGatherExtended: " << deviceFeatures.shaderImageGatherExtended << "\n";
	std::cout << "  shaderStorageImageExtendedFormats: " << deviceFeatures.shaderStorageImageExtendedFormats << "\n";
	std::cout << "  shaderStorageImageMultisample: " << deviceFeatures.shaderStorageImageExtendedFormats << "\n";
	std::cout << "  shaderStorageImageReadWithoutFormat: " << deviceFeatures.shaderStorageImageReadWithoutFormat << "\n";
	std::cout << "  shaderStorageImageWriteWithoutFormat: " << deviceFeatures.shaderStorageImageWriteWithoutFormat << "\n";
	std::cout << "  shaderUniformBufferArrayDynamicIndexing: " << deviceFeatures.shaderUniformBufferArrayDynamicIndexing << "\n";
	std::cout << "  shaderSampledImageArrayDynamicIndexing: " << deviceFeatures.shaderSampledImageArrayDynamicIndexing << "\n";
	std::cout << "  shaderStorageBufferArrayDynamicIndexing: " << deviceFeatures.shaderStorageBufferArrayDynamicIndexing << "\n";
	std::cout << "  shaderStorageImageArrayDynamicIndexing: " << deviceFeatures.shaderStorageImageArrayDynamicIndexing << "\n";
	std::cout << "  shaderClipDistance: " << deviceFeatures.shaderClipDistance << "\n";
	std::cout << "  shaderCullDistance: " << deviceFeatures.shaderCullDistance << "\n";
	std::cout << "  shaderFloat64: " << deviceFeatures.shaderFloat64 << "\n";
	std::cout << "  shaderInt64: " << deviceFeatures.shaderInt64 << "\n";
	std::cout << "  shaderInt16: " << deviceFeatures.shaderInt16 << "\n";
	std::cout << "  shaderResourceResidency: " << deviceFeatures.shaderResourceResidency << "\n";
	std::cout << "  shaderResourceMinLod: " << deviceFeatures.shaderResourceMinLod << "\n";
	std::cout << "  sparseBinding: " << deviceFeatures.sparseBinding << "\n";
	std::cout << "  sparseResidencyBuffer: " << deviceFeatures.sparseResidencyBuffer << "\n";
	std::cout << "  sparseResidencyImage2D: " << deviceFeatures.sparseResidencyImage2D << "\n";
	std::cout << "  sparseResidencyImage3D: " << deviceFeatures.sparseResidencyImage3D << "\n";
	std::cout << "  sparseResidency2Samples: " << deviceFeatures.sparseResidency2Samples << "\n";
	std::cout << "  sparseResidency4Samples: " << deviceFeatures.sparseResidency4Samples << "\n";
	std::cout << "  sparseResidency8Samples: " << deviceFeatures.sparseResidency8Samples << "\n";
	std::cout << "  sparseResidency16Samples: " << deviceFeatures.sparseResidency16Samples << "\n";
	std::cout << "  sparseResidencyAliased: " << deviceFeatures.sparseResidencyAliased << "\n";
	std::cout << "  variableMultisampleRate: " << deviceFeatures.variableMultisampleRate << "\n";
	std::cout << "  inheritedQueries: " << deviceFeatures.inheritedQueries << "\n";



	std::cout << "当前显存信息：" << "\n";
	std::cout << "  Memory Type Count: " << deviceMemoryProperties.memoryTypeCount << "\n";
	for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; ++i) {
		const auto& memoryType = deviceMemoryProperties.memoryTypes[i];
		std::cout << "  Memory Type " << i << " Heap Index: " << memoryType.heapIndex << " Property Flags: " << memoryType.propertyFlags << "\n";
	}

	std::cout << "  Memory Heap Count: " << deviceMemoryProperties.memoryHeapCount << "\n";
	for (uint32_t i = 0; i < deviceMemoryProperties.memoryHeapCount; ++i) {
		const auto& memoryHeap = deviceMemoryProperties.memoryHeaps[i];
		std::cout << "  Memory Heap " << i << " Size: " << memoryHeap.size << " Flags: " << memoryHeap.flags << "\n";
	}



	// Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
	//getEnabledFeatures();

	// Vulkan device creation
	// This is handled by a separate class that gets a logical device representation
	// and encapsulates functions related to a device
	vulkanDevice = new vks::VulkanDevice(physicalDevice);

	// Derived examples can enable extensions based on the list of supported extensions read from the physical device
	//getEnabledExtensions();

	VkResult res = vulkanDevice->createLogicalDevice(enabledFeatures, enabledDeviceExtensions, deviceCreatepNextChain);
	if (res != VK_SUCCESS) {
		vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(res), res);
		return false;
	}
	device = vulkanDevice->logicalDevice;

	// Get a graphics queue from the device
	vkGetDeviceQueue(device, vulkanDevice->queueFamilyIndices.graphics, 0, &queue);

	// 打印所有支持的格式 好像必须自己遍历
	//uint32_t formatCount = 0;
	//VkFormatProperties formatProperties;
	//vkGetPhysicalDeviceFormatProperties(physicalDevice, VK_FORMAT_UNDEFINED, &formatProperties);
	//std::vector<VkFormat> formats(formatCount);

	//for (VkFormat format = VK_FORMAT_UNDEFINED; format < VK_FORMAT_RANGE_SIZE; format++) {
	//	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

	//	if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
	//		std::cout << "Physical Device supports format: " << format << std::endl;
	//	}
	//}




	// Find a suitable depth format
	VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &depthFormat);
	assert(validDepthFormat);

	swapChain.connect(instance, physicalDevice, device);

	// Create synchronization objects
	VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
	// Create a semaphore used to synchronize image presentation
	// Ensures that the image is displayed before we start submitting new commands to the queue
	VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.presentComplete));
	// Create a semaphore used to synchronize command submission
	// Ensures that the image is not presented until all commands have been submitted and executed
	VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.renderComplete));

	// Set up submit info structure
	// Semaphores will stay the same during application lifetime
	// Command buffer submission info is set by each example
	submitInfo = vks::initializers::submitInfo();
	submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphores.presentComplete;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphores.renderComplete;

}



void VulkanExample::renderLoop() {


	destWidth = width;
	destHeight = height;
	lastTimestamp = std::chrono::high_resolution_clock::now();
	tPrevEnd = lastTimestamp;



	// 消息循环了
#if defined(_WIN32)
	MSG msg;
	bool quitMessageReceived = false;
	while (!quitMessageReceived) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				quitMessageReceived = true;
				break;
			}
		}
		if (prepared && !IsIconic(window)) {
			nextFrame();
		}
	}
#endif

	// Flush device to make sure all resources can be freed
	if (device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(device);
	}

}

void VulkanExample::destroyCommandBuffers()
{
	vkFreeCommandBuffers(device, cmdPool, static_cast<uint32_t>(drawCmdBuffers.size()), drawCmdBuffers.data());
}

void VulkanExample::windowResize()
{
	if (!prepared)
	{
		return;
	}
	prepared = false;
	resized = true;

	// Ensure all operations on the device have been finished before destroying resources
	vkDeviceWaitIdle(device);

	// Recreate swap chain
	width = destWidth;
	height = destHeight;
	setupSwapChain();

	// Recreate the frame buffers
	vkDestroyImageView(device, depthStencil.view, nullptr);
	vkDestroyImage(device, depthStencil.image, nullptr);
	vkFreeMemory(device, depthStencil.mem, nullptr);
	setupDepthStencil();
	for (uint32_t i = 0; i < frameBuffers.size(); i++) {
		vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
	}
	setupFrameBuffer();

	if ((width > 0.0f) && (height > 0.0f)) {
		if (settings.overlay) {
			//UIOverlay.resize(width, height);
		}
	}

	// Command buffers need to be recreated as they may store
	// references to the recreated frame buffer
	destroyCommandBuffers();
	createCommandBuffers();
	buildCommandBuffers();

	// SRS - Recreate fences in case number of swapchain images has changed on resize
	for (auto& fence : waitFences) {
		vkDestroyFence(device, fence, nullptr);
	}
	createSynchronizationPrimitives();

	vkDeviceWaitIdle(device);

	if ((width > 0.0f) && (height > 0.0f)) {
		camera.updateAspectRatio((float)width / (float)height);
	}

	// Notify derived class
	//windowResized();
	viewChanged();

	prepared = true;
}

void VulkanExample::prepareFrame()
{
	// Acquire the next image from the swap chain
	VkResult result = swapChain.acquireNextImage(semaphores.presentComplete, &currentBuffer);
	// Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE)
	// SRS - If no longer optimal (VK_SUBOPTIMAL_KHR), wait until submitFrame() in case number of swapchain images will change on resize
	if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			windowResize();
		}
		return;
	}
	else {
		VK_CHECK_RESULT(result);
	}
}

void VulkanExample::submitFrame()
{
	VkResult result = swapChain.queuePresent(queue, currentBuffer, semaphores.renderComplete);
	// Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
	if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
		windowResize();
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			return;
		}
	}
	else {
		VK_CHECK_RESULT(result);
	}
	VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}

void VulkanExample::renderFrame()
{
	VulkanExample::prepareFrame();
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
	VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
	VulkanExample::submitFrame();
}

void VulkanExample::viewChanged()
{
	updateUniformBuffers();
}

void VulkanExample::render()
{
	renderFrame();
	if (camera.updated) {
		updateUniformBuffers();
	}
}

void VulkanExample::nextFrame()
{
	auto tStart = std::chrono::high_resolution_clock::now();
	if (viewUpdated)
	{
		viewUpdated = false;
		viewChanged();
	}

	render();
	frameCounter++;
	auto tEnd = std::chrono::high_resolution_clock::now();
#if (defined(VK_USE_PLATFORM_IOS_MVK) || (defined(VK_USE_PLATFORM_MACOS_MVK) && !defined(VK_EXAMPLE_XCODE_GENERATED)))
	// SRS - Calculate tDiff as time between frames vs. rendering time for iOS/macOS displayLink-driven examples project
	auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tPrevEnd).count();
#else
	auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
#endif
	frameTimer = (float)tDiff / 1000.0f;
	camera.update(frameTimer);
	if (camera.moving())
	{
		viewUpdated = true;
	}
	// Convert to clamped timer value
	if (!paused)
	{
		timer += timerSpeed * frameTimer;
		if (timer > 1.0)
		{
			timer -= 1.0f;
		}
	}
	float fpsTimer = (float)(std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count());
	if (fpsTimer > 1000.0f)
	{
		lastFPS = static_cast<uint32_t>((float)frameCounter * (1000.0f / fpsTimer));
#if defined(_WIN32)
		if (!settings.overlay) {
			std::string windowTitle = getWindowTitle();
			SetWindowText(window, windowTitle.c_str());
		}
#endif
		frameCounter = 0;
		lastTimestamp = tEnd;
	}
	tPrevEnd = tEnd;

	// TODO: Cap UI overlay update rates
	//updateOverlay();
}


void VulkanExample::initSwapchain()
{
#if defined(_WIN32)
	swapChain.initSurface(windowInstance, window);
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//	swapChain.initSurface(androidApp->window);
//#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
//	swapChain.initSurface(view);
//#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
//	swapChain.initSurface(dfb, surface);
//#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
//	swapChain.initSurface(display, surface);
//#elif defined(VK_USE_PLATFORM_XCB_KHR)
//	swapChain.initSurface(connection, window);
//#elif (defined(_DIRECT2DISPLAY) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
//	swapChain.initSurface(width, height);
#endif
}

void VulkanExample::createCommandPool()
{
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = swapChain.queueNodeIndex;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool));
}

void VulkanExample::setupSwapChain()
{
	swapChain.create(&width, &height, settings.vsync, settings.fullscreen);
}

void VulkanExample::createCommandBuffers()
{
	// Create one command buffer for each swap chain image and reuse for rendering
	drawCmdBuffers.resize(swapChain.imageCount);

	VkCommandBufferAllocateInfo cmdBufAllocateInfo =
		vks::initializers::commandBufferAllocateInfo(
			cmdPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			static_cast<uint32_t>(drawCmdBuffers.size()));

	VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, drawCmdBuffers.data()));
}

void VulkanExample::createSynchronizationPrimitives()
{
	// Wait fences to sync command buffer access
	VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
	waitFences.resize(drawCmdBuffers.size());
	for (auto& fence : waitFences) {
		VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));
	}
}

void VulkanExample::setupDepthStencil()
{
	VkImageCreateInfo imageCI{};
	imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCI.imageType = VK_IMAGE_TYPE_2D;
	imageCI.format = depthFormat;
	imageCI.extent = { width, height, 1 };
	imageCI.mipLevels = 1;
	imageCI.arrayLayers = 1;
	imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	VK_CHECK_RESULT(vkCreateImage(device, &imageCI, nullptr, &depthStencil.image));
	VkMemoryRequirements memReqs{};
	vkGetImageMemoryRequirements(device, depthStencil.image, &memReqs);

	VkMemoryAllocateInfo memAllloc{};
	memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllloc.allocationSize = memReqs.size;
	memAllloc.memoryTypeIndex = vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	VK_CHECK_RESULT(vkAllocateMemory(device, &memAllloc, nullptr, &depthStencil.mem));
	VK_CHECK_RESULT(vkBindImageMemory(device, depthStencil.image, depthStencil.mem, 0));

	VkImageViewCreateInfo imageViewCI{};
	imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCI.image = depthStencil.image;
	imageViewCI.format = depthFormat;
	imageViewCI.subresourceRange.baseMipLevel = 0;
	imageViewCI.subresourceRange.levelCount = 1;
	imageViewCI.subresourceRange.baseArrayLayer = 0;
	imageViewCI.subresourceRange.layerCount = 1;
	imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	// Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
	if (depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
		imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCI, nullptr, &depthStencil.view));
}

void VulkanExample::setupRenderPass()
{
	std::array<VkAttachmentDescription, 2> attachments = {};
	// Color attachment
	attachments[0].format = swapChain.colorFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	// Depth attachment
	attachments[1].format = depthFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;
	subpassDescription.pResolveAttachments = nullptr;

	// Subpass dependencies for layout transitions
	std::array<VkSubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	dependencies[0].dependencyFlags = 0;

	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescription;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));
}

void VulkanExample::createPipelineCache()
{
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache));
}

void VulkanExample::setupFrameBuffer()
{
	VkImageView attachments[2];

	// Depth/Stencil attachment is the same for all frame buffers
	attachments[1] = depthStencil.view;

	VkFramebufferCreateInfo frameBufferCreateInfo = {};
	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.pNext = NULL;
	frameBufferCreateInfo.renderPass = renderPass;
	frameBufferCreateInfo.attachmentCount = 2;
	frameBufferCreateInfo.pAttachments = attachments;
	frameBufferCreateInfo.width = width;
	frameBufferCreateInfo.height = height;
	frameBufferCreateInfo.layers = 1;

	// Create frame buffers for every swap chain image
	frameBuffers.resize(swapChain.imageCount);
	for (uint32_t i = 0; i < frameBuffers.size(); i++)
	{
		attachments[0] = swapChain.buffers[i].view;
		VK_CHECK_RESULT(vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &frameBuffers[i]));
	}
}

/*
	Vulkan glTF scene class
*/

VulkanglTFScene::~VulkanglTFScene()
{
	for (auto node : nodes) {
		delete node;
	}
	// Release all Vulkan resources allocated for the model
	vkDestroyBuffer(vulkanDevice->logicalDevice, vertices.buffer, nullptr);
	vkFreeMemory(vulkanDevice->logicalDevice, vertices.memory, nullptr);
	vkDestroyBuffer(vulkanDevice->logicalDevice, indices.buffer, nullptr);
	vkFreeMemory(vulkanDevice->logicalDevice, indices.memory, nullptr);
	for (Image image : images) {
		vkDestroyImageView(vulkanDevice->logicalDevice, image.texture.view, nullptr);
		vkDestroyImage(vulkanDevice->logicalDevice, image.texture.image, nullptr);
		vkDestroySampler(vulkanDevice->logicalDevice, image.texture.sampler, nullptr);
		vkFreeMemory(vulkanDevice->logicalDevice, image.texture.deviceMemory, nullptr);
	}
	for (Material material : materials) {
		vkDestroyPipeline(vulkanDevice->logicalDevice, material.pipeline, nullptr);
	}
}

/*
	glTF loading functions

	The following functions take a glTF input model loaded via tinyglTF and convert all required data into our own structure
*/

void VulkanglTFScene::loadImages(tinygltf::Model& input)
{
	// POI: The textures for the glTF file used in this sample are stored as external ktx files, so we can directly load them from disk without the need for conversion
	images.resize(input.images.size());
	std::cout << "image数量：" << input.images.size() << std::endl;
	for (size_t i = 0; i < input.images.size(); i++) {
		tinygltf::Image& glTFImage = input.images[i];
		std::cout << "  " << i << ".名称：" << glTFImage.name << std::endl;
		std::cout << "    " << "格式：" << glTFImage.mimeType << " " << glTFImage.uri << std::endl;
		images[i].texture.loadFromFile(path + "/" + glTFImage.uri, VK_FORMAT_R8G8B8A8_UNORM, vulkanDevice, copyQueue);
	}
}

void VulkanglTFScene::loadTextures(tinygltf::Model& input)
{
	textures.resize(input.textures.size());
	std::cout << "texture(纹理)数量：" << input.textures.size() << std::endl;
	for (size_t i = 0; i < input.textures.size(); i++) {
		textures[i].imageIndex = input.textures[i].source;
	}
}

void VulkanglTFScene::loadMaterials(tinygltf::Model& input)
{
	materials.resize(input.materials.size());
	std::cout << "material(材质)数量：" << input.materials.size() << std::endl;
	for (size_t i = 0; i < input.materials.size(); i++) {
		// We only read the most basic properties required for our sample
		tinygltf::Material glTFMaterial = input.materials[i];
		std::cout << "  " << i << ".名称：" << glTFMaterial.name << std::endl;
		// Get the base color factor
		if (glTFMaterial.values.find("baseColorFactor") != glTFMaterial.values.end()) {
			materials[i].baseColorFactor = glm::make_vec4(glTFMaterial.values["baseColorFactor"].ColorFactor().data());
		}
		// Get base color texture index
		if (glTFMaterial.values.find("baseColorTexture") != glTFMaterial.values.end()) {
			materials[i].baseColorTextureIndex = glTFMaterial.values["baseColorTexture"].TextureIndex();
			std::cout << "    baseColorTexture 索引：" << glTFMaterial.values["baseColorTexture"].TextureIndex() << std::endl;
		}
		// Get the normal map texture index
		if (glTFMaterial.additionalValues.find("normalTexture") != glTFMaterial.additionalValues.end()) {
			std::cout << "    normalTexture 索引：" << glTFMaterial.additionalValues["normalTexture"].TextureIndex() << std::endl;
			materials[i].normalTextureIndex = glTFMaterial.additionalValues["normalTexture"].TextureIndex();
		}
		// Get some additional material parameters that are used in this sample
		materials[i].alphaMode = glTFMaterial.alphaMode;
		materials[i].alphaCutOff = (float)glTFMaterial.alphaCutoff;
		materials[i].doubleSided = glTFMaterial.doubleSided;
		std::cout << "    doubleSided：" << std::boolalpha << glTFMaterial.doubleSided << std::endl;
	}
}

void VulkanglTFScene::loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, VulkanglTFScene::Node* parent, std::vector<uint32_t>& indexBuffer, std::vector<VulkanglTFScene::Vertex>& vertexBuffer)
{
	VulkanglTFScene::Node* node = new VulkanglTFScene::Node{};
	node->name = inputNode.name;
	node->parent = parent;

	// Get the local node matrix
	// It's either made up from translation, rotation, scale or a 4x4 matrix
	node->matrix = glm::mat4(1.0f);
	if (inputNode.translation.size() == 3) {
		node->matrix = glm::translate(node->matrix, glm::vec3(glm::make_vec3(inputNode.translation.data())));
	}
	if (inputNode.rotation.size() == 4) {
		glm::quat q = glm::make_quat(inputNode.rotation.data());
		node->matrix *= glm::mat4(q);
	}
	if (inputNode.scale.size() == 3) {
		node->matrix = glm::scale(node->matrix, glm::vec3(glm::make_vec3(inputNode.scale.data())));
	}
	if (inputNode.matrix.size() == 16) {
		node->matrix = glm::make_mat4x4(inputNode.matrix.data());
	};
	std::cout << "  model 矩阵：" << std::endl;

	std::cout << "    [" << node->matrix[0][0] << " " << node->matrix[0][1] << " " << node->matrix[0][2] << " " << node->matrix[0][3] << "] " << std::endl;
	std::cout << "    [" << node->matrix[1][0] << " " << node->matrix[1][1] << " " << node->matrix[1][2] << " " << node->matrix[1][3] << "] " << std::endl;
	std::cout << "    [" << node->matrix[2][0] << " " << node->matrix[2][1] << " " << node->matrix[2][2] << " " << node->matrix[2][3] << "] " << std::endl;
	std::cout << "    [" << node->matrix[3][0] << " " << node->matrix[3][1] << " " << node->matrix[3][2] << " " << node->matrix[3][3] << "] " << std::endl;

	// Load node's children
	if (inputNode.children.size() > 0) {
		for (size_t i = 0; i < inputNode.children.size(); i++) {
			loadNode(input.nodes[inputNode.children[i]], input, node, indexBuffer, vertexBuffer);
		}
	}

	// If the node contains mesh data, we load vertices and indices from the buffers
	// In glTF this is done via accessors and buffer views
	if (inputNode.mesh > -1) {
		const tinygltf::Mesh mesh = input.meshes[inputNode.mesh];

		std::cout << "  mesh 名称：" << mesh.name << std::endl;

		// Iterate through all primitives of this node's mesh
		for (size_t i = 0; i < mesh.primitives.size(); i++) {
			const tinygltf::Primitive& glTFPrimitive = mesh.primitives[i];

			uint32_t firstIndex = static_cast<uint32_t>(indexBuffer.size());
			uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
			uint32_t indexCount = 0;
			// Vertices
			{
				const float* positionBuffer = nullptr;
				const float* normalsBuffer = nullptr;
				const float* texCoordsBuffer = nullptr;
				const float* tangentsBuffer = nullptr;
				size_t vertexCount = 0;

				// Get buffer data for vertex normals
				if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("POSITION")->second];
					std::cout << "    primitives[" << i << "].POSITION accessors 索引：" << glTFPrimitive.attributes.find("POSITION")->second << std::endl;
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					std::cout << "    primitives[" << i << "].POSITION bufferView 索引：" << accessor.bufferView << std::endl;
					std::cout << "    primitives[" << i << "].POSITION.bufferView view.byteOffset：" << view.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].POSITION.bufferView accessor.byteOffset：" << accessor.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].POSITION.bufferView accessor.count(顶点数,每个顶点3个float)：" << accessor.count << std::endl;
					std::cout << "    primitives[" << i << "].POSITION buffer 索引：" << view.buffer << "起始字节：" << accessor.byteOffset + view.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].POSITION buffer 名称：" << input.buffers[view.buffer].uri << std::endl;
					positionBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
					vertexCount = accessor.count;
				}
				// Get buffer data for vertex normals
				if (glTFPrimitive.attributes.find("NORMAL") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("NORMAL")->second];
					std::cout << "    primitives[" << i << "].NORMAL accessors 索引：" << glTFPrimitive.attributes.find("NORMAL")->second << std::endl;
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					std::cout << "    primitives[" << i << "].NORMAL bufferView 索引：" << accessor.bufferView << std::endl;
					std::cout << "    primitives[" << i << "].NORMAL.bufferView view.byteOffset：" << view.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].NORMAL.bufferView accessor.byteOffset：" << accessor.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].NORMAL.bufferView accessor.count(同上)：" << accessor.count << std::endl;
					std::cout << "    primitives[" << i << "].NORMAL buffer 索引：" << view.buffer << "起始字节：" << accessor.byteOffset + view.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].NORMAL buffer 名称：" << input.buffers[view.buffer].uri << std::endl;
					normalsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}
				// Get buffer data for vertex texture coordinates
				// glTF supports multiple sets, we only load the first one
				if (glTFPrimitive.attributes.find("TEXCOORD_0") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("TEXCOORD_0")->second];
					std::cout << "    primitives[" << i << "].TEXCOORD_0 accessors 索引：" << glTFPrimitive.attributes.find("TEXCOORD_0")->second << std::endl;
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					std::cout << "    primitives[" << i << "].TEXCOORD_0 bufferView 索引：" << accessor.bufferView << std::endl;
					std::cout << "    primitives[" << i << "].TEXCOORD_0.bufferView view.byteOffset：" << view.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].TEXCOORD_0.bufferView accessor.byteOffset：" << accessor.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].TEXCOORD_0.bufferView accessor.count(同上，每个顶点2个float)：" << accessor.count << std::endl;
					std::cout << "    primitives[" << i << "].TEXCOORD_0 buffer 索引：" << view.buffer << "起始字节：" << accessor.byteOffset + view.byteOffset << std::endl;
					std::cout << "    primitives[" << i << "].TEXCOORD_0 buffer 名称：" << input.buffers[view.buffer].uri << std::endl;
					texCoordsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}
				// POI: This sample uses normal mapping, so we also need to load the tangents from the glTF file
				if (glTFPrimitive.attributes.find("TANGENT") != glTFPrimitive.attributes.end()) {
					const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("TANGENT")->second];
					const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
					tangentsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				}


				//std::cout << "  打印顶点和法线：" << std::endl;

				// Append data to model's vertex buffer
				for (size_t v = 0; v < vertexCount; v++) {
					Vertex vert{};
					vert.pos = glm::vec4(glm::make_vec3(&positionBuffer[v * 3]), 1.0f);
					//std::cout << "    " << v << ". (" << vert.pos.x << ", " << vert.pos.y << ", " << vert.pos.z << ")" << std::endl;
					vert.normal = glm::normalize(glm::vec3(normalsBuffer ? glm::make_vec3(&normalsBuffer[v * 3]) : glm::vec3(0.0f)));
					//std::cout << "    " << v << ". (" << vert.normal.x << ", " << vert.normal.y << ", " << vert.normal.z << ")" << std::endl;
					vert.uv = texCoordsBuffer ? glm::make_vec2(&texCoordsBuffer[v * 2]) : glm::vec3(0.0f);
					vert.color = glm::vec3(1.0f);
					vert.tangent = tangentsBuffer ? glm::make_vec4(&tangentsBuffer[v * 4]) : glm::vec4(0.0f);
					vertexBuffer.push_back(vert);
				}
			}
			// Indices
			{
				const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.indices];
				std::cout << "    primitives[" << i << "].indices 索引：" << glTFPrimitive.indices << std::endl;
				const tinygltf::BufferView& bufferView = input.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = input.buffers[bufferView.buffer];

				std::cout << "    primitives[" << i << "].indices bufferView 索引：" << accessor.bufferView << std::endl;
				std::cout << "    primitives[" << i << "].indices.bufferView view.byteOffset：" << bufferView.byteOffset << std::endl;
				std::cout << "    primitives[" << i << "].indices.bufferView accessor.byteOffset：" << accessor.byteOffset << std::endl;
				std::cout << "    primitives[" << i << "].indices.bufferView accessor.count(索引数)：" << accessor.count << std::endl;
				std::cout << "    primitives[" << i << "].indices buffer 索引：" << bufferView.buffer << "起始字节：" << accessor.byteOffset + bufferView.byteOffset << "[索引可能共用]" << std::endl;
				std::cout << "    primitives[" << i << "].indices buffer 名称：" << buffer.uri << std::endl;
				// std::cout << "    vertexStart ？：" << vertexStart << std::endl; 只是为了在上一次基础增加


				indexCount += static_cast<uint32_t>(accessor.count);

				// glTF supports different component types of indices
				switch (accessor.componentType) {
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
					const uint32_t* buf = reinterpret_cast<const uint32_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
					for (size_t index = 0; index < accessor.count; index++) {
						indexBuffer.push_back(buf[index] + vertexStart);
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {

					const uint16_t* buf = reinterpret_cast<const uint16_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
					for (size_t index = 0; index < accessor.count; index++) {
						indexBuffer.push_back(buf[index] + vertexStart);
					}

					//std::cout << "  打印索引 [类型：ushort]：" << std::endl;
					for (size_t index = 0; index < accessor.count; index += 3) {
						//std::cout << "    " << (index) / 3 << ". " << buf[index] << " " << buf[index + 1] << " " << buf[index + 2] << std::endl;
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
					const uint8_t* buf = reinterpret_cast<const uint8_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
					for (size_t index = 0; index < accessor.count; index++) {
						indexBuffer.push_back(buf[index] + vertexStart);
					}
					break;
				}
				default:
					std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
					return;
				}
			}
			Primitive primitive{};
			primitive.firstIndex = firstIndex;
			std::cout << "    indexBuffer 起始索引[每个索引3个int]：" << firstIndex << std::endl;
			primitive.indexCount = indexCount;
			primitive.materialIndex = glTFPrimitive.material;
			std::cout << "    primitives[" << i << "].material 索引：" << glTFPrimitive.material << std::endl;
			node->mesh.primitives.push_back(primitive);
		}
	}

	if (parent) {
		parent->children.push_back(node);
	}
	else {
		nodes.push_back(node);
	}
}


VkDescriptorImageInfo VulkanglTFScene::getTextureDescriptor(const size_t index)
{
	return images[index].texture.descriptor;
}

/*
	glTF rendering functions
*/

// Draw a single node including child nodes (if present)
void VulkanglTFScene::drawNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VulkanglTFScene::Node* node)
{
	if (!node->visible) {
		return;
	}
	if (node->mesh.primitives.size() > 0) {
		// Pass the node's matrix via push constants
		// Traverse the node hierarchy to the top-most parent to get the final matrix of the current node
		glm::mat4 nodeMatrix = node->matrix;
		VulkanglTFScene::Node* currentParent = node->parent;
		while (currentParent) {
			nodeMatrix = currentParent->matrix * nodeMatrix;
			currentParent = currentParent->parent;
		}
		// Pass the final matrix to the vertex shader using push constants
		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &nodeMatrix);
		for (VulkanglTFScene::Primitive& primitive : node->mesh.primitives) {
			if (primitive.indexCount > 0) {
				VulkanglTFScene::Material& material = materials[primitive.materialIndex];
				// POI: Bind the pipeline for the node's material
				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material.pipeline);
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, &material.descriptorSet, 0, nullptr);
				vkCmdDrawIndexed(commandBuffer, primitive.indexCount, 1, primitive.firstIndex, 0, 0);
			}
		}
	}
	for (auto& child : node->children) {
		drawNode(commandBuffer, pipelineLayout, child);
	}
}

// Draw the glTF scene starting at the top-level-nodes
void VulkanglTFScene::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
{
	// All vertices and indices are stored in single buffers, so we only need to bind once
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices.buffer, offsets);
	vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
	// Render all nodes at top-level
	for (auto& node : nodes) {
		drawNode(commandBuffer, pipelineLayout, node);
	}
}


void VulkanExample::loadglTFFile(std::string filename)
{
	tinygltf::Model glTFInput;
	tinygltf::TinyGLTF gltfContext;
	std::string error, warning;

	this->device = device;

#if defined(__ANDROID__)
	// On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset manager
	// We let tinygltf handle this, by passing the asset manager of our app
	tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
	bool fileLoaded = gltfContext.LoadASCIIFromFile(&glTFInput, &error, &warning, filename);

	// Pass some Vulkan resources required for setup and rendering to the glTF model loading class
	glTFScene.vulkanDevice = vulkanDevice;
	glTFScene.copyQueue = queue;

	size_t pos = filename.find_last_of('/');
	glTFScene.path = filename.substr(0, pos);

	std::vector<uint32_t> indexBuffer;
	std::vector<VulkanglTFScene::Vertex> vertexBuffer;

	if (fileLoaded) {
		std::cout << "============================================================================================" << std::endl;
		glTFScene.loadImages(glTFInput);
		std::cout << "============================================================================================" << std::endl;
		glTFScene.loadMaterials(glTFInput);
		std::cout << "============================================================================================" << std::endl;
		glTFScene.loadTextures(glTFInput);
		std::cout << "============================================================================================" << std::endl;
		const tinygltf::Scene& scene = glTFInput.scenes[0];
		std::cout << "nodes数量：" << scene.nodes.size() << std::endl;
		std::cout << "vert内存格式：vector<VulkanglTFScene::Vertex> " << std::endl;
		std::cout << "index内存格式：vector<uint32_t> " << std::endl;
		std::cout << "============================================================================================" << std::endl;

		for (size_t i = 0; i < scene.nodes.size(); i++) {
			const tinygltf::Node node = glTFInput.nodes[scene.nodes[i]];
			std::cout << "node：" << node.name << std::endl;
			std::cout << "  mesh 序号：" << node.mesh << std::endl;
			glTFScene.loadNode(node, glTFInput, nullptr, indexBuffer, vertexBuffer);
		}
	}
	else {
		vks::tools::exitFatal("Could not open the glTF file.\n\nThe file is part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
		return;
	}

	// Create and upload vertex and index buffer
	// We will be using one single vertex buffer and one single index buffer for the whole glTF scene
	// Primitives (of the glTF model) will then index into these using index offsets

	size_t vertexBufferSize = vertexBuffer.size() * sizeof(VulkanglTFScene::Vertex);
	size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	glTFScene.indices.count = static_cast<uint32_t>(indexBuffer.size());
	std::cout << "============================================================================================" << std::endl;

	std::cout << "Vertex buffer总大小：" << vertexBuffer.size() << "个 x " << sizeof(VulkanglTFScene::Vertex) << "(Vertex) = " << vertexBufferSize << "字节" << std::endl;
	std::cout << "Index buffer总大小：" << indexBuffer.size() << "个 x " << sizeof(uint32_t) << "(int32) = " << indexBufferSize << "字节" << std::endl;

	struct StagingBuffer {
		VkBuffer buffer;
		VkDeviceMemory memory;
	} vertexStaging, indexStaging;

	// Create host visible staging buffers (source)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertexBufferSize,
		&vertexStaging.buffer,
		&vertexStaging.memory,
		vertexBuffer.data()));
	// Index data
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		indexBufferSize,
		&indexStaging.buffer,
		&indexStaging.memory,
		indexBuffer.data()));

	// Create device local buffers (target)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBufferSize,
		&glTFScene.vertices.buffer,
		&glTFScene.vertices.memory));
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBufferSize,
		&glTFScene.indices.buffer,
		&glTFScene.indices.memory));

	// Copy data from staging buffers (host) do device local buffer (gpu)
	VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
	VkBufferCopy copyRegion = {};

	copyRegion.size = vertexBufferSize;
	vkCmdCopyBuffer(
		copyCmd,
		vertexStaging.buffer,
		glTFScene.vertices.buffer,
		1,
		&copyRegion);

	copyRegion.size = indexBufferSize;
	vkCmdCopyBuffer(
		copyCmd,
		indexStaging.buffer,
		glTFScene.indices.buffer,
		1,
		&copyRegion);

	vulkanDevice->flushCommandBuffer(copyCmd, queue, true);

	// Free staging resources
	vkDestroyBuffer(device, vertexStaging.buffer, nullptr);
	vkFreeMemory(device, vertexStaging.memory, nullptr);
	vkDestroyBuffer(device, indexStaging.buffer, nullptr);
	vkFreeMemory(device, indexStaging.memory, nullptr);
}



void VulkanExample::loadAssets()
{
	std::cout << "开始导入gltf" << std::endl;
	loadglTFFile("C:/WorkMe/gltf_export/test_ktx/origin2/vulakan.gltf");
	//loadglTFFile("D:/WorkGitbub3/Vulkan/data/models/sponza/sponza.gltf");

}

void VulkanExample::prepareUniformBuffers()
{
	std::cout << "创建UniformBuffer" << std::endl;
	std::cout << "  UniformBuffer结构 ShaderData.Values 大小：" << sizeof(shaderData.values) << std::endl;
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&shaderData.buffer,
		sizeof(shaderData.values)));
	VK_CHECK_RESULT(shaderData.buffer.map());
	updateUniformBuffers();
}

void VulkanExample::updateUniformBuffers()
{
	shaderData.values.projection = camera.matrices.perspective;
	shaderData.values.view = camera.matrices.view;
	shaderData.values.viewPos = camera.viewPos;
	memcpy(shaderData.buffer.mapped, &shaderData.values, sizeof(shaderData.values));
}


void VulkanExample::setupDescriptors()
{
	/*
		This sample uses separate descriptor sets (and layouts) for the matrices and materials (textures)
	*/

	// One ubo to pass dynamic data to the shader
	// Two combined image samplers per material as each material uses color and normal maps
	std::vector<VkDescriptorPoolSize> poolSizes = {
		vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
		vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(glTFScene.materials.size()) * 2),
	};
	// One set for matrices and one per model image/texture
	const uint32_t maxSetCount = static_cast<uint32_t>(glTFScene.images.size()) + 1;
	VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, maxSetCount);
	VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));

	// Descriptor set layout for passing matrices
	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0)
	};
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings.data(), static_cast<uint32_t>(setLayoutBindings.size()));

	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.matrices));

	// Descriptor set layout for passing material textures
	setLayoutBindings = {
		// Color map
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0),
		// Normal map
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1),
	};
	descriptorSetLayoutCI.pBindings = setLayoutBindings.data();
	descriptorSetLayoutCI.bindingCount = 2;
	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.textures));

	// Pipeline layout using both descriptor sets (set 0 = matrices, set 1 = material)
	std::array<VkDescriptorSetLayout, 2> setLayouts = { descriptorSetLayouts.matrices, descriptorSetLayouts.textures };
	VkPipelineLayoutCreateInfo pipelineLayoutCI = vks::initializers::pipelineLayoutCreateInfo(setLayouts.data(), static_cast<uint32_t>(setLayouts.size()));
	// We will use push constants to push the local matrices of a primitive to the vertex shader
	VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0);
	// Push constant ranges are part of the pipeline layout
	pipelineLayoutCI.pushConstantRangeCount = 1;
	pipelineLayoutCI.pPushConstantRanges = &pushConstantRange;
	VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &pipelineLayout));

	// Descriptor set for scene matrices
	VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.matrices, 1);
	VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));
	VkWriteDescriptorSet writeDescriptorSet = vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &shaderData.buffer.descriptor);
	vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);

	// Descriptor sets for materials
	for (auto& material : glTFScene.materials) {
		const VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.textures, 1);
		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &material.descriptorSet));
		VkDescriptorImageInfo colorMap = glTFScene.getTextureDescriptor(material.baseColorTextureIndex);
		VkDescriptorImageInfo normalMap = glTFScene.getTextureDescriptor(material.normalTextureIndex);
		std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
			vks::initializers::writeDescriptorSet(material.descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &colorMap),
			vks::initializers::writeDescriptorSet(material.descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &normalMap),
		};
		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}
}

std::string VulkanExample::getShadersPath() const
{
	return getAssetPath() + "shaders/" + shaderDir + "/";
}


void VulkanExample::preparePipelines()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCI = vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCI = vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);
	VkPipelineColorBlendAttachmentState blendAttachmentStateCI = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
	VkPipelineColorBlendStateCreateInfo colorBlendStateCI = vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentStateCI);
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCI = vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
	VkPipelineViewportStateCreateInfo viewportStateCI = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);
	VkPipelineMultisampleStateCreateInfo multisampleStateCI = vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, 0);
	const std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicStateCI = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables.data(), static_cast<uint32_t>(dynamicStateEnables.size()), 0);
	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

	const std::vector<VkVertexInputBindingDescription> vertexInputBindings = {
		vks::initializers::vertexInputBindingDescription(0, sizeof(VulkanglTFScene::Vertex), VK_VERTEX_INPUT_RATE_VERTEX),
	};
	const std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
		vks::initializers::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFScene::Vertex, pos)),
		vks::initializers::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFScene::Vertex, normal)),
		vks::initializers::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFScene::Vertex, uv)),
		vks::initializers::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFScene::Vertex, color)),
		vks::initializers::vertexInputAttributeDescription(0, 4, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanglTFScene::Vertex, tangent)),
	};
	VkPipelineVertexInputStateCreateInfo vertexInputStateCI = vks::initializers::pipelineVertexInputStateCreateInfo(vertexInputBindings, vertexInputAttributes);

	VkGraphicsPipelineCreateInfo pipelineCI = vks::initializers::pipelineCreateInfo(pipelineLayout, renderPass, 0);
	pipelineCI.pVertexInputState = &vertexInputStateCI;
	pipelineCI.pInputAssemblyState = &inputAssemblyStateCI;
	pipelineCI.pRasterizationState = &rasterizationStateCI;
	pipelineCI.pColorBlendState = &colorBlendStateCI;
	pipelineCI.pMultisampleState = &multisampleStateCI;
	pipelineCI.pViewportState = &viewportStateCI;
	pipelineCI.pDepthStencilState = &depthStencilStateCI;
	pipelineCI.pDynamicState = &dynamicStateCI;
	pipelineCI.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineCI.pStages = shaderStages.data();

	shaderStages[0] = loadShader(getShadersPath() + "gltfscenerendering/scene.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	//shaderStages[1] = loadShader(getShadersPath() + "gltfscenerendering/scene-nolight.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	shaderStages[1] = loadShader(getShadersPath() + "gltfscenerendering/scene.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

	// POI: Instead if using a few fixed pipelines, we create one pipeline for each material using the properties of that material
	for (auto& material : glTFScene.materials) {

		struct MaterialSpecializationData {
			VkBool32 alphaMask;
			float alphaMaskCutoff;
		} materialSpecializationData;

		materialSpecializationData.alphaMask = material.alphaMode == "MASK";
		materialSpecializationData.alphaMaskCutoff = material.alphaCutOff;

		// POI: Constant fragment shader material parameters will be set using specialization constants
		std::vector<VkSpecializationMapEntry> specializationMapEntries = {
			vks::initializers::specializationMapEntry(0, offsetof(MaterialSpecializationData, alphaMask), sizeof(MaterialSpecializationData::alphaMask)),
			vks::initializers::specializationMapEntry(1, offsetof(MaterialSpecializationData, alphaMaskCutoff), sizeof(MaterialSpecializationData::alphaMaskCutoff)),
		};
		VkSpecializationInfo specializationInfo = vks::initializers::specializationInfo(specializationMapEntries, sizeof(materialSpecializationData), &materialSpecializationData);
		shaderStages[1].pSpecializationInfo = &specializationInfo;

		// For double sided materials, culling will be disabled
		rasterizationStateCI.cullMode = material.doubleSided ? VK_CULL_MODE_NONE : VK_CULL_MODE_BACK_BIT;

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &material.pipeline));
	}
}


VkPipelineShaderStageCreateInfo VulkanExample::loadShader(std::string fileName, VkShaderStageFlagBits stage)
{
	VkPipelineShaderStageCreateInfo shaderStage = {};
	shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStage.stage = stage;
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
	shaderStage.module = vks::tools::loadShader(androidApp->activity->assetManager, fileName.c_str(), device);
#else
	shaderStage.module = vks::tools::loadShader(fileName.c_str(), device);
#endif
	shaderStage.pName = "main";
	assert(shaderStage.module != VK_NULL_HANDLE);
	shaderModules.push_back(shaderStage.module);
	return shaderStage;
}


void VulkanExample::buildCommandBuffers()
{
	VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

	VkClearValue clearValues[2];
	clearValues[0].color = defaultClearColor;
	//clearValues[0].color = { { 0.8f, 0.8f, 0.8f, 1.0f } };;
	clearValues[0].color = { { 0.25f, 0.25f, 0.25f, 1.0f } };;
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = width;
	renderPassBeginInfo.renderArea.extent.height = height;
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

	const VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
	const VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);

	for (int32_t i = 0; i < drawCmdBuffers.size(); ++i)
	{
		renderPassBeginInfo.framebuffer = frameBuffers[i];
		VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo));
		vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);
		vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissor);
		// Bind scene matrices descriptor to set 0
		vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

		// POI: Draw the glTF scene
		glTFScene.draw(drawCmdBuffers[i], pipelineLayout);

		drawUI(drawCmdBuffers[i]);
		vkCmdEndRenderPass(drawCmdBuffers[i]);
		VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
	}
}

void VulkanExample::drawUI(const VkCommandBuffer commandBuffer)
{
	//if (settings.overlay && UIOverlay.visible) {
	//	const VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
	//	const VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);
	//	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	//	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	//	UIOverlay.draw(commandBuffer);
	//}
}




void VulkanExample::prepare() {

	initSwapchain();
	createCommandPool();
	setupSwapChain();
	createCommandBuffers();
	createSynchronizationPrimitives();

	setupDepthStencil();
	setupRenderPass();
	createPipelineCache();

	setupFrameBuffer();

	//settings.overlay = settings.overlay && (!benchmark.active);
	//settings.overlay = settings.overlay && (!benchmark.active);
	//if (settings.overlay) {
	//	UIOverlay.device = vulkanDevice;
	//	UIOverlay.queue = queue;
	//	UIOverlay.shaders = {
	//		loadShader(getShadersPath() + "base/uioverlay.vert.spv", VK_SHADER_STAGE_VERTEX_BIT),
	//		loadShader(getShadersPath() + "base/uioverlay.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT),
	//	};
	//	UIOverlay.prepareResources();
	//	UIOverlay.preparePipeline(pipelineCache, renderPass, swapChain.colorFormat, depthFormat);
	//}



	loadAssets();
	std::cout << "读取Assets结束[创建VertexBuffer、IndexBuffer、Image、ImageView]" << std::endl;
	std::cout << "============================================================================================" << std::endl;
	prepareUniformBuffers();
	std::cout << "准备UniformBuffers结束" << std::endl;
	std::cout << "============================================================================================" << std::endl;
	setupDescriptors();
	std::cout << "准备Descriptors结束[描述ShaderUniform参数：Uniform矩阵、Uniform材质(2个)、constant矩阵]" << std::endl;
	std::cout << "============================================================================================" << std::endl;
	preparePipelines();
	std::cout << "准备Pipelines结束[设置Shader文件、描述VertexShader入参、设置VkPipelineLayout(里面Uniform相关的描述)]" << std::endl;
	std::cout << "============================================================================================" << std::endl;
	buildCommandBuffers();
	std::cout << "准备CommandBuffers结束" << std::endl;
	std::cout << "============================================================================================" << std::endl;
	prepared = true;


}

//void ConsoleAlloc() {
//#ifndef DISABLE_LOGGING_CONSOLE
//	AllocConsole();
//
//	SetConsoleTitleA("Debug Console");
//
//	freopen_s(reinterpret_cast<FILE**>(stdin), "conin$", "r", stdin);
//	freopen_s(reinterpret_cast<FILE**>(stdout), "conout$", "w", stdout);
//
//	::ShowWindow(GetConsoleWindow(), SW_SHOW);
//#endif
//}
//
//
//void ConsoleFree() {
//#ifndef DISABLE_LOGGING_CONSOLE
//	fclose(stdin);
//	fclose(stdout);
//
//
//	FreeConsole();
//
//#endif
//}



VkResult VulkanExample::createInstance(bool enableValidation)
{
	this->settings.validation = enableValidation;

	// Validation can also be forced via a define
#if defined(_VALIDATION)
	this->settings.validation = true;
#endif

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = name.c_str();
	appInfo.pEngineName = name.c_str();
	appInfo.apiVersion = apiVersion;

	std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

	// Enable surface extensions depending on os
#if defined(_WIN32)
	instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#endif

	// Get extensions supported by the instance and store for later use
	uint32_t extCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	if (extCount > 0)
	{

		std::cout << "支持的Instance扩展:" << "\n";
		std::vector<VkExtensionProperties> extensions(extCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
		{
			for (VkExtensionProperties extension : extensions)
			{
				supportedInstanceExtensions.push_back(extension.extensionName);
				std::cout << "  " << supportedInstanceExtensions.size() << "." << extension.extensionName << "\n";

			}
		}
	}



	// Enabled requested instance extensions
	if (enabledInstanceExtensions.size() > 0)
	{
		for (const char* enabledExtension : enabledInstanceExtensions)
		{
			// Output message if requested extension is not available
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), enabledExtension) == supportedInstanceExtensions.end())
			{
				std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
			}
			instanceExtensions.push_back(enabledExtension);
		}
	}

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.pApplicationInfo = &appInfo;


	std::cout << "启动的扩展:" << "\n";
	if (instanceExtensions.size() > 0)
	{
		if (settings.validation)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);	// SRS - Dependency when VK_EXT_DEBUG_MARKER is enabled
			instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();


		// 输出一下启动的扩展
		int i = 0;
		for (const char* enabledExtension : instanceExtensions)
		{
			std::cout << "  " << ++i << "." << enabledExtension << "\n";
		}
	}

	// The VK_LAYER_KHRONOS_validation contains all current validation functionality.
	// Note that on Android this layer requires at least NDK r20
	const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
	if (settings.validation)
	{
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		bool validationLayerPresent = false;
		for (VkLayerProperties layer : instanceLayerProperties) {
			if (strcmp(layer.layerName, validationLayerName) == 0) {
				validationLayerPresent = true;
				break;
			}
		}
		if (validationLayerPresent) {
			instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
			instanceCreateInfo.enabledLayerCount = 1;
		}
		else {
			std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
		}
	}
	return vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}







VulkanExample* vulkanExample;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (vulkanExample != NULL)
	{
		vulkanExample->handleMessages(hWnd, uMsg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	//for (size_t i = 0; i < __argc; i++) { VulkanExample::args.push_back(__argv[i]); };
	//ConsoleAlloc();
	vulkanExample = new VulkanExample();
	vulkanExample->initVulkan();
	vulkanExample->setupWindow(hInstance, WndProc);
	vulkanExample->prepare();
	vulkanExample->renderLoop();
	delete(vulkanExample);
	return 0;
}