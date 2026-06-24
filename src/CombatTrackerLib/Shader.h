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
		unsigned int getShaderID() const;

	private:
		unsigned int m_shaderID{};
	};
}