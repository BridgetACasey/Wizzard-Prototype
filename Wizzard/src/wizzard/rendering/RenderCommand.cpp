//@BridgetACasey

#include "wzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Wizzard
{
	RendererAPI* RenderCommand::rendererAPI = new OpenGLRendererAPI;
}