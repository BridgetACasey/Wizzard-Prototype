//@BridgetACasey

#pragma once

#include "wizzard/core/Core.h"

namespace Wizzard
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}