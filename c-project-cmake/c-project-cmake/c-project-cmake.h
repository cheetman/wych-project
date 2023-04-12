// c-project-cmake.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once



#include "VulkanExampleBase.h"
//#include "VulkanglTFScene.h"
#include "base/VulkanglTFModel.h"


class VulkanExample : public VulkanExampleBase
{

public:
	// 私有
	VulkanglTFScene glTFScene;

	struct {
		//vkglTF::Model ufo;
		//vkglTF::Model ufoGlow;
		vkglTF::Model skyBox;
	} glTFModels;
	// 天空盒材质相关
	vks::TextureCubeMap cubemap;


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


	struct UBO {
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
	};
	struct SkyBox {
		vks::Buffer buffer;
		struct UBO {
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;
		} ubo;
	} shaderDataSkybox;




	struct DescriptorSetLayouts {
		VkDescriptorSetLayout matrices;
		VkDescriptorSetLayout textures;
		// 新增一个
		VkDescriptorSetLayout custom;
		VkDescriptorSetLayout skybox;

	} descriptorSetLayouts;


	VkPipelineLayout pipelineLayout;
	VkPipelineLayout pipelineLayoutSkybox;

	// 给CMD使用 vkCmdBindDescriptorSets 
	VkDescriptorSet descriptorSet;
	// 新增
	VkDescriptorSet descriptorSetCustom;
	// 新增天空盒
	VkDescriptorSet descriptorSetSkyBox;


	// 材质的pipeline存在自己的结构体中了
	struct {
		/*VkPipeline blurVert;
		VkPipeline blurHorz;
		VkPipeline glowPass;
		VkPipeline phongPass;*/
		
		VkPipeline skyBox;
	} pipelines;




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




	// Descriptor set 池，这个其实都在子类中使用了
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;



};



// TODO: 在此处引用程序需要的其他标头。
