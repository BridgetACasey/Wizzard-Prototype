//@BridgetACasey

#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Wizzard
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t rendererID;
	};
}