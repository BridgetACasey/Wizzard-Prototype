//@BridgetACasey

#include "wzpch.h"

#include "Shader.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLShader.h"

namespace Wizzard
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSource, fragmentSource);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}