//@BridgetACasey

#pragma once

#include <string>
#include <unordered_map>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Wizzard
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static WizRef<Shader> Create(const std::string& filePath);
		static WizRef<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);

		virtual const std::string& GetName() const = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const WizRef<Shader>& shader);
		void Add(const WizRef<Shader>& shader);

		WizRef<Shader> Load(const std::string& filePath);
		WizRef<Shader> Load(const std::string& name, const std::string& filePath);

		WizRef<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, WizRef<Shader>> shaders;
	};
}
