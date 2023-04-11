// c-project-cmake.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once



#include "VulkanExampleBase.h"
#include "VulkanglTFScene.h"


class VulkanExample : public VulkanExampleBase
{

public:
	// 私有
	class VulkanglTFScene glTFScene;

	// Shader相关

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


	struct ShaderDataCustom {
		vks::Buffer buffer;
		struct Values {
			float ambient = 0.1f;
			float specularExponent = 32.0f;
			
		} values;
	} shaderDataCustom;


	struct DescriptorSetLayouts {
		VkDescriptorSetLayout matrices;
		VkDescriptorSetLayout textures;
		// 新增一个
		VkDescriptorSetLayout custom;

	} descriptorSetLayouts;


	VkPipelineLayout pipelineLayout;

	// 给CMD使用 vkCmdBindDescriptorSets 
	VkDescriptorSet descriptorSet;
	// 新增
	VkDescriptorSet descriptorSetCustom;






	VulkanExample();



	VkClearColorValue defaultClearColor = { { 0.025f, 0.025f, 0.025f, 1.0f } };

















	void prepare();




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

	// 设置 device features
	virtual void getEnabledFeatures();

	virtual void OnUpdateUIOverlay(vks::UIOverlay* overlay);

private:










protected:



















	// Descriptor set pool
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;



	std::string shaderDir = "glsl";


	// Returns the path to the root of the glsl or hlsl shader directory.
	std::string getShadersPath() const;






};



// TODO: 在此处引用程序需要的其他标头。
