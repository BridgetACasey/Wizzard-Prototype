//@BridgetACasey

#pragma once

#include "box2d/b2_body.h"

namespace Wizzard
{
	class Physics2D
	{
	public:
		Physics2D(){};
		
		inline void CreateBox2DWorld()
		{
			gravity = b2Vec2(0.0f, -9.8f);
			//world = new b2World(gravity);
		}

		b2Vec2 gravity;
		b2World* world;
	};
}