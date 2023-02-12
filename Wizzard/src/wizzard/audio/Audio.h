//@BridgetACasey

#pragma once

namespace Wizzard
{
	class AudioSource;

	class Audio
	{
	public:
		static void Init();

		static AudioSource LoadAudioSource(const std::string& filename);
		static void Play(const AudioSource& source);

		// TODO: temporary whilst audio functionality is in early development
		static void SetDebugLogging(bool log);

	private:
		static AudioSource LoadAudioSourceMP3(const std::string& filename);

		//TODO: Implement support for .ogg files
		static AudioSource LoadAudioSourceOgg(const std::string& filename);
	};
}