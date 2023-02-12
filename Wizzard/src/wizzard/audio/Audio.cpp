//@BridgetACasey

#include "wzpch.h"

#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "examples/common/alhelpers.h"

#include "AudioSource.h"

namespace Wizzard
{
	static ALCdevice* s_AudioDevice = nullptr;
	//static mp3dec_t s_Mp3d;

	static uint8_t* s_AudioScratchBuffer;
	static uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially

	static bool s_DebugLog = true;

	// Currently supported file formats
	enum class AudioFileFormat
	{
		None = 0,
		Ogg,
		MP3
	};

	static void PrintAudioDeviceInfo()
	{
		//if (s_DebugLog)
		//{
		//	WIZ_INFO("Audio Device Info:");
		//	WIZ_INFO("Name: " << s_AudioDevice->DeviceName);
		//	WIZ_INFO("Sample Rate: " << s_AudioDevice->Frequency);
		//	WIZ_INFO("Max Sources: " << s_AudioDevice->SourcesMax);
		//	WIZ_INFO("Mono: " << s_AudioDevice->NumMonoSources);
		//	WIZ_INFO("Stereo: " << s_AudioDevice->NumStereoSources);
		//}
	}

	void Audio::Init()
	{
		//if (InitAL(s_AudioDevice, nullptr) != 0)
		if (InitAL(nullptr, nullptr) != 0)
			WIZ_ERROR("Audio device error!");
		else
			WIZ_INFO("Successfully initalised OpenAL...");

		PrintAudioDeviceInfo();

		//mp3dec_init(&s_Mp3d);

		s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];

		// Init listener
		ALfloat listenerPos[] = { 0.0,0.0,0.0 };
		ALfloat listenerVel[] = { 0.0,0.0,0.0 };
		ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}

	AudioSource Audio::LoadAudioSource(const std::string& filename)
	{
		return AudioSource();
	}

	void Audio::Play(const AudioSource& source)
	{
	}

	void Audio::SetDebugLogging(bool log)
	{
	}

	AudioSource Audio::LoadAudioSourceOgg(const std::string& filename)
	{
		return AudioSource();
	}

	AudioSource Audio::LoadAudioSourceMP3(const std::string& filename)
	{
		return AudioSource();
	}
}
