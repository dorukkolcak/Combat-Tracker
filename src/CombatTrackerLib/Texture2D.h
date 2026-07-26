#pragma once

#include <glad/glad.h>
#include <string>

namespace CombatTrackerLib {
    class Texture2D {
    public:
        Texture2D() = default;
        explicit Texture2D(std::string path);

        // Should be non-copiable to prevent double deletions.
        Texture2D(const Texture2D& texture) = delete;
        Texture2D& operator=(const Texture2D& texture) = delete;

        // Explicitly define the move constructor and move assignment operator (rule of five).
        Texture2D(Texture2D&& texture) noexcept;
        Texture2D& operator=(Texture2D&& texture) noexcept;

        ~Texture2D();

        void bindTexture() const;
        void unbindTexture() const;
        bool isTextureValid() const;

        GLuint getTextureID() const;
        int getTextureWidth() const;
        int getTextureHeight() const;

    private:
        std::string m_filePath{};
        GLuint m_textureID{};
        int m_textureWidth{};
        int m_textureHeight{};
        int m_numberOfColorChannels{};

    };
}
