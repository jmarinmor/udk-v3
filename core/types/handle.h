#pragma once

namespace rav
{
	struct handle_t
	{
		enum type_t {UNKNOWN, HANDLE_WINDOWS, HANDLE_OPENGL, HANDLE_VULKAN, HANDLE_DIRECTX,

			VULKAN, VULKAN_VkSurfaceKHR, VULKAN_VkInstance, VULKAN_VkPhysicalDevice, VULKAN_VkDevice, 
			VULKAN_VkRenderPass, VULKAN_VkSwapchainKHR, VULKAN_VkFramebuffer, VULKAN_VkImage, VULKAN_VkBuffer, VULKAN_VkImageView,
			VULKAN_VkPipeline, VULKAN_VkPipelineLayout, VULKAN_VkShaderModule, VULKAN_VkQueue, VULKAN_VkCommandPool,
			VULKAN_VkFence, VULKAN_VkSemaphore, VULKAN_VkCommandBuffer, 
			VULKAN_VkFormat, VULKAN_VkColorSpaceKHR, VULKAN_VkSampler, VULKAN_VkDeviceMemory,
			VULKAN_VkDescriptorSetLayout,VULKAN_VkDescriptorSet,VULKAN_VkDescriptorPool,
			
			GLFW_GLFWwindow};

		inline handle_t() {}
		inline handle_t(void* h, type_t t) {handle = h; type = t;}
		inline bool is_null() const {return handle == nullptr;}

		void* handle;
		type_t type;
	};	
}