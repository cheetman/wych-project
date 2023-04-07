

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


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#define TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#endif
#include "tiny_gltf.h"


#include "VulkanglTFScene.h"


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
					node->countPosNor = accessor.count;
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
				node->countIndex = accessor.count;
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