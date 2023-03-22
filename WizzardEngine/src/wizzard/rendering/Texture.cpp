//@BridgetACasey

#include "wzpch.h"

#include "Texture.h"
#include "Renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace Wizzard
{
	WizRef<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return WizRef<OpenGLTexture2D>::CreateRef(width, height);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	WizRef<Texture2D> Texture2D::Create(const std::string& path)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return WizRef<OpenGLTexture2D>::CreateRef(path);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}