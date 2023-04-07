// c-project-cmake.cpp: 定义应用程序的入口点。
//

#include "c-project-cmake.h"


#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

#include "VulkanglTFScene.h"


using namespace std;


// Options and values to display/toggle from the UI
struct UISettings {
	bool displayModels = true;
	bool displayLogos = true;
	bool displayBackground = true;
	bool animateLight = false;
	float lightSpeed = 0.25f;
	std::array<float, 50> frameTimes{};
	float frameTimeMin = 9999.0f, frameTimeMax = 0.0f;
	float lightTimer = 0.0f;
} uiSettings;


VulkanExample::VulkanExample() :VulkanExampleBase(){


	title = "glTF scene rendering";
	camera.type = Camera::CameraType::firstperson;
	camera.flipY = true;
	camera.setPosition(glm::vec3(-0.9f, 1.0f, 0.0f));
	camera.setRotation(glm::vec3(0.0f, -24.0f, 0.0f));
	camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);
}













void VulkanExample::getEnabledFeatures()
{
	enabledFeatures.samplerAnisotropy = deviceFeatures.samplerAnisotropy;
}







void VulkanExample::renderFrame()
{
	VulkanExample::prepareFrame();

	if (settings.overlay && UIOverlay.visible) {
		buildCommandBuffers();
	}
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



void VulkanExample::OnUpdateUIOverlay(vks::UIOverlay* overlay)
{


	ImGui::NewFrame();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::SetNextWindowPos(ImVec2(10 * UIOverlay.scale, 10 * UIOverlay.scale));
	//ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::Begin("Vulkan Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::TextUnformatted(title.c_str());



	ImGui::PushItemWidth(110.0f * UIOverlay.scale);







	ImGui::PopItemWidth();

	ImGui::End();
	ImGui::PopStyleVar();




	ImGui::Begin(u8"显卡信息");
	ImGui::TextUnformatted(vulkanDevice->properties.deviceName);
	ImGui::Text("Vulkan API %i.%i.%i", VK_API_VERSION_MAJOR(vulkanDevice->properties.apiVersion), VK_API_VERSION_MINOR(vulkanDevice->properties.apiVersion), VK_API_VERSION_PATCH(vulkanDevice->properties.apiVersion));
	ImGui::Text("%s %s", driverProperties.driverName, driverProperties.driverInfo);

	// Update frame time display
	if ((frameCounter == 0)) {
		std::rotate(uiSettings.frameTimes.begin(), uiSettings.frameTimes.begin() + 1, uiSettings.frameTimes.end());
		float frameTime = 1000.0f / (frameTimer * 1000.0f);
		uiSettings.frameTimes.back() = frameTime;
		if (frameTime < uiSettings.frameTimeMin) {
			uiSettings.frameTimeMin = frameTime;
		}
		if (frameTime > uiSettings.frameTimeMax) {
			uiSettings.frameTimeMax = frameTime;
		}
	}

	ImGui::PlotLines("Frame Times", &uiSettings.frameTimes[0], 50, 0, "", uiSettings.frameTimeMin, uiSettings.frameTimeMax, ImVec2(0, 80));
	ImGui::Text("%.2f ms/frame (%.1d fps)", (1000.0f / lastFPS), lastFPS);
	ImGui::End();
	ImGui::Begin(u8"状态信息");
	ImGui::Text(u8"相机信息");
	ImGui::InputFloat3(u8"位置", &camera.position.x, "%.1f");
	ImGui::InputFloat3(u8"旋转", &camera.rotation.x, "%.1f");
	ImGui::End();
	ImGui::Begin(u8"场景信息");
	if (UIOverlay.header("Visibility")) {

		if (UIOverlay.button("All")) {
			std::for_each(glTFScene.nodes.begin(), glTFScene.nodes.end(), [](VulkanglTFScene::Node* node) { node->visible = true; });
			buildCommandBuffers();
		}
		ImGui::SameLine();
		if (UIOverlay.button("None")) {
			std::for_each(glTFScene.nodes.begin(), glTFScene.nodes.end(), [](VulkanglTFScene::Node* node) { node->visible = false; });
			buildCommandBuffers();
		}
		ImGui::NewLine();

		// POI: Create a list of glTF nodes for visibility toggle
		//ImGui::BeginChild("#nodelist", ImVec2(200.0f * UIOverlay.scale, 340.0f * UIOverlay.scale), false);
		for (auto& node : glTFScene.nodes)
		{

			if (ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{

				ImGui::Text(u8"顶点数量:%d", node->countPosNor);
				ImGui::Text(u8"顶点大小:%d字节 [%d * float[3]]", node->countPosNor * 4 * 3, node->countPosNor);
				ImGui::Text(u8"索引数量:%d - %d组", node->countIndex, node->countIndex / 3);
				ImGui::Text(u8"索引大小:%d字节 [%d * int]", node->countIndex * 4, node->countPosNor);
				if (UIOverlay.checkBox(u8"是否隐藏", &node->visible))
				{
					buildCommandBuffers();
				}



				ImGui::TreePop();
			}





		}
		//ImGui::EndChild();
	}


	ImGui::End();

	//if (overlay->header("Visibility")) {

	//	if (overlay->button("All")) {
	//		std::for_each(glTFScene.nodes.begin(), glTFScene.nodes.end(), [](VulkanglTFScene::Node* node) { node->visible = true; });
	//		buildCommandBuffers();
	//	}
	//	ImGui::SameLine();
	//	if (overlay->button("None")) {
	//		std::for_each(glTFScene.nodes.begin(), glTFScene.nodes.end(), [](VulkanglTFScene::Node* node) { node->visible = false; });
	//		buildCommandBuffers();
	//	}
	//	ImGui::NewLine();

	//	// POI: Create a list of glTF nodes for visibility toggle
	//	ImGui::BeginChild("#nodelist", ImVec2(200.0f * overlay->scale, 340.0f * overlay->scale), false);
	//	for (auto& node : glTFScene.nodes)
	//	{
	//		if (overlay->checkBox(node->name.c_str(), &node->visible))
	//		{
	//			buildCommandBuffers();
	//		}
	//	}
	//	ImGui::EndChild();
	//}
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
	if (settings.overlay && UIOverlay.visible) {
		const VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
		const VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		UIOverlay.draw(commandBuffer);
	}
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

	settings.overlay = settings.overlay;//&& (!benchmark.active);
	if (settings.overlay) {
		UIOverlay.device = vulkanDevice;
		UIOverlay.queue = queue;
		UIOverlay.shaders = {
			loadShader(getShadersPath() + "base/uioverlay.vert.spv", VK_SHADER_STAGE_VERTEX_BIT),
			loadShader(getShadersPath() + "base/uioverlay.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT),
		};
		UIOverlay.prepareResources();
		UIOverlay.preparePipeline(pipelineCache, renderPass, swapChain.colorFormat, depthFormat);
	}



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
	vulkanExample = new VulkanExample();
	vulkanExample->initVulkan();
	vulkanExample->setupWindow(hInstance, WndProc);
	vulkanExample->prepare();
	vulkanExample->renderLoop();
	delete(vulkanExample);
	return 0;
}