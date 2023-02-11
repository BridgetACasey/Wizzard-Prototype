//@BridgetACasey

#pragma once

#include "rendering/Texture.h"

namespace Wizzard
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return width; }
		uint32_t GetHeight() const override { return height; }

		void Bind(uint32_t slot = 0) const override;

	private:
		std::string path;
		uint32_t width, height;
		uint32_t rendererID;
	};
}
