#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

class ShaderProgram {
public:
	explicit ShaderProgram(std::vector<unsigned int> shaders);

	ShaderProgram(const ShaderProgram& program) = delete;
	ShaderProgram& operator=(const ShaderProgram& program) = delete;

	ShaderProgram(ShaderProgram&& program) noexcept;
	ShaderProgram& operator=(ShaderProgram&& program) noexcept;

	~ShaderProgram() = default;

	// These functions mostly serve as simpler abstractions over the OpenGL API.
	void activate();
	void deleteProgram();
	void setMatrix4(const std::string& uniformName, glm::mat4 transformation);
	void setVec4(const std::string& uniformName, glm::vec4 vector);
	void setFloat(const std::string& uniformName, float val);

	unsigned int getProgramID() const;

private:
	std::vector<unsigned int> m_shaders{};
	unsigned int m_programID{};
};