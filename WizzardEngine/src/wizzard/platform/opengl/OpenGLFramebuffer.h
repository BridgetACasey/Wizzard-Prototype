//@BridgetACasey

#pragma once

#include "wizzard/rendering/Framebuffer.h"

namespace Wizzard
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return colorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return specification; }

	private:
		uint32_t rendererID;
		uint32_t colorAttachment, depthAttachment;
		FramebufferSpecification specification;
	};
}