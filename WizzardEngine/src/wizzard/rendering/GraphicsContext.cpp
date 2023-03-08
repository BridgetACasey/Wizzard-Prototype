//@BridgetACasey

#include "wzpch.h"

#include "GraphicsContext.h"
#include "RendererAPI.h"

#include "common/Application.h"
#include "platform/opengl/OpenGLContext.h"

namespace Wizzard
{
	//Currently, the only implemented graphical API is OpenGL and all other options will return nullptr
	GraphicsContext* GraphicsContext::Create()
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
				return nullptr;
			case RendererAPI::API::OpenGL:
					return new OpenGLContext(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
			case RendererAPI::API::Vulkan:
			case RendererAPI::API::DX11:
			case RendererAPI::API::DX12:
			default:
				return nullptr;
		}
	}
}