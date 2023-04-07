

#include "VulkanExampleBase.h"
// =======================  event ========================

void VulkanExampleBase::mouseMoved(double x, double y, bool& handled) {}

void VulkanExampleBase::windowResized() {}

void VulkanExampleBase::viewChanged() {}

void VulkanExampleBase::buildCommandBuffers() {}

void VulkanExampleBase::OnUpdateUIOverlay(vks::UIOverlay* overlay) {}

// =======================  event ========================


// =======================  private ========================

std::string VulkanExampleBase::getWindowTitle()
{
	std::string device(deviceProperties.deviceName);
	std::string windowTitle;
	windowTitle = title + " - " + device;
	//if (!settings.overlay) {
	windowTitle += " - " + std::to_string(frameCounter) + " fps";
	//}
	return windowTitle;
}

void VulkanExampleBase::handleMouseMove(int32_t x, int32_t y)
{
	int32_t dx = (int32_t)mousePos.x - x;
	int32_t dy = (int32_t)mousePos.y - y;

	bool handled = false;

	if (settings.overlay) {
		ImGuiIO& io = ImGui::GetIO();
		handled = io.WantCaptureMouse && UIOverlay.visible;
	}
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

void VulkanExampleBase::windowResize()
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
			UIOverlay.resize(width, height);
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
	windowResized();
	viewChanged();

	prepared = true;
}

void VulkanExampleBase::destroyCommandBuffers()
{
	vkFreeCommandBuffers(device, cmdPool, static_cast<uint32_t>(drawCmdBuffers.size()), drawCmdBuffers.data());
}

// =======================  private ========================

VulkanExampleBase::VulkanExampleBase()
{

#if defined(_WIN32)
	// Enable console if validation is active, debug message callback will output to it
	if (this->settings.validation)
	{
		setupConsole("Vulkan Project");
	}
	setupDPIAwareness();
#endif
}

void VulkanExampleBase::setupConsole(std::string title)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w+", stdout);
	freopen_s(&stream, "CONOUT$", "w+", stderr);
	SetConsoleTitle(TEXT(title.c_str()));
}

void VulkanExampleBase::setupDPIAwareness()
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

void VulkanExampleBase::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		case KEY_F1:
			UIOverlay.visible = !UIOverlay.visible;
			UIOverlay.updated = true;
			break;
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

HWND VulkanExampleBase::setupWindow(HINSTANCE hinstance, WNDPROC wndproc) {

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




VkResult VulkanExampleBase::createInstance(bool enableValidation)
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

void VulkanExampleBase::getEnabledFeatures() {}

void VulkanExampleBase::getEnabledExtensions() {}

bool VulkanExampleBase::initVulkan()
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
	getEnabledFeatures();

	// Vulkan device creation
	// This is handled by a separate class that gets a logical device representation
	// and encapsulates functions related to a device
	vulkanDevice = new vks::VulkanDevice(physicalDevice);

	// Derived examples can enable extensions based on the list of supported extensions read from the physical device
	getEnabledExtensions();

	VkResult res = vulkanDevice->createLogicalDevice(enabledFeatures, enabledDeviceExtensions, deviceCreatepNextChain);
	if (res != VK_SUCCESS) {
		vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(res), res);
		return false;
	}
	device = vulkanDevice->logicalDevice;

	//SRS - Get Vulkan device driver information if available, use later for display
	if (vulkanDevice->extensionSupported(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME))
	{
		VkPhysicalDeviceProperties2 deviceProperties2 = {};
		deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		deviceProperties2.pNext = &driverProperties;
		driverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;
		vkGetPhysicalDeviceProperties2(vulkanDevice->physicalDevice, &deviceProperties2);
	}



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

void VulkanExampleBase::initSwapchain()
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

void VulkanExampleBase::createCommandPool()
{
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = swapChain.queueNodeIndex;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool));
}

void VulkanExampleBase::setupSwapChain()
{
	swapChain.create(&width, &height, settings.vsync, settings.fullscreen);
}

void VulkanExampleBase::createCommandBuffers()
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


void VulkanExampleBase::createSynchronizationPrimitives()
{
	// Wait fences to sync command buffer access
	VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
	waitFences.resize(drawCmdBuffers.size());
	for (auto& fence : waitFences) {
		VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));
	}
}



void VulkanExampleBase::setupDepthStencil()
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

void VulkanExampleBase::setupRenderPass()
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

void VulkanExampleBase::createPipelineCache()
{
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache));
}

void VulkanExampleBase::setupFrameBuffer()
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



void VulkanExampleBase::renderLoop() {
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


void VulkanExampleBase::nextFrame()
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
		//if (!settings.overlay) {
		std::string windowTitle = getWindowTitle();
		SetWindowText(window, windowTitle.c_str());
		//}
#endif
		frameCounter = 0;
		lastTimestamp = tEnd;
	}
	tPrevEnd = tEnd;

	// TODO: Cap UI overlay update rates
	updateOverlay();
}

void VulkanExampleBase::updateOverlay()
{
	if (!settings.overlay)
		return;

	ImGuiIO& io = ImGui::GetIO();


	io.DisplaySize = ImVec2((float)width, (float)height);
	io.DeltaTime = frameTimer;

	io.MousePos = ImVec2(mousePos.x, mousePos.y);
	io.MouseDown[0] = mouseButtons.left && UIOverlay.visible;
	io.MouseDown[1] = mouseButtons.right && UIOverlay.visible;
	io.MouseDown[2] = mouseButtons.middle && UIOverlay.visible;


	OnUpdateUIOverlay(&UIOverlay);






	ImGui::Render();

	//buildCommandBuffers();
	if (UIOverlay.update() || UIOverlay.updated) {
		buildCommandBuffers();
		UIOverlay.updated = false;
	}


}






void VulkanExampleBase::prepareFrame()
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

void VulkanExampleBase::submitFrame()
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