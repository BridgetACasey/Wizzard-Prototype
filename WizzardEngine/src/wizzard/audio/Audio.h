//@BridgetACasey

#pragma once

namespace Wizzard
{
	class AudioSource;

	struct AudioData;

	class Audio
	{
	public:
		static void Init();

		static AudioSource LoadAudioSource(const std::string& fileName);

		static void Play(const AudioSource& audioSource);
		static void Pause(const AudioSource& audioSource);
		static void Stop(const AudioSource& audioSource);

		// TODO: temporary whilst audio functionality is in early development
		static void SetDebugLogging(bool log);
		static void PrintAudioDeviceInfo();

	private:
		static AudioSource LoadAudioSourceMP3(const std::string& fileName);

		//TODO: Implement support for .ogg files
		static AudioSource LoadAudioSourceOgg(const std::string& fileName);
	};
}