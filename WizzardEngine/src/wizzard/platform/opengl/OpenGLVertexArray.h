//@BridgetACasey

#pragma once

#include "rendering/VertexArray.h"

namespace Wizzard
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const WizRef<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const WizRef<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<WizRef<VertexBuffer>>& GetVertexBuffers() const { return vertexBuffers; }
		virtual const WizRef<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }

	private:
		uint32_t rendererID;
		uint32_t vertexBufferIndex = 0;
		std::vector<WizRef<VertexBuffer>> vertexBuffers;
		WizRef<IndexBuffer> indexBuffer;
	};
}