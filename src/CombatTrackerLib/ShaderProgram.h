#pragma once

#include "CombatTrackerLib/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <vector>

namespace CombatTrackerLib {
	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		explicit ShaderProgram(std::vector<Shader>& shaders);

		ShaderProgram(const ShaderProgram& program) = delete;
		ShaderProgram& operator=(const ShaderProgram& program) = delete;

		ShaderProgram(ShaderProgram&& program) noexcept;
		ShaderProgram& operator=(ShaderProgram&& program) noexcept;

		~ShaderProgram();

		void use() const;
		void unbind() const;

		/*
		This function is designed to be used with any uniform type.
		It abstracts away every single glUniform function through the use of templates and type traits.
		We have to do a long chain of if statements because OpenGL itself has a seperate function definition for each uniform type.
		*/
		template <typename T> void setUniform(std::string_view uniformName, T value) const;

	private:
		unsigned int m_ID{};
	};

	template <typename T> void ShaderProgram::setUniform(std::string_view uniformName, T value) const {
		const auto uniformLocation{ glGetUniformLocation(m_ID, uniformName.data()) };
		if constexpr (std::is_same_v<T, bool>) {
			glUniform1i(uniformLocation, value);
		}
		else if constexpr (std::is_same_v<T, int>) {
			glUniform1i(uniformLocation, value);
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			glUniform1ui(uniformLocation, value);
		}
		else if constexpr (std::is_same_v<T, float>) {
			glUniform1f(uniformLocation, value);
		}
		else if constexpr (std::is_same_v<T, glm::vec2>) {
			glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::vec3>) {
			glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::vec4>) {
			glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::ivec2>) {
			glUniform2iv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::ivec3>) {
			glUniform3iv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::ivec4>) {
			glUniform4iv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::uvec2>) {
			glUniform2uiv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::uvec3>) {
			glUniform3uiv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::uvec4>) {
			glUniform4uiv(uniformLocation, 1, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat2>) {
			glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat3>) {
			glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat4>) {
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat2x3>) {
			glUniformMatrix2x3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat3x2>) {
			glUniformMatrix3x2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat2x4>) {
			glUniformMatrix2x4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat4x2>) {
			glUniformMatrix4x2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat3x4>) {
			glUniformMatrix3x4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else if constexpr (std::is_same_v<T, glm::mat4x3>) {
			glUniformMatrix4x3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
		else {
			std::cerr << "Invalid uniform type.\n";
		}
	}
}