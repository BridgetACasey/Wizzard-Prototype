//@BridgetACasey

#pragma once

#include "Scene.h"

namespace YAML
{
	class Emitter;
	class Node;
}

namespace Wizzard
{
	class SceneSerialiser
	{
	public:
		SceneSerialiser(const WizRef<Scene>& scene);

		void Serialise(const std::string& filepath);
		void SerialiseRuntime(const std::string& filepath);

		bool Deserialise(const std::string& filepath);
		bool DeserialiseRuntime(const std::string& filepath);

		static void SerialiseEntity(YAML::Emitter& out, Entity entity);
		static void DeserialiseEntities(YAML::Node& entitiesNode, WizRef<Scene> scene);

		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);

		inline static std::string_view fileFilter = "Wizzard Scene (*.wizscene)\0*.wizscene\0";
		inline static std::string_view defaultExtension = ".wizscene";

	private:
		WizRef<Scene> scene;
	};
}