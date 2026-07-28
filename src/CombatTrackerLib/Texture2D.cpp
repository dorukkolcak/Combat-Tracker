#include "stb_image.h"

#include "Texture2D.h"

namespace CombatTrackerLib {

    Texture2D::Texture2D(std::string path) : 
        m_filePath{std::move(path)},
        m_textureID{}, 
        m_textureWidth{}, 
        m_textureHeight{},
        m_numberOfColorChannels{}
    {

        // Load pixel data into CPU memory using stb's image loading API.
        auto* localBuffer = stbi_load(m_filePath.c_str(), &m_textureWidth, &m_textureHeight, &m_numberOfColorChannels, STBI_rgb_alpha);
        if (localBuffer == nullptr) {
            return;
        }

        // Generate and bind the texture object.
        glGenTextures(1, &m_textureID);
        bindTexture();

        // Set texture parameters. 
        // These might need to be modifiable by the caller at some point so consider parametrizing them.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload CPU pixel array to GPU storage and generate the mipmap.
        // Again the format might need to be parametrizable (currently RGBA which should be fine) at some point.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glGenerateMipmap(GL_TEXTURE_2D); // Probably won't be needed but no reason not to have it I guess.

        // Clean up local CPU buffer and unbind.
        stbi_image_free(localBuffer);
        unbindTexture();
    }
    Texture2D::Texture2D(Texture2D&& texture) noexcept {

        this->m_filePath = std::move(texture.m_filePath);
        this->m_numberOfColorChannels = texture.m_numberOfColorChannels;
        this->m_textureHeight = texture.m_textureHeight;
        this->m_textureWidth = texture.m_textureWidth;
        this->m_textureID = texture.m_textureID;

        // Leave the moved from object in a valid-default state.
        texture.m_filePath = "";
        texture.m_numberOfColorChannels = 0;
        texture.m_textureHeight = 0;
        texture.m_textureWidth = 0;
        texture.m_textureID = 0;
    }
    Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept {

        // Prevent self move.
        if (&texture == this) {
            return *this;
        }

        // Release the previously owned resource before moving.
        glDeleteTextures(1, &this->m_textureID);

        this->m_filePath = std::move(texture.m_filePath);
        this->m_numberOfColorChannels = texture.m_numberOfColorChannels;
        this->m_textureHeight = texture.m_textureHeight;
        this->m_textureWidth = texture.m_textureWidth;
        this->m_textureID = texture.m_textureID;

        texture.m_filePath = "";
        texture.m_numberOfColorChannels = 0;
        texture.m_textureHeight = 0;
        texture.m_textureWidth = 0;
        texture.m_textureID = 0;

        return *this;
    }
    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_textureID);
    }
    void Texture2D::bindTexture() const {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
    void Texture2D::unbindTexture() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    GLuint Texture2D::getTextureID() const {
        return m_textureID;
    }
    int Texture2D::getTextureWidth() const {
        return m_textureWidth;
    }
    int Texture2D::getTextureHeight() const {
        return m_textureHeight;
    }
}