//@BridgetACasey

#include "wzpch.h"

#include "Shader.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLShader.h"

namespace Wizzard
{
	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(filePath);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		WIZ_ASSERT(!Exists(name), "Shader already exists!");
		shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		WIZ_ASSERT(Exists(name), "Shader not found!");
		return shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return shaders.find(name) != shaders.end();
	}
}