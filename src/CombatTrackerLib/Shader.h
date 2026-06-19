#pragma once

#include <filesystem>

namespace CombatTrackerLib {
	enum class ShaderType {
		VERTEX,
		FRAGMENT
	};
	class Shader {
	public:
		Shader(const std::filesystem::path& shaderPath, ShaderType type);
		unsigned int getShaderID() const;

	private:
		unsigned int shaderID{};
	};
}