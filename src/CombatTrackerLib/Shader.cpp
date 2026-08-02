#include "Shader.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

namespace CombatTrackerLib {
	Shader::Shader(const std::filesystem::path& shaderPath, Shader::Type type) : m_isLinkedToProgram{ false } {

		// Read the shader file.
		auto shaderCode{ readShaderCode(shaderPath) };
		if (shaderCode.empty()) {
			std::cerr << "Failed to read shader file: " << shaderPath << '\n';
			return;
		}

		// Create and compile the shader. 
		switch (type) {
		case Type::VERTEX:
			m_shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case Type::GEOMETRY:
			m_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case Type::FRAGMENT:
			m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			std::cerr << "No corresponding shader type: \n";
			return;
		}
		auto shaderCodePointer{ shaderCode.c_str() };
		glShaderSource(m_shaderID, 1, &shaderCodePointer, nullptr);
		glCompileShader(m_shaderID);

		// Error handling: Check whether the shader is actually compiled.
		GLint isCompiled{};
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint logLength{};
			glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logLength);

			// Create a string with logLength characters. Basically an empty char buffer.
			std::string infoLog(static_cast<size_t>(std::max(logLength, 1)), '\0');
			glGetShaderInfoLog(m_shaderID, logLength, nullptr, infoLog.data());
			std::cerr << "Shader compilation failed: " << infoLog << std::endl;
			glDeleteShader(m_shaderID);
		}
	}
	Shader::Shader(Shader&& shader) noexcept {
		this->m_shaderID = shader.m_shaderID;
		this->m_isLinkedToProgram = shader.m_isLinkedToProgram;

		// Set the moved from object to a default state.
		shader.m_shaderID = 0;
		shader.m_isLinkedToProgram = false;
	}
	Shader& Shader::operator=(Shader&& shader) noexcept {

		// Prevent self move.
		if (this == &shader) {
			return *this;
		}

		// Free resources from the object that is being moved to.
		if (!m_isLinkedToProgram) {
			glDeleteShader(m_shaderID);
		}
		this->m_shaderID = shader.m_shaderID;
		this->m_isLinkedToProgram = shader.m_isLinkedToProgram;
		shader.m_shaderID = 0;
		shader.m_isLinkedToProgram = false;
		return *this;
	}
	Shader::~Shader() {

		// A linked shader shoudn't call glDeleteShader from its destructor, 
		// as its shader program will have already called this.
		if (!m_isLinkedToProgram) {
			glDeleteShader(m_shaderID);
		}
	}
	unsigned int Shader::getShaderID() const {
		return m_shaderID;
	}
	void Shader::setAsLinked() {
		m_isLinkedToProgram = true;
	}
	std::string Shader::readShaderCode(const std::filesystem::path& filePath) {

		std::ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << " Can't open file: " << filePath.string() << "\n";
			return "";
		}

		std::ostringstream fileStream;
		fileStream << file.rdbuf();
		return fileStream.str();
	}
}