#include "ImGuiRenderer.h"

static void check_vk_result(VkResult err)
{
	if (err == 0) return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0) abort();
}

ImGuiRenderer::ImGuiRenderer(GLFWwindow* window)
	: mGLFWWindow(window)
{
}

void ImGuiRenderer::initialize()
{
	//ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

	//// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	////ImGui::StyleColorsLight();

	//// Setup Platform/Renderer backends
	//ImGui_ImplGlfw_InitForVulkan(mGLFWWindow, true);
	//ImGui_ImplVulkan_InitInfo init_info{};
	//init_info.Instance = g_Instance;
	//init_info.PhysicalDevice = g_PhysicalDevice;
	//init_info.Device = g_Device;
	//init_info.QueueFamily = g_QueueFamily;
	//init_info.Queue = g_Queue;
	//init_info.PipelineCache = g_PipelineCache;
	//init_info.DescriptorPool = g_DescriptorPool;
	//init_info.Subpass = 0;
	//init_info.MinImageCount = g_MinImageCount;
	//init_info.ImageCount = wd->ImageCount;
	//init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	//init_info.Allocator = g_Allocator;
	//init_info.CheckVkResultFn = check_vk_result;
	//ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
	
}

void ImGuiRenderer::update()
{

}

