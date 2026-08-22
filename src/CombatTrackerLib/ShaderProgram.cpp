#include "CombatTrackerLib/ShaderProgram.h"

namespace CombatTrackerLib {
	ShaderProgram::ShaderProgram(std::vector<Shader>& shaders) {

		// Create the shader program, attach and link shaders.
		m_ID = glCreateProgram();
		for (const auto& shader : shaders) {
			glAttachShader(m_ID, shader.getShaderID());
		}
		glLinkProgram(m_ID);

		// Check for linker errors.
		GLint isLinked{};
		glGetProgramiv(m_ID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint logLength{};
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &logLength);
			std::string infoLog(static_cast<size_t>(std::max(logLength, 1)), '\0');
			glGetProgramInfoLog(m_ID, logLength, nullptr, infoLog.data());
			std::cerr << "Program link failed: " << infoLog << std::endl;
			glDeleteProgram(m_ID);
			return;
		}

		// Delete the shaders after the program is properly constructed as we don't need them anymore.
		// Also set the linked member variable to true so that the shaders can be properly destructed/moved.
		for (auto& shader : shaders) {
			glDeleteShader(shader.getShaderID());
			shader.setAsLinked();
		}
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& program) noexcept {
		this->m_ID = program.m_ID;
		program.m_ID = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& program) noexcept {

		// Prevent self move.
		if (this == &program) {
			return *this;
		}

		// Free resources from the object that is being moved to.
		glDeleteProgram(m_ID);
		this->m_ID = program.m_ID;
		program.m_ID = 0;
		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ID);
	}

	void ShaderProgram::use() const {
		glUseProgram(m_ID);
	}

	void ShaderProgram::unbind() const {
		glUseProgram(0);
	}
}
