#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "CommonType.h"

class ImGuiRenderer
{
public:
	ImGuiRenderer(GLFWwindow* window);

	void initialize();
	void update();

private:
	GLFWwindow* mGLFWWindow = nullptr;
	ImGui_ImplVulkanH_Window* mWD;
};
