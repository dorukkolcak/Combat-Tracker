#include "Shader.h"
#include "./Utilities/IOUtils.h"

#include "glad/glad.h"

#include <algorithm>
#include <iostream>

namespace CombatTrackerLib {
	Shader::Shader(const std::filesystem::path& shaderPath, Type type) : m_shaderType{type} 
	{

		std::string shaderCodeTemp{ readFileToString(shaderPath) };
		if (shaderCodeTemp.empty()) {
			std::cerr << "Failed to read shader file: " << shaderPath << '\n';
			return;
		}

		const char* shaderCode{ shaderCodeTemp.c_str() };
		switch (type) {
		case Type::VERTEX:
			m_shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case Type::FRAGMENT:
			m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			std::cerr << "No corresponding shader type: \n";
			return;
		}

		glShaderSource(m_shaderID, 1, &shaderCode, NULL);
		glCompileShader(m_shaderID);

		// Error handling.
		int isCompiled{};
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			int logLength = 0;
			glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logLength);
			std::string infoLog(static_cast<size_t>(std::max(logLength, 1)), '\0');
			glGetShaderInfoLog(m_shaderID, logLength, nullptr, infoLog.data());
			std::cerr << "Shader compilation failed: " << infoLog << std::endl;
			glDeleteShader(m_shaderID);
			return;
		}
	}
	Shader::Shader(Shader&& shader) noexcept {
		this->m_shaderID = shader.m_shaderID;
		this->m_shaderType = shader.m_shaderType;
		shader.m_shaderID = 0;
		shader.m_shaderType = Type::INVALID;
	}
	Shader& Shader::operator=(Shader&& shader) noexcept {
		if (this == &shader) {
			return *this;
		}
		glDeleteShader(m_shaderID); // This might actually be dangerous to do, considering the fact a shader program already calls this for each shader it owns.
		this->m_shaderID = shader.m_shaderID;
		this->m_shaderType = shader.m_shaderType;
		shader.m_shaderID = 0;
		shader.m_shaderType = Type::INVALID;
		return *this;
	}
	unsigned int Shader::getShaderID() const {
		return m_shaderID;
	}
}