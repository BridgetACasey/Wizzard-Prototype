//@BridgetACasey

#pragma once

namespace Wizzard
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		//TODO: Refactor this to return Ref instead of raw pointer
		static GraphicsContext* Create();
	};
}