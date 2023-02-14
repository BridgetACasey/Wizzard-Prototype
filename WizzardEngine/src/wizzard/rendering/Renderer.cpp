//@BridgetACasey

#include "wzpch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "platform/opengl/OpenGLShader.h"

namespace Wizzard
{
	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		WIZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		WIZ_PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		WIZ_PROFILE_FUNCTION();

		sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		WIZ_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		WIZ_PROFILE_FUNCTION();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}