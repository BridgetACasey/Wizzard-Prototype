//@BridgetACasey

#include "wzpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "base/ResourcePathFinder.h"
#include "glm/ext/matrix_transform.hpp"
#include "platform/opengl/OpenGLShader.h"

namespace Wizzard
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		WizRef<VertexArray> quadVertexArray;
		WizRef<VertexBuffer> quadVertexBuffer;
		WizRef<Shader> textureShader;
		WizRef<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<WizRef<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics statistics;
	};

	static Renderer2DData* data;

	void Renderer2D::Init()
	{
		WIZ_PROFILE_FUNCTION();

		data = new Renderer2DData();

		data->quadVertexArray = VertexArray::Create();
		data->quadVertexBuffer = VertexBuffer::Create(data->maxVertices * sizeof(QuadVertex));

		data->quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" },
			});
		data->quadVertexArray->AddVertexBuffer(data->quadVertexBuffer);

		data->quadVertexBufferBase = new QuadVertex[data->maxVertices];

		uint32_t* quadIndices = new uint32_t[data->maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < data->maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		WizRef<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, data->maxIndices);
		data->quadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		data->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[data->MaxTextureSlots];
		for (uint32_t i = 0; i < data->MaxTextureSlots; i++)
			samplers[i] = i;

		data->textureShader = Shader::Create(ResourcePath::GetResourcePath(SHADER, "TextureShader.glsl"));
		data->textureShader->Bind();
		data->textureShader->SetIntArray("u_Textures", samplers, data->MaxTextureSlots);

		// Set all texture slots to 0
		data->TextureSlots[0] = data->whiteTexture;

		data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data->QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		data->QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		WIZ_PROFILE_FUNCTION();

		delete data;
		data->TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		WIZ_PROFILE_FUNCTION();

		data->textureShader->Bind();
		data->textureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		data->quadIndexCount = 0;
		data->quadVertexBufferPtr = data->quadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		WIZ_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjection();

		data->textureShader->Bind();
		data->textureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		WIZ_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		data->textureShader->Bind();
		data->textureShader->SetMat4("u_ViewProjection", viewProj);
		data->quadIndexCount = 0;
		data->quadVertexBufferPtr = data->quadVertexBufferBase;
		data->TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		WIZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)data->quadVertexBufferPtr - (uint8_t*)data->quadVertexBufferBase;
		data->quadVertexBuffer->SetData(data->quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		WIZ_PROFILE_FUNCTION();

		// Bind textures
		for (uint32_t i = 0; i < data->TextureSlotIndex; i++)
			data->TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(data->quadVertexArray, data->quadIndexCount);
		data->statistics.drawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		WIZ_PROFILE_FUNCTION();

		EndScene();

		data->quadIndexCount = 0;
		data->quadVertexBufferPtr = data->quadVertexBufferBase;

		data->TextureSlotIndex = 1;
	}

	void Renderer2D::StartBatch()
	{
		data->quadIndexCount = 0;
		data->quadVertexBufferPtr = data->quadVertexBufferBase;
		data->TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		WIZ_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		WIZ_PROFILE_FUNCTION();

		if (data->quadIndexCount >= Renderer2DData::maxIndices)
			FlushAndReset();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[0];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[1];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[2];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[3];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadIndexCount += 6;

		data->statistics.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const WizRef<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		WIZ_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const WizRef<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		WIZ_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (data->quadIndexCount >= Renderer2DData::maxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data->TextureSlotIndex; i++)
		{
			if (data->TextureSlots[i].Get() == texture.Get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)data->TextureSlotIndex;
			data->TextureSlots[data->TextureSlotIndex] = texture;
			data->TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[0];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[1];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[2];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[3];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadIndexCount += 6;

		data->statistics.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		WIZ_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (data->quadIndexCount >= Renderer2DData::maxIndices)
			FlushAndReset();

		for(size_t i = 0; i < quadVertexCount; i++)
		{
			data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[i];
			data->quadVertexBufferPtr->Color = color;
			data->quadVertexBufferPtr->TexCoord = textureCoords[i];
			data->quadVertexBufferPtr->TexIndex = textureIndex;
			data->quadVertexBufferPtr->TilingFactor = tilingFactor;
			data->quadVertexBufferPtr->EntityID = entityID;
			data->quadVertexBufferPtr++;
		}

		data->quadIndexCount += 6;

		data->statistics.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const WizRef<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		WIZ_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (data->quadIndexCount >= Renderer2DData::maxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data->TextureSlotIndex; i++)
		{
			if (*data->TextureSlots[i].Get() == *texture.Get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (data->TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)data->TextureSlotIndex;
			data->TextureSlots[data->TextureSlotIndex] = texture;
			data->TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[i];
			data->quadVertexBufferPtr->Color = tintColor;
			data->quadVertexBufferPtr->TexCoord = textureCoords[i];
			data->quadVertexBufferPtr->TexIndex = textureIndex;
			data->quadVertexBufferPtr->TilingFactor = tilingFactor;
			data->quadVertexBufferPtr->EntityID = entityID;
			data->quadVertexBufferPtr++;
		}

		data->quadIndexCount += 6;

		data->statistics.quadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		WIZ_PROFILE_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		WIZ_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		if (data->quadIndexCount >= Renderer2DData::maxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[0];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[1];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[2];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[3];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadIndexCount += 6;

		data->statistics.quadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const WizRef<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		WIZ_PROFILE_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const WizRef<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		WIZ_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (data->quadIndexCount >= Renderer2DData::maxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data->TextureSlotIndex; i++)
		{
			if (*data->TextureSlots[i].Get() == *texture.Get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)data->TextureSlotIndex;
			data->TextureSlots[data->TextureSlotIndex] = texture;
			data->TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[0];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[1];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[2];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadVertexBufferPtr->Position = transform * data->QuadVertexPositions[3];
		data->quadVertexBufferPtr->Color = color;
		data->quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		data->quadVertexBufferPtr->TexIndex = textureIndex;
		data->quadVertexBufferPtr->TilingFactor = tilingFactor;
		data->quadVertexBufferPtr++;

		data->quadIndexCount += 6;

		data->statistics.quadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteComponent& src, int entityID)
	{
		DrawQuad(transform, src.color, entityID);
	}

	void Renderer2D::ResetStatistics()
	{
		WIZ_PROFILE_FUNCTION();

		memset(&data->statistics, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		WIZ_PROFILE_FUNCTION();

		return data->statistics;
	}
}