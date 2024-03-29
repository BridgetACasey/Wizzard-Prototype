//@BridgetACasey

#include "wizzard/rendering/Buffer.h"

namespace Wizzard
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return bufferLayout; }
		virtual void SetLayout(const BufferLayout& layout) override { bufferLayout = layout; }
	private:
		uint32_t rendererID;
		BufferLayout bufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return count; }
	private:
		uint32_t rendererID;
		uint32_t count;
	};
}