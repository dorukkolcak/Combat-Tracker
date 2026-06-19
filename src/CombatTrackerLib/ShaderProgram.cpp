#include "ShaderProgram.h"

#include "glad/glad.h"

#include <iostream>

// Constructed with std::move as the ShaderProgram should have "ownership" over the set of IDs given in the vector.
ShaderProgram::ShaderProgram(std::vector<unsigned int> shaderIDs) :
	shaders(std::move(shaderIDs))
{
	programID = glCreateProgram();
	for (const auto& shaderID : shaders) {
		if (shaderID == 0) {
			std::cerr << "Cannot link program: invalid shader ID.\n";
			glDeleteProgram(programID);
			return;
		}
		glAttachShader(programID, shaderID);
	}
	glLinkProgram(programID);

	// Error handling.
	int isLinked{};
	glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		int logLength{};
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		std::string infoLog(static_cast<size_t>(std::max(logLength, 1)), '\0');
		glGetProgramInfoLog(programID, logLength, nullptr, infoLog.data());
		std::cerr << "Program link failed: " << infoLog << std::endl;
		glDeleteProgram(programID);
		return;
	}

	// Delete the shaders after the program is constructed as we don't need them anymore.
	for (const auto& shaderID : shaders) {
		glDeleteShader(shaderID);
	}
}
ShaderProgram::ShaderProgram() {}
void ShaderProgram::activate() {
	glUseProgram(programID);
}
void ShaderProgram::deleteProgram() {
	glDeleteProgram(programID);
}
void ShaderProgram::setMatrix4(const std::string& uniformName, glm::mat4 transformation) {
	glUniformMatrix4fv(glGetUniformLocation(programID, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(transformation));
}
void ShaderProgram::setVec4(const std::string& uniformName, glm::vec4 vector) {
	glUniform4f(glGetUniformLocation(programID, uniformName.c_str()), vector.x, vector.y, vector.z, vector.w);
}
void ShaderProgram::setFloat(const std::string& uniformName, float val) {
	glUniform1f(glGetUniformLocation(programID, uniformName.c_str()), val);
}
unsigned int ShaderProgram::getProgramID() const {
	return programID;
}