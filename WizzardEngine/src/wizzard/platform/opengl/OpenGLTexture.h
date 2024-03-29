//@BridgetACasey

#pragma once

#include "rendering/Texture.h"

#include <glad/glad.h>

namespace Wizzard
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return width; }
		uint32_t GetHeight() const override { return height; }

		void Bind(uint32_t slot = 0) const override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual uint32_t GetRendererID() const override { return rendererID; }

		virtual bool operator==(const Texture& other) const override
		{
			return rendererID == ((OpenGLTexture2D&)other).rendererID;
		}

	private:
		std::string path;
		uint32_t width, height;
		uint32_t rendererID;
		GLenum internalFormat, dataFormat;
	};
}