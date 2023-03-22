#pragma once

#include <glm/glm.hpp>

namespace Wizzard::Maths
{
	struct WizVector2f
	{
		float x, y;

		constexpr WizVector2f() : x(0.0f), y(0.0f) {}
		constexpr WizVector2f(float x, float y) : x(x), y(y) {}
	};

	struct WizVector3f
	{
		float x, y, z;

		constexpr WizVector3f() : x(0.0f), y(0.0f), z(0.0f) {}
		constexpr WizVector3f(float x, float y, float z) : x(x), y(y), z(z) {}
	};

	struct WizVector4f
	{
		float x, y, z, w;

		constexpr WizVector4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		constexpr WizVector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	};

	struct WizVector2i
	{
		int x, y;

		constexpr WizVector2i() : x(0), y(0) {}
		constexpr WizVector2i(int x, int y) : x(x), y(y) {}
	};

	struct WizVector3i
	{
		int x, y, z;

		constexpr WizVector3i() : x(0), y(0), z(0) {}
		constexpr WizVector3i(int x, int y, int z) : x(x), y(y), z(z) {}
	};

	struct WizVector4i
	{
		int x, y, z, w;

		constexpr WizVector4i() : x(0), y(0), z(0), w(0) {}
		constexpr WizVector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	};

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}