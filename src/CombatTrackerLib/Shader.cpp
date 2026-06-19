#include "Shader.h"
#include "Utils.h"

#include "glad/glad.h"

#include <iostream>
#include <fstream>

namespace CombatTrackerLib {
	Shader::Shader(const std::filesystem::path& shaderPath, ShaderType type) {

		std::string shaderCodeTemp{ readFile(shaderPath) };
		if (shaderCodeTemp.empty()) {
			std::cerr << "Failed to read shader file: " << shaderPath << '\n';
			return;
		}

		const char* shaderCode{ shaderCodeTemp.c_str() };
		switch (type) {
		case ShaderType::VERTEX:
			shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::FRAGMENT:
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			std::cerr << "No corresponding shader type: \n";
			return;
		}

		glShaderSource(shaderID, 1, &shaderCode, NULL);
		glCompileShader(shaderID);

		// Error handling.
		int isCompiled{};
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			int logLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
			std::string infoLog(static_cast<size_t>(std::max(logLength, 1)), '\0');
			glGetShaderInfoLog(shaderID, logLength, nullptr, infoLog.data());
			std::cerr << "Shader compilation failed: " << infoLog << std::endl;
			glDeleteShader(shaderID);
			return;
		}
	}
	unsigned int Shader::getShaderID() const {
		return shaderID;
	}
}