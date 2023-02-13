//@BridgetACasey

#pragma once

//OpenAL forward declarations
//struct ALCcontext;
//struct ALCdevice;

namespace Wizzard
{
	class AudioSource;

	struct AudioData;

	class Audio
	{
	public:
		static void Init();

		static AudioSource LoadAudioSource(const std::string& filename);
		static void Play(const AudioSource& source);

		// TODO: temporary whilst audio functionality is in early development
		static void SetDebugLogging(bool log);
		static void PrintAudioDeviceInfo();

	private:
		static AudioSource LoadAudioSourceMP3(const std::string& filename);

		//TODO: Implement support for .ogg files
		static AudioSource LoadAudioSourceOgg(const std::string& filename);

		//static AudioData* audioData;
	};
}
