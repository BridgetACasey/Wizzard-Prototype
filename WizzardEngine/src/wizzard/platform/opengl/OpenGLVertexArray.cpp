//@BridgetACasey

#include "wzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Wizzard
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		WIZ_PROFILE_FUNCTION();

		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		WIZ_ASSERT(false, "Unknown ShaderDataType!");

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		WIZ_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		WIZ_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &rendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		WIZ_PROFILE_FUNCTION();

		glBindVertexArray(rendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		WIZ_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const WizRef<VertexBuffer>& vertexBuffer)
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(rendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					element.isNormalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribIPointer(vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					layout.GetStride(),
					(const void*)element.offset);
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.isNormalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(vertexBufferIndex, 1);
					vertexBufferIndex++;
				}
				break;
			}
			default:
				WIZ_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
		vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const WizRef<IndexBuffer>& buffer)
	{
		WIZ_PROFILE_FUNCTION();

		glBindVertexArray(rendererID);
		buffer->Bind();

		indexBuffer = buffer;
	}
}