//@BridgetACasey

#include "wzpch.h"

#include "Texture.h"
#include "Renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace Wizzard
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}