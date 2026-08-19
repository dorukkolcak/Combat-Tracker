#pragma once

#include <filesystem>

namespace CombatTrackerLib {
	class Texture2D {
	public:

		Texture2D() = delete;

		// TODO: Ideally we would like to limit the texture parameters to a set of valid options instead of any arbitrary value.
		Texture2D(
			const std::filesystem::path& texturePath,
			int numberOfDesiredChannels,
			bool isOriginAtTop,
			int sAxisWrap,
			int tAxisWrap,
			int magnifyingFilter,
			int minifyingMipmapFilter
		);
		Texture2D(
			const std::filesystem::path& texturePath,
			int numberOfDesiredChannels,
			bool isOriginAtTop,
			int sAxisWrap,
			int tAxisWrap,
			int magnifyingFilter,
			int minifyingMipmapFilter,
			unsigned int glUnit,
			int integerUnit
		);

		// Should be non-copiable because of double delete cases.
		Texture2D(const Texture2D& texture) = delete;
		Texture2D& operator=(const Texture2D& texture) = delete;

		Texture2D(Texture2D&& texture) noexcept;
		Texture2D& operator=(Texture2D&& texture) noexcept;

		~Texture2D();

		void use() const;
		void unbind() const;

		void setUnit(unsigned int glUnit, int integerUnit);

		int getIntUnit() const;
		int getWidth() const;
		int getHeight() const;
		unsigned int getID() const;
	private:
		unsigned int m_ID{};
		unsigned int m_glUnit{};
		int m_integerUnit{};
		int m_width{};
		int m_height{};
		int m_numberOfColorChannels{};
	};
}