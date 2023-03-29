//@BridgetACasey

#pragma once

namespace Wizzard
{
	class AudioSource;
	class AudioEventListener;

	struct AudioData;

#define WIZ_AUDIO_UIWINDOWCHANGED "UIWindowChanged"
#define WIZ_AUDIO_UIELEMENTSELECTED "UIElementSelected"
#define WIZ_AUDIO_EDITORSTARTUP "EditorStartUp"
#define WIZ_AUDIO_EDITORSHUTDOWN "EditorShutDown"
#define WIZ_AUDIO_ENTITYSELECTED "EntitySelected"

	class Audio
	{
	public:
		static void Init();
		static void Shutdown();

		static AudioSource LoadAudioSource(const std::string& fileName);
		static AudioSource LoadAudioSourceForEditor(const char* assetName, const std::string& fileName);

		static void Play(const AudioSource& audioSource);
		static void Pause(const AudioSource& audioSource);
		static void Stop(const AudioSource& audioSource);

		static void QueuePlay(const AudioSource& audioSource);

		// TODO: temporary whilst audio functionality is in early development
		static void SetDebugLogging(bool log);
		static void PrintAudioDeviceInfo();

		static bool IsActive();

		static AudioEventListener* GetEventListener() { return eventListener; }

		static AudioSource& GetEditorAudioSource(const char* assetName) { return editorAudioSources.at(assetName); }

	private:
		static AudioSource LoadAudioSourceMP3(const std::string& fileName);
		static AudioSource LoadAudioSourceOgg(const std::string& fileName);

		inline static AudioEventListener* eventListener = nullptr;

		inline static std::unordered_map<const char*, AudioSource> editorAudioSources;	//TODO: In future, decouple this from audio class and handle in an asset manager
	};
}