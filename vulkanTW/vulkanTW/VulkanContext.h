#pragma once

#include <vector>
#include "vulkan/vulkan.h"

class VulkanContext
{
public:
	VulkanContext(const std::vector<const char*>& requiredInstanceExtensions, bool useSwapChain);
	~VulkanContext();

	void cleanup();
	void createQueues();
	void createInstance(std::vector<const char*> instanceExtensions);

private:
	VkInstance mInstance = VK_NULL_HANDLE;


#ifdef NDEBUG
	bool mEnableValidationLayers = false;
#else
	bool mEnableValidationLayers = true;
#endif

};
