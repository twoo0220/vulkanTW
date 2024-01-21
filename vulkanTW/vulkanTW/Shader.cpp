#include "Shader.h"

std::vector<char> Shader::readShaderFile(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Failed to open ShaderFile. return void vector<char>" << std::endl;
		return mBuffer;
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	mBuffer.resize(fileSize);

	file.seekg(0);
	file.read(mBuffer.data(), fileSize);
	file.close();

	return mBuffer;
}

std::vector<char> Shader::getBuffer() const
{
	return mBuffer;
}
