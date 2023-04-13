// c-project-cmake.cpp: 定义应用程序的入口点。
//

#include "c-project-cmake.h"

//
//#define TINYGLTF_NO_STB_IMAGE_WRITE
//#define TINYGLTF_NO_STB_IMAGE
//#define TINYGLTF_NO_EXTERNAL_IMAGE
//#include "tiny_gltf.h"

//#include "VulkanglTFScene.h"
//#include "base/VulkanglTFModel.h"


using namespace std;


// 设置开启的Features
void VulkanExample::getEnabledFeatures()
{
	enabledFeatures.samplerAnisotropy = deviceFeatures.samplerAnisotropy;

	// 启动geometryShader
	if (deviceFeatures.geometryShader) {
		enabledFeatures.geometryShader = VK_TRUE;
	}
	else {
		vks::tools::exitFatal("Selected GPU does not support geometry shaders!", VK_ERROR_FEATURE_NOT_PRESENT);
	}

}



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


VulkanExample::VulkanExample() :VulkanExampleBase() {


	title = "glTF scene rendering";
	camera.type = Camera::CameraType::firstperson;
	camera.flipY = true;
	camera.setPosition(glm::vec3(-0.9f, 1.0f, 0.0f));
	camera.setRotation(glm::vec3(0.0f, -24.0f, 0.0f));
	camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);
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

	if (!UIOverlay.visible) {
		return;
	}

	ImGui::NewFrame();
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	//ImGui::SetNextWindowPos(ImVec2(10 * UIOverlay.scale, 10 * UIOverlay.scale));
	////ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	//ImGui::Begin("Vulkan Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	//ImGui::TextUnformatted(title.c_str());

	//ImGui::PushItemWidth(110.0f * UIOverlay.scale);
	//ImGui::PopItemWidth();

	//ImGui::End();
	//ImGui::PopStyleVar();




	ImGui::Begin(u8"设备信息 [F1隐藏]");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar(u8"Tab", tab_bar_flags)) {

		if (ImGui::BeginTabItem(u8"状态", 0, 0)) {
			ImGui::Text(u8"显卡信息：");
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

			ImGui::Separator();
			ImGui::Text(u8"显存信息：");


			for (uint32_t i = 0; i < deviceMemoryProperties.memoryHeapCount; ++i) {
				const auto& memoryHeap = deviceMemoryProperties.memoryHeaps[i];
				ImGui::Text(u8"%d.%s:%dM", i + 1, (memoryHeap.flags == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) ? u8"本地内存" : u8"共享内存", memoryHeap.size / 1024 / 1024);

				for (uint32_t j = 0; j < deviceMemoryProperties.memoryTypeCount; ++j) {
					const auto& memoryType = deviceMemoryProperties.memoryTypes[j];
					if (memoryType.heapIndex == i) {
						ImGui::Text(u8"   MemoryPropertyFlags: %d", memoryType.propertyFlags);
					}
				}
			}


			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"扩展信息[Instance]", 0, 0)) {

			ImGui::Text(u8"支持的扩展[%d]：", supportedInstanceExtensions.size());
			int i = 1;
			for (const auto& supportedExtension : supportedInstanceExtensions)
			{
				bool found = false;
				for (const auto& enabledExtensions : instanceExtensions) {
					if (supportedExtension == std::string(enabledExtensions)) {
						found = true;
						break;
					}
				}

				if (found)
					ImGui::Text(u8"%d.%s [已开启]", i++, supportedExtension.c_str());
				else
					ImGui::Text(u8"%d.%s", i++, supportedExtension.c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"扩展信息[Device]", 0, 0)) {

			ImGui::Text(u8"支持的扩展[%d]：", vulkanDevice->supportedExtensions.size());
			int i = 1;
			for (const auto& supported : vulkanDevice->supportedExtensions)
			{
				ImGui::Text(u8"%d.%s", i++, supported.c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"特性信息", 0, 0)) {

			ImGui::Text(u8"特性是否支持：");
			int i = 1;
			ImGui::Text(u8"%d.robustBufferAccess: %s", i++, deviceFeatures.robustBufferAccess ? "true" : "false");
			ImGui::Text(u8"%d.fullDrawIndexUint32: %s", i++, deviceFeatures.fullDrawIndexUint32 ? "true" : "false");
			ImGui::Text(u8"%d.imageCubeArray: %s", i++, deviceFeatures.imageCubeArray ? "true" : "false");
			ImGui::Text(u8"%d.independentBlend: %s", i++, deviceFeatures.independentBlend ? "true" : "false");
			ImGui::Text(u8"%d.geometryShader: %s", i++, deviceFeatures.geometryShader ? "true" : "false");
			ImGui::Text(u8"%d.tessellationShader: %s", i++, deviceFeatures.tessellationShader ? "true" : "false");
			ImGui::Text(u8"%d.sampleRateShading: %s", i++, deviceFeatures.sampleRateShading ? "true" : "false");
			ImGui::Text(u8"%d.dualSrcBlend: %s", i++, deviceFeatures.dualSrcBlend ? "true" : "false");
			ImGui::Text(u8"%d.logicOp: %s", i++, deviceFeatures.logicOp ? "true" : "false");
			ImGui::Text(u8"%d.multiDrawIndirect: %s", i++, deviceFeatures.multiDrawIndirect ? "true" : "false");
			ImGui::Text(u8"%d.drawIndirectFirstInstance: %s", i++, deviceFeatures.drawIndirectFirstInstance ? "true" : "false");
			ImGui::Text(u8"%d.depthClamp: %s", i++, deviceFeatures.depthClamp ? "true" : "false");
			ImGui::Text(u8"%d.depthBiasClamp: %s", i++, deviceFeatures.depthBiasClamp ? "true" : "false");
			ImGui::Text(u8"%d.fillModeNonSolid: %s", i++, deviceFeatures.fillModeNonSolid ? "true" : "false");
			ImGui::Text(u8"%d.depthBounds: %s", i++, deviceFeatures.depthBounds ? "true" : "false");
			ImGui::Text(u8"%d.wideLines: %s", i++, deviceFeatures.wideLines ? "true" : "false");
			ImGui::Text(u8"%d.largePoints: %s", i++, deviceFeatures.largePoints ? "true" : "false");
			ImGui::Text(u8"%d.alphaToOne: %s", i++, deviceFeatures.alphaToOne ? "true" : "false");
			ImGui::Text(u8"%d.multiViewport: %s", i++, deviceFeatures.multiViewport ? "true" : "false");
			ImGui::Text(u8"%d.samplerAnisotropy: %s", i++, deviceFeatures.samplerAnisotropy ? "true" : "false");
			ImGui::Text(u8"%d.textureCompressionETC2: %s", i++, deviceFeatures.textureCompressionETC2 ? "true" : "false");
			ImGui::Text(u8"%d.textureCompressionASTC_LDR: %s", i++, deviceFeatures.textureCompressionASTC_LDR ? "true" : "false");
			ImGui::Text(u8"%d.textureCompressionBC: %s", i++, deviceFeatures.textureCompressionBC ? "true" : "false");
			ImGui::Text(u8"%d.occlusionQueryPrecise: %s", i++, deviceFeatures.occlusionQueryPrecise ? "true" : "false");
			ImGui::Text(u8"%d.pipelineStatisticsQuery: %s", i++, deviceFeatures.pipelineStatisticsQuery ? "true" : "false");
			ImGui::Text(u8"%d.vertexPipelineStoresAndAtomics: %s", i++, deviceFeatures.vertexPipelineStoresAndAtomics ? "true" : "false");
			ImGui::Text(u8"%d.fragmentStoresAndAtomics: %s", i++, deviceFeatures.fragmentStoresAndAtomics ? "true" : "false");
			ImGui::Text(u8"%d.shaderTessellationAndGeometryPointSize: %s", i++, deviceFeatures.shaderTessellationAndGeometryPointSize ? "true" : "false");
			ImGui::Text(u8"%d.shaderImageGatherExtended: %s", i++, deviceFeatures.shaderImageGatherExtended ? "true" : "false");
			ImGui::Text(u8"%d.shaderStorageImageExtendedFormats: %s", i++, deviceFeatures.shaderStorageImageExtendedFormats ? "true" : "false");
			ImGui::Text(u8"%d.shaderStorageImageMultisample: %s", i++, deviceFeatures.shaderStorageImageExtendedFormats ? "true" : "false");
			ImGui::Text(u8"%d.shaderStorageImageReadWithoutFormat: %s", i++, deviceFeatures.shaderStorageImageReadWithoutFormat ? "true" : "false");
			ImGui::Text(u8"%d.shaderStorageImageWriteWithoutFormat: %s", i++, deviceFeatures.shaderStorageImageWriteWithoutFormat ? "true" : "false");
			ImGui::Text(u8"%d.shaderUniformBufferArrayDynamicIndexing: %s", i++, deviceFeatures.shaderUniformBufferArrayDynamicIndexing ? "true" : "false");
			ImGui::Text(u8"%d.shaderSampledImageArrayDynamicIndexing: %s", i++, deviceFeatures.shaderSampledImageArrayDynamicIndexing ? "true" : "false");
			ImGui::Text(u8"%d.shaderStorageBufferArrayDynamicIndexing: %s", i++, deviceFeatures.shaderStorageBufferArrayDynamicIndexing ? "true" : "false");
			ImGui::Text(u8"%d.shaderStorageImageArrayDynamicIndexing: %s", i++, deviceFeatures.shaderStorageImageArrayDynamicIndexing ? "true" : "false");
			ImGui::Text(u8"%d.shaderClipDistance: %s", i++, deviceFeatures.shaderClipDistance ? "true" : "false");
			ImGui::Text(u8"%d.shaderCullDistance: %s", i++, deviceFeatures.shaderCullDistance ? "true" : "false");
			ImGui::Text(u8"%d.shaderFloat64: %s", i++, deviceFeatures.shaderFloat64 ? "true" : "false");
			ImGui::Text(u8"%d.shaderInt64: %s", i++, deviceFeatures.shaderInt64 ? "true" : "false");
			ImGui::Text(u8"%d.shaderInt16: %s", i++, deviceFeatures.shaderInt16 ? "true" : "false");
			ImGui::Text(u8"%d.shaderResourceResidency: %s", i++, deviceFeatures.shaderResourceResidency ? "true" : "false");
			ImGui::Text(u8"%d.shaderResourceMinLod: %s", i++, deviceFeatures.shaderResourceMinLod ? "true" : "false");
			ImGui::Text(u8"%d.sparseBinding: %s", i++, deviceFeatures.sparseBinding ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidencyBuffer: %s", i++, deviceFeatures.sparseResidencyBuffer ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidencyImage2D: %s", i++, deviceFeatures.sparseResidencyImage2D ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidencyImage3D: %s", i++, deviceFeatures.sparseResidencyImage3D ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidency2Samples: %s", i++, deviceFeatures.sparseResidency2Samples ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidency4Samples: %s", i++, deviceFeatures.sparseResidency4Samples ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidency8Samples: %s", i++, deviceFeatures.sparseResidency8Samples ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidency16Samples: %s", i++, deviceFeatures.sparseResidency16Samples ? "true" : "false");
			ImGui::Text(u8"%d.sparseResidencyAliased: %s", i++, deviceFeatures.sparseResidencyAliased ? "true" : "false");
			ImGui::Text(u8"%d.variableMultisampleRate: %s", i++, deviceFeatures.variableMultisampleRate ? "true" : "false");
			ImGui::Text(u8"%d.inheritedQueries: %s", i++, deviceFeatures.inheritedQueries ? "true" : "false");

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();


	ImGui::Begin(u8"状态信息");
	ImGui::Text(u8"坐标：Y朝上,Z向里");
	ImGui::Separator();





	ImGui::Text(u8"光线信息");
	ImGui::Text(u8"光照模型: Phong");
	ImGui::SliderFloat3(u8"方向向量[XYZ]", &shaderData.values.lightPos.x, -90.0f, 90.0f);
	ImGui::SliderFloat(u8"环境光", &shaderDataCustom.values.ambient, 0.0f, 1.0f);
	ImGui::SliderFloat(u8"高光强度", &shaderDataCustom.values.specularExponent, 0.0f, 100.0f);

	ImGui::Separator();

	ImGui::Text(u8"相机信息");
	ImGui::SliderFloat(u8"移动速度", &camera.movementSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat(u8"旋转速度", &camera.rotationSpeed, 0.1f, 2.0f);
	ImGui::SliderFloat(u8"Fov", &camera.fov, 5.0f, 170.0f); ImGui::SameLine();
	if (ImGui::SmallButton(u8"更新##1")) {
		camera.updatePerspective();
	}
	ImGui::SliderFloat(u8"近平面", &camera.znear, 0.1f, 10.0f); ImGui::SameLine();
	if (ImGui::SmallButton(u8"更新##2")) {
		camera.updatePerspective();
	}
	ImGui::SliderFloat(u8"远平面", &camera.zfar, 20.0f, 1000.0f); ImGui::SameLine();
	if (ImGui::SmallButton(u8"更新##3")) {
		camera.updatePerspective();
	}
	ImGui::Text(u8"相机方向: X: %.3f, Y: %.3f, Z: %.3f", camera.camFront.x, camera.camFront.y, camera.camFront.z);
	ImGui::InputFloat3(u8"位置", &camera.position.x, u8"%.3f"); ImGui::SameLine();
	if (ImGui::SmallButton(u8"置0##1")) {
		camera.position.x = 0;
		camera.position.y = 0;
		camera.position.z = 0;
	}
	ImGui::InputFloat3(u8"旋转", &camera.rotation.x, u8"%.1f°"); ImGui::SameLine();
	if (ImGui::SmallButton(u8"置0##2")) {
		camera.rotation.x = 0;
		camera.rotation.y = 0;
		camera.rotation.z = 0;
	}

	ImGui::Separator();
	ImGui::Text(u8"矩阵：[% .2f, % .2f, % .2f, % .2f]", shaderData.values.view[0][0], shaderData.values.view[1][0], shaderData.values.view[2][0], shaderData.values.view[3][0]);
	ImGui::Text(u8"视图　[% .2f, % .2f, % .2f, % .2f]", shaderData.values.view[0][1], shaderData.values.view[1][1], shaderData.values.view[2][1], shaderData.values.view[3][1]);
	ImGui::Text(u8"　　　[% .2f, % .2f, % .2f, % .2f]", shaderData.values.view[0][2], shaderData.values.view[1][2], shaderData.values.view[2][2], shaderData.values.view[3][2]);
	ImGui::Text(u8"　　　[% .2f, % .2f, % .2f, % .2f]", shaderData.values.view[0][3], shaderData.values.view[1][3], shaderData.values.view[2][3], shaderData.values.view[3][3]);

	ImGui::Separator();
	ImGui::Text(u8"矩阵：[% .2f, % .2f, % .2f, % .2f]", shaderData.values.projection[0][0], shaderData.values.projection[1][0], shaderData.values.projection[2][0], shaderData.values.projection[3][0]);
	ImGui::Text(u8"投影　[% .2f, % .2f, % .2f, % .2f]", shaderData.values.projection[0][1], shaderData.values.projection[1][1], shaderData.values.projection[2][1], shaderData.values.projection[3][1]);
	ImGui::Text(u8"　　　[% .2f, % .2f, % .2f, % .2f]", shaderData.values.projection[0][2], shaderData.values.projection[1][2], shaderData.values.projection[2][2], shaderData.values.projection[3][2]);
	ImGui::Text(u8"　　　[% .2f, % .2f, % .2f, % .2f]", shaderData.values.projection[0][3], shaderData.values.projection[1][3], shaderData.values.projection[2][3], shaderData.values.projection[3][3]);



	ImGui::End();


	ImGui::Begin(u8"场景信息");


	if (UIOverlay.header(u8"天空盒")) {
		if (overlay->checkBox(u8"是否隐藏", &displaySkybox)) {
			buildCommandBuffers();
		}


	}

	if (UIOverlay.header(u8"调试信息")) {
		if (overlay->checkBox(u8"显示法线", &displayNormals)) {
			buildCommandBuffers();
		}
		if (displayNormals) {
			ImGui::SliderFloat(u8"法线长度", &shaderDataCustom.values.normalLength, 0.01f, 10.0f);
		}
	}







	if (UIOverlay.header(u8"模型信息")) {

		if (UIOverlay.button(u8"显示所有")) {
			std::for_each(glTFScene.nodes.begin(), glTFScene.nodes.end(), [](VulkanglTFScene::Node* node) { node->visible = true; });
			buildCommandBuffers();
		}
		ImGui::SameLine();
		if (UIOverlay.button(u8"隐藏所有")) {
			std::for_each(glTFScene.nodes.begin(), glTFScene.nodes.end(), [](VulkanglTFScene::Node* node) { node->visible = false; });
			buildCommandBuffers();
		}
		//ImGui::NewLine();
		// POI: Create a list of glTF nodes for visibility toggle
		//ImGui::BeginChild("#nodelist", ImVec2(200.0f * UIOverlay.scale, 340.0f * UIOverlay.scale), false);
		for (auto& node : glTFScene.nodes)
		{
			if (ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (UIOverlay.checkBox(u8"是否隐藏", &node->visible))
				{
					buildCommandBuffers();
				}

				int i = 1;
				for (auto& primitive : node->mesh.primitives) {
					ImGuiTreeNodeFlags_ flag = ImGuiTreeNodeFlags_NoAutoOpenOnLog;
					if (i == 1) {
						flag = ImGuiTreeNodeFlags_DefaultOpen;
					}


					std::string name = std::to_string(i++) + u8" - 索引：" + std::to_string(primitive.firstIndex) + " [" + std::to_string(primitive.countIndex) + " * int]";


					if (ImGui::TreeNodeEx(name.c_str(), flag))
					{

						ImGui::Text(u8"导入前");
						ImGui::Separator();
						ImGui::TextDisabled(u8"顶点数量:"); ImGui::SameLine();
						ImGui::Text(u8"%d", primitive.countPosNor);
						ImGui::TextDisabled(u8"顶点大小:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节 [%d * float[3]]", primitive.countPosNor * 4 * 3, primitive.countPosNor);
						ImGui::TextDisabled(u8"顶点数据:"); ImGui::SameLine();
						ImGui::Text(u8"偏移:%d字节 - %s[%d]", primitive.posOffsetAccessor + primitive.posOffsetBufferView, primitive.posUri.c_str(), primitive.posIndexBufferView);

						ImGui::TextDisabled(u8"法线大小:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节 [%d * float[3]]", primitive.countPosNor * 4 * 3, primitive.countPosNor);
						ImGui::TextDisabled(u8"法线数据:"); ImGui::SameLine();
						ImGui::Text(u8"偏移:%d字节 - %s[%d]", primitive.norOffsetAccessor + primitive.norOffsetBufferView, primitive.norUri.c_str(), primitive.norIndexBufferView);

						if (primitive.tgtExist) {
							ImGui::TextDisabled(u8"切线大小:"); ImGui::SameLine();
							ImGui::Text(u8"%d字节 [%d * float[4]]", primitive.countPosNor * 4 * 4, primitive.countPosNor);
							ImGui::TextDisabled(u8"切线数据:"); ImGui::SameLine();
							ImGui::Text(u8"偏移:%d字节 - %s[%d]", primitive.tgtOffsetAccessor + primitive.tgtOffsetBufferView, primitive.tgtUri.c_str(), primitive.tgtIndexBufferView);
						}

						ImGui::TextDisabled(u8"UV大小:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节 [%d * float[2]]", primitive.countPosNor * 4 * 2, primitive.countPosNor);
						ImGui::TextDisabled(u8"UV数据:"); ImGui::SameLine();
						ImGui::Text(u8"偏移:%d字节 - %s[%d]", primitive.uvOffsetAccessor + primitive.uvOffsetBufferView, primitive.uvUri.c_str(), primitive.uvIndexBufferView);

						ImGui::TextDisabled(u8"索引数量:"); ImGui::SameLine();
						ImGui::Text(u8"%d - %d组", primitive.countIndex, primitive.countIndex / 3);
						ImGui::TextDisabled(u8"索引大小:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节 [%d * int]", primitive.countIndex * 4, primitive.countIndex); ImGui::SameLine();
						ImGui::Text(u8" (Buffer中:%d字节 [%d * %s])", primitive.countIndex * primitive.idxSize, primitive.countIndex, primitive.idxType.c_str());
						ImGui::TextDisabled(u8"索引数据:"); ImGui::SameLine();
						ImGui::Text(u8"偏移:%d字节 - %s[%d]", primitive.idxOffsetAccessor + primitive.idxOffsetBufferView, primitive.idxUri.c_str(), primitive.idxIndexBufferView);

						ImGui::TextDisabled(u8"材质:"); ImGui::SameLine();
						ImGui::Text(u8"%d", primitive.materialIndex);


						ImGui::NewLine();
						ImGui::Text(u8"导入后");
						ImGui::Separator();
						ImGui::TextDisabled(u8"IndexBuffer[大小]:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节 [%d * int]", primitive.indexCount * 4, primitive.indexCount);
						ImGui::TextDisabled(u8"IndexBuffer[偏移]:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节", primitive.firstIndex * 4);
						ImGui::TextDisabled(u8"VertexBuffer[大小]:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节 [%d * VulkanglTFScene::Vertex]", primitive.vertexCount * sizeof(VulkanglTFScene::Vertex), primitive.vertexCount);
						ImGui::TextDisabled(u8"VertexBuffer[偏移]:"); ImGui::SameLine();
						ImGui::Text(u8"%d字节", primitive.firstVertex * sizeof(VulkanglTFScene::Vertex));

						ImGui::NewLine();
						ImGui::TreePop();

					}
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

	// 天空盒
	const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;
	glTFModels.skyBox.loadFromFile(getAssetPath() + "models/cube.gltf", vulkanDevice, queue, glTFLoadingFlags);
	cubemap.loadFromFile(getAssetPath() + "textures/cubemap_space.ktx", VK_FORMAT_R8G8B8A8_UNORM, vulkanDevice, queue);

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

	// 新增的自定义的
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&shaderDataCustom.buffer,
		sizeof(shaderDataCustom.values)));
	VK_CHECK_RESULT(shaderDataCustom.buffer.map());


	// 新增的天空盒
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&shaderDataSkybox.buffer,
		sizeof(shaderDataSkybox.ubo)));
	VK_CHECK_RESULT(shaderDataSkybox.buffer.map());


	updateUniformBuffers();
}

void VulkanExample::updateUniformBuffers()
{

	shaderData.values.projection = camera.matrices.perspective;
	shaderData.values.view = camera.matrices.view;
	shaderData.values.viewPos = camera.viewPos;
	memcpy(shaderData.buffer.mapped, &shaderData.values, sizeof(shaderData.values));



	memcpy(shaderDataCustom.buffer.mapped, &shaderDataCustom.values, sizeof(shaderDataCustom.values));


	// 天空盒 显示异常是受flipY影响
	shaderDataSkybox.ubo.projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 256.0f);

	if (camera.flipY) {
		shaderDataSkybox.ubo.view = glm::mat4(glm::mat3(camera.matrices.viewNoFlipY));
	}
	else {
		shaderDataSkybox.ubo.view = glm::mat4(glm::mat3(camera.matrices.view));
	}
	shaderDataSkybox.ubo.model = glm::mat4(1.0f);
	memcpy(shaderDataSkybox.buffer.mapped, &shaderDataSkybox.ubo, sizeof(shaderDataSkybox.ubo));
}


void VulkanExample::setupDescriptors()
{
	/*
		This sample uses separate descriptor sets (and layouts) for the matrices and materials (textures)
	*/

	// One ubo to pass dynamic data to the shader
	// Two combined image samplers per material as each material uses color and normal maps
	std::vector<VkDescriptorPoolSize> poolSizes = {
		vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 5),
		vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(glTFScene.materials.size()) * 2),
		//vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1) // 添加参数
	};
	// One set for matrices and one per model image/texture
	const uint32_t maxSetCount = static_cast<uint32_t>(glTFScene.images.size()) + 5;
	VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, maxSetCount);
	VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));

	// Descriptor set layout for passing matrices
	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
		// 加一个geom用
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_GEOMETRY_BIT, 1)
	};
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings.data(), static_cast<uint32_t>(setLayoutBindings.size()));

	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.matrices));


	// 2023/4/11 新增一个setlayout
	setLayoutBindings = {
	   vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 0),
	   vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_GEOMETRY_BIT, 1)
	};
	descriptorSetLayoutCI.pBindings = setLayoutBindings.data();
	descriptorSetLayoutCI.bindingCount = 2;
	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.customSet2));


	// 写错阶段会报错 Shader uses descriptor slot 0.0 but descriptor not accessible from stage VK_SHADER_STAGE_VERTEX_BIT The Vulkan spec states: layout must be consistent with all shaders specified in pStages 
	setLayoutBindings = {
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
		vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
	};
	descriptorSetLayoutCI.pBindings = setLayoutBindings.data();
	descriptorSetLayoutCI.bindingCount = 2;
	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayouts.skybox));



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
	// 新增一个
	std::array<VkDescriptorSetLayout, 3> setLayouts = {
		descriptorSetLayouts.matrices
		, descriptorSetLayouts.textures
		, descriptorSetLayouts.customSet2 };

	VkPipelineLayoutCreateInfo pipelineLayoutCI = vks::initializers::pipelineLayoutCreateInfo(setLayouts.data(), static_cast<uint32_t>(setLayouts.size()));
	// We will use push constants to push the local matrices of a primitive to the vertex shader
	VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0);
	// Push constant ranges are part of the pipeline layout
	pipelineLayoutCI.pushConstantRangeCount = 1;
	pipelineLayoutCI.pPushConstantRanges = &pushConstantRange;


	//std::vector<VkPushConstantRange> pushConstantRanges = {
	//	vks::initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0),
	//	// 加一个geom用
	//     vks::initializers::pushConstantRange(VK_SHADER_STAGE_GEOMETRY_BIT, sizeof(glm::mat4),  sizeof(glm::mat4))
	//};
	//pipelineLayoutCI.pushConstantRangeCount = 2;
	//pipelineLayoutCI.pPushConstantRanges = pushConstantRanges.data();


	VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &pipelineLayout));




	// 新增一套 (天空盒)
	pipelineLayoutCI = vks::initializers::pipelineLayoutCreateInfo(&descriptorSetLayouts.skybox, 1);
	VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &pipelineLayoutSkybox));


	// Descriptor set for scene matrices
	VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.matrices, 1);
	VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));
	//VkWriteDescriptorSet writeDescriptorSet = vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &shaderData.buffer.descriptor);
	//vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
	std::vector<VkWriteDescriptorSet> writeDescriptorSets2 = {
		vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &shaderData.buffer.descriptor),
		vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, &shaderData.buffer.descriptor),
	};
	vkUpdateDescriptorSets(device, writeDescriptorSets2.size(), writeDescriptorSets2.data(), 0, nullptr);

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


	// 新增一个
	allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.customSet2, 1);
	VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSetCustomSet2));

	writeDescriptorSets2 = {
			vks::initializers::writeDescriptorSet(descriptorSetCustomSet2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &shaderDataCustom.buffer.descriptor),
	        vks::initializers::writeDescriptorSet(descriptorSetCustomSet2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, &shaderDataCustom.buffer.descriptor)
	};
	vkUpdateDescriptorSets(device, writeDescriptorSets2.size(), writeDescriptorSets2.data(), 0, nullptr);

	//VkWriteDescriptorSet writeDescriptorSet = 
	//vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);


	// 新增一个 天空盒
	allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.skybox, 1);
	VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSetSkyBox));
	std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
		vks::initializers::writeDescriptorSet(descriptorSetSkyBox, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &shaderDataSkybox.buffer.descriptor),						// Binding 0: Vertex shader uniform buffer
		vks::initializers::writeDescriptorSet(descriptorSetSkyBox, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,	1, &cubemap.descriptor),							// Binding 1: Fragment shader texture sampler
	};
	vkUpdateDescriptorSets(device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);




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
		//rasterizationStateCI.cullMode = material.doubleSided ? VK_CULL_MODE_NONE : VK_CULL_MODE_BACK_BIT;
		rasterizationStateCI.cullMode = VK_CULL_MODE_BACK_BIT;

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &material.pipeline));
	}



	// 加一个天空盒
	/*VkPipelineVertexInputStateCreateInfo emptyInputState = vks::initializers::pipelineVertexInputStateCreateInfo();
	pipelineCI.pVertexInputState = &emptyInputState;*/
	pipelineCI.pVertexInputState = vkglTF::Vertex::getPipelineVertexInputState({
		vkglTF::VertexComponent::Position });
	shaderStages[0] = loadShader(getShadersPath() + "bloom/skybox.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	shaderStages[1] = loadShader(getShadersPath() + "bloom/skybox.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	depthStencilStateCI.depthWriteEnable = VK_FALSE;
	rasterizationStateCI.cullMode = VK_CULL_MODE_FRONT_BIT;
	pipelineCI.renderPass = renderPass;
	pipelineCI.layout = pipelineLayoutSkybox;
	VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.skyBox));



	// 加一个法线调试
	std::array<VkPipelineShaderStageCreateInfo, 3> shaderStages3;
	pipelineCI.pVertexInputState = &vertexInputStateCI;
	shaderStages3[0] = loadShader(getShadersPath() + "new/normal.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	shaderStages3[1] = loadShader(getShadersPath() + "new/normal.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	shaderStages3[2] = loadShader(getShadersPath() + "new/normaldebug.geom.spv", VK_SHADER_STAGE_GEOMETRY_BIT);
	pipelineCI.stageCount = shaderStages3.size();
	pipelineCI.pStages = shaderStages3.data();
	depthStencilStateCI.depthWriteEnable = VK_TRUE;
	rasterizationStateCI.cullMode = VK_CULL_MODE_BACK_BIT;
	pipelineCI.renderPass = renderPass;
	pipelineCI.layout = pipelineLayout;
	VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &glTFScene.normal));


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


		if (displaySkybox) {
			// Skybox
			vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayoutSkybox, 0, 1, &descriptorSetSkyBox, 0, NULL);
			vkCmdBindPipeline(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.skyBox);
			glTFModels.skyBox.draw(drawCmdBuffers[i]);
		}


		// Bind scene matrices descriptor to set 0
		vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 2, 1, &descriptorSetCustomSet2, 0, nullptr);



		// POI: Draw the glTF scene
		glTFScene.draw(drawCmdBuffers[i], pipelineLayout);

		if (displayNormals) {
			glTFScene.draw(drawCmdBuffers[i], pipelineLayout, glTFScene.normal);
		}



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

	VulkanExampleBase::initSwapchain();
	VulkanExampleBase::createCommandPool();
	VulkanExampleBase::setupSwapChain();
	VulkanExampleBase::createCommandBuffers();
	VulkanExampleBase::createSynchronizationPrimitives();
	VulkanExampleBase::setupDepthStencil();
	VulkanExampleBase::setupRenderPass();
	VulkanExampleBase::createPipelineCache();
	VulkanExampleBase::setupFrameBuffer();
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





VULKAN_EXAMPLE_MAIN()
