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

		virtual uint32_t GetColorAttachmentRendererID() const override { return colorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return specification; }
	private:
		uint32_t rendererID;
		uint32_t colorAttachment, depthAttachment;
		FramebufferSpecification specification;
	};
}