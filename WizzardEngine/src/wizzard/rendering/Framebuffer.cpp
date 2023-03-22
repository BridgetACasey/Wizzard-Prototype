//@BridgetACasey

#include "wzpch.h"

#include "Framebuffer.h"

#include "wizzard/rendering/Renderer.h"

#include "wizzard/platform/opengl/OpenGLFramebuffer.h"

namespace Wizzard
{
	WizRef<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return WizRef<OpenGLFramebuffer>::CreateRef(spec);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}