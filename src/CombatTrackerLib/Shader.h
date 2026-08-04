#pragma once

#include <string>
#include <filesystem>
#include <glad/glad.h>

namespace CombatTrackerLib {
	class Shader {
	public:
		enum class Type {
			INVALID,
			VERTEX,
			GEOMETRY,
			FRAGMENT
		};

		Shader() = default;
		Shader(const std::filesystem::path& shaderPath, Type type);

		// Shouldn't be copiable as a copy might result in double delete, double call to glDeleteShader(shaderID).
		Shader(const Shader& shader) = delete;
		Shader& operator=(const Shader& shader) = delete;

		// Can be movable, the moved from object should have an id of zero.
		Shader(Shader&& shader) noexcept;
		Shader& operator=(Shader&& shader) noexcept;

		~Shader();

		unsigned int getShaderID() const;
		void setAsLinked();
	private:
		unsigned int m_shaderID{};
		bool m_isLinkedToProgram{};
	};
}