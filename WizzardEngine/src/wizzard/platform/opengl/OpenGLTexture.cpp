//@BridgetACasey

#include "wzpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

#include "glad/glad.h"

namespace Wizzard
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : width(width), height(height)
	{
		WIZ_PROFILE_FUNCTION();

		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFormat, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path)
	{
		WIZ_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		WIZ_ASSERT(data, "Failed to load image!");
		width = width;
		height = height;

		GLenum internalFmt = 0, dataFmt = 0;
		if (channels == 4)
		{
			internalFmt = GL_RGBA8;
			dataFmt = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFmt = GL_RGB8;
			dataFmt = GL_RGB;
		}

		internalFormat = internalFmt;
		dataFormat = dataFmt;

		WIZ_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
		glTextureStorage2D(rendererID, 1, internalFmt, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFmt, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		WIZ_PROFILE_FUNCTION();

		glDeleteTextures(1, &rendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		WIZ_PROFILE_FUNCTION();

		glBindTextureUnit(slot, rendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		WIZ_PROFILE_FUNCTION();

		uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
		WIZ_ASSERT(size == width * height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
	}
}
