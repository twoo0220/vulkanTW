#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "CommonType.h"

class Shader
{
public:
	Shader() = default;

	std::vector<char> readShaderFile(const std::string& fileName);
	std::vector<char> getBuffer() const;

private:
	std::vector<char> mBuffer;
};
