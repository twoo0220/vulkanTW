#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "CommonType.h"

class ImGuiRenderer
{
public:
	ImGuiRenderer(GLFWwindow* window);

	void initialize();
	void update();

private:
	GLFWwindow* mGLFWWindow = nullptr;
};
