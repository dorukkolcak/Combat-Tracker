#pragma once

#include <filesystem>

namespace CombatTrackerLib {
	class Shader {
	public:

		// Should be public to allow for proper object instantiation.
		enum class Type {
			INVALID,
			VERTEX,
			FRAGMENT,
		};

		Shader(const std::filesystem::path& shaderPath, Type type);

		Shader(const Shader& shader) = delete;
		Shader& operator=(const Shader& shader) = delete;

		Shader(Shader&& shader) noexcept;
		Shader& operator=(Shader&& shader) noexcept;

		unsigned int getShaderID() const;

	private:
		unsigned int m_shaderID{};
		Type m_shaderType{};
	};
}