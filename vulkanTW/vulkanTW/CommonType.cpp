#include "CommonType.h"

VkInstance g_Instance = nullptr;
ImGui_ImplVulkanH_Window g_ImGuiVulkanWindowData{};

VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
VkDevice g_Device = VK_NULL_HANDLE;
VkQueue g_GraphicsQueue = VK_NULL_HANDLE;
VkQueue g_PresentQueue = VK_NULL_HANDLE;