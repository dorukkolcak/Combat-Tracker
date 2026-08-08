#include "CombatTrackerLib/Texture2D.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace CombatTrackerLib {
	Texture2D::Texture2D(
		std::string_view texturePath,
		unsigned int numberOfDesiredChannels,
		bool isOriginAtTop,
		int sAxisWrap,
		int tAxisWrap,
		int magnifyingFilter,
		int minifyingMipmapFilter
	) : Texture2D(
		texturePath,
		numberOfDesiredChannels,
		isOriginAtTop,
		sAxisWrap,
		tAxisWrap,
		magnifyingFilter,
		minifyingMipmapFilter,
		GL_TEXTURE0)
	{}

	Texture2D::Texture2D(
		std::string_view texturePath,
		unsigned int numberOfDesiredChannels,
		bool isOriginAtTop,
		int sAxisWrap,
		int tAxisWrap,
		int magnifyingFilter,
		int minifyingMipmapFilter,
		unsigned int unit
	) : m_textureUnit{ unit } {

		/*
		Set the internal and CPU format based on the number of desired channels of the loaded texture.
		4: PNG, TIFF, WebP => GL_RGBA8 - GL_RGBA
		3: JPEG, PNG => GL_RGB8 - GL_RGB
		1: Greyscale, heightmaps => GL_R8 - GL_RED
		*/
		int internalFormat;
		unsigned int cpuFormat;
		if (numberOfDesiredChannels == 4) {
			internalFormat = GL_RGBA8;
			cpuFormat = GL_RGBA;
		}
		else if (numberOfDesiredChannels == 3) {
			internalFormat = GL_RGB8;
			cpuFormat = GL_RGB;
		}
		else if (numberOfDesiredChannels == 1) {
			internalFormat = GL_R8;
			cpuFormat = GL_RED;
		}
		// This is not really extensible right now, I have to handle each image format case specifically.
		else {
			return;
		}

		// Load pixel data into CPU memory using stb's image loading API.
		stbi_set_flip_vertically_on_load(isOriginAtTop);
		auto* localBuffer{ stbi_load(texturePath.data(), &m_textureWidth, &m_textureHeight, &m_numberOfColorChannels, numberOfDesiredChannels) };
		if (localBuffer == nullptr) {
			return;
		}

		// Generate the texture id and set its parameters.
		glGenTextures(1, &m_textureID);
		use();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sAxisWrap); // Parameter for what to do if texture x coordinates fall outside the range [0, 1].
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tAxisWrap); // Parameter for what to do if texture y coordinates fall outside the range [0, 1].
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnifyingFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minifyingMipmapFilter);

		// Generate the actual texture and its mipmaps.
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_textureWidth, m_textureHeight, 0, cpuFormat, GL_UNSIGNED_BYTE, localBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Cleanup.
		unbind();
		stbi_image_free(localBuffer);
	}
	Texture2D::Texture2D(Texture2D&& texture) noexcept {
		this->m_numberOfColorChannels = texture.m_numberOfColorChannels;
		this->m_textureUnit = texture.m_textureUnit;
		this->m_textureHeight = texture.m_textureHeight;
		this->m_textureWidth = texture.m_textureWidth;
		this->m_textureID = texture.m_textureID;

		// Leave the moved from texture in a valid state.
		texture.m_numberOfColorChannels = 0;
		texture.m_textureUnit = GL_TEXTURE0;
		texture.m_textureHeight = 0;
		texture.m_textureWidth = 0;
		texture.m_textureID = 0;
	}
	Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept {

		// Prevent self move.
		if (this == &texture) {
			return *this;
		}

		// Delete the previously owned texture of this object.
		glDeleteTextures(1, &(this->m_textureID));

		this->m_numberOfColorChannels = texture.m_numberOfColorChannels;
		this->m_textureUnit = texture.m_textureUnit;
		this->m_textureHeight = texture.m_textureHeight;
		this->m_textureWidth = texture.m_textureWidth;
		this->m_textureID = texture.m_textureID;

		// Leave the moved from texture in a valid state.
		texture.m_numberOfColorChannels = 0;
		texture.m_textureUnit = GL_TEXTURE0;
		texture.m_textureHeight = 0;
		texture.m_textureWidth = 0;
		texture.m_textureID = 0;

		return *this;
	}
	Texture2D::~Texture2D() {
		glDeleteTextures(1, &m_textureID);
	}
	void Texture2D::use() const {
		glActiveTexture(m_textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
	void Texture2D::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::setTextureUnit(unsigned int unit) {
		m_textureUnit = unit;
	}
	int Texture2D::getWidth() const {
		return m_textureWidth;
	}
	int Texture2D::getHeight() const {
		return m_textureHeight;
	}
	unsigned int Texture2D::getID() const {
		return m_textureID;
	}
}