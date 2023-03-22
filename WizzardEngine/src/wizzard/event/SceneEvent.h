//@BridgetACasey

#pragma once

#include "Event.h"
#include "wizzard/scene/Scene.h"

namespace Wizzard
{
	class SceneEvent : public Event
	{
	public:
		const WizRef<Scene>& GetScene() const { return scene; }
		WizRef<Scene> GetScene() { return scene; }

		EVENT_CLASS_CATEGORY(EventCategoryEditor | EventCategoryScene)
	protected:
		SceneEvent(const WizRef<Scene>& scene) : scene(scene) {}

		WizRef<Scene> scene;
	};

	class SceneBeginPlayEvent : public SceneEvent
	{
	public:
		SceneBeginPlayEvent(const WizRef<Scene>& scene) : SceneEvent(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SceneBeginPlayEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(SceneBeginPlay)
	};

	class ScenePreBeginPlayEvent : public SceneEvent
	{
	public:
		ScenePreBeginPlayEvent(const WizRef<Scene>& scene) : SceneEvent(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePreBeginPlayEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(ScenePreBeginPlay)
	};

	class ScenePostBeginPlayEvent : public SceneEvent
	{
	public:
		ScenePostBeginPlayEvent(const WizRef<Scene>& scene) : SceneEvent(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePostBeginPlayEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(ScenePostBeginPlay)
	};

	class ScenePreEndPlayEvent : public SceneEvent
	{
	public:
		ScenePreEndPlayEvent(const WizRef<Scene>& scene) : SceneEvent(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePreEndPlayEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(ScenePreEndPlay)
	};

	class ScenePostEndPlayEvent : public SceneEvent
	{
	public:
		ScenePostEndPlayEvent(const WizRef<Scene>& scene) : SceneEvent(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePostEndPlayEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(ScenePostEndPlay)
	};

	class SceneEndPlayEvent : public SceneEvent
	{
	public:
		SceneEndPlayEvent(const WizRef<Scene>& scene) : SceneEvent(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SceneEndPlayEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(SceneEndPlay)
	};
}