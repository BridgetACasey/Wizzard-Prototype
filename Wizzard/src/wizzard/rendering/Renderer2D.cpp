//@BridgetACasey

#include "wzpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/ext/matrix_transform.hpp"
#include "platform/opengl/OpenGLShader.h"

namespace Wizzard
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> flatColorShader;
		Ref<Shader> textureShader;
	};

	static Renderer2DStorage* data;

	void Renderer2D::Init()
	{
		data = new Renderer2DStorage();
		data->quadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		data->quadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		data->quadVertexArray->SetIndexBuffer(squareIB);

		data->flatColorShader = Shader::Create("res/shaders/FlatColorShader.glsl");
		data->textureShader = Shader::Create("res/shaders/TextureShader.glsl");
		data->textureShader->Bind();
		data->textureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		data->flatColorShader->Bind();
		data->flatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		data->textureShader->Bind();
		data->textureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		data->flatColorShader->Bind();
		data->flatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data->flatColorShader->SetMat4("u_Transform", transform);

		data->quadVertexArray->Bind();
		RenderCommand::DrawIndexed(data->quadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		data->textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data->textureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		data->quadVertexArray->Bind();
		RenderCommand::DrawIndexed(data->quadVertexArray);
	}
}