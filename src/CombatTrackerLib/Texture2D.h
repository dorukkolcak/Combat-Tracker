#pragma once

#include <string_view>

namespace CombatTrackerLib {
	class Texture2D {
	public:

		Texture2D() = default;

		// TODO: Ideally we would like to limit the texture parameters to a set of valid options instead of any arbitrary value.
		Texture2D(
			std::string_view texturePath,
			unsigned int numberOfDesiredChannels,
			bool isOriginAtTop,
			int sAxisWrap,
			int tAxisWrap,
			int magnifyingFilter,
			int minifyingMipmapFilter
		);
		Texture2D(
			std::string_view texturePath,
			unsigned int numberOfDesiredChannels,
			bool isOriginAtTop,
			int sAxisWrap,
			int tAxisWrap,
			int magnifyingFilter,
			int minifyingMipmapFilter,
			unsigned int unit
		);

		// Should be non-copiable because of double delete cases.
		Texture2D(const Texture2D& texture) = delete;
		Texture2D& operator=(const Texture2D& texture) = delete;

		Texture2D(Texture2D&& texture) noexcept;
		Texture2D& operator=(Texture2D&& texture) noexcept;

		~Texture2D();

		void use() const;
		void unbind() const;

		void setTextureUnit(unsigned int unit);

		int getWidth() const;
		int getHeight() const;
		unsigned int getID() const;
	private:
		unsigned int m_textureID{};
		unsigned int m_textureUnit{};
		int m_textureWidth{};
		int m_textureHeight{};
		int m_numberOfColorChannels{};
	};
}