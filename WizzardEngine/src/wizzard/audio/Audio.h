//@BridgetACasey

#pragma once

namespace Wizzard
{
	class AudioSource;
	class AudioEventListener;

	struct AudioData;

	class Audio
	{
	public:
		static void Init();
		static void Shutdown();

		static AudioSource LoadAudioSource(const std::string& fileName);

		static void Play(const AudioSource& audioSource);
		static void Pause(const AudioSource& audioSource);
		static void Stop(const AudioSource& audioSource);

		static void QueuePlay(const AudioSource& audioSource);

		// TODO: temporary whilst audio functionality is in early development
		static void SetDebugLogging(bool log);
		static void PrintAudioDeviceInfo();

		static bool IsActive();

		static AudioEventListener* GetEventListener() { return eventListener; }

	private:
		static AudioSource LoadAudioSourceMP3(const std::string& fileName);
		static AudioSource LoadAudioSourceOgg(const std::string& fileName);

		inline static AudioEventListener* eventListener = nullptr;
	};
}