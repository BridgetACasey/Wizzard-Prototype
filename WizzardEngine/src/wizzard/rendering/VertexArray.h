//@BridgetACasey

#pragma once

#include <memory>
#include "wizzard/rendering/Buffer.h"

namespace Wizzard
{
	class VertexArray : public WizRefCounter
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const WizRef<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const WizRef<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<WizRef<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const WizRef<IndexBuffer>& GetIndexBuffer() const = 0;

		static WizRef<VertexArray> Create();
	};
}