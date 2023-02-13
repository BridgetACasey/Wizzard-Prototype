//@BridgetACasey

#include "wzpch.h"

#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "examples/common/alhelpers.h"

#include "AudioSource.h"
#include "alc/context.h"
#include "alc/device.h"

namespace Wizzard
{
	// Currently supported file formats
	enum class AudioFileFormat
	{
		NONE = 0,
		MP3,
		OGG
	};

	struct AudioData
	{
		ALCcontext* context;
		ALCdevice* device;
	};

	//TODO:: Make this accessible through Audio header and in the rest of the project
	static AudioData* audioData = new AudioData();

	static bool debugLogging = true;

	void Audio::Init()
	{
		if (InitAL(nullptr, nullptr) != 0)
			WIZ_ERROR("Audio device error! Could not initialise OpenAL!");
		else
		{
			audioData->context = alcGetCurrentContext();
			audioData->device = alcGetContextsDevice(audioData->context);

			if (audioData->device)
				PrintAudioDeviceInfo();

			WIZ_INFO("Successfully initalised OpenAL...");
		}
	}

	void Audio::SetDebugLogging(bool log)
	{
	}

	void Audio::PrintAudioDeviceInfo()
	{
		if(debugLogging)
		{
			WIZ_INFO("---Audio Device Information---");
			WIZ_INFO("Name: {0}", audioData->device->DeviceName);
			WIZ_INFO("Sample Rate: {0}", audioData->device->Frequency);
			WIZ_INFO("Max Sources: {0}", audioData->device->SourcesMax);
			WIZ_INFO("Mono: {0}", audioData->device->NumMonoSources);
			WIZ_INFO("Stereo: {0}", audioData->device->NumStereoSources);
		}
	}

	AudioSource Audio::LoadAudioSource(const std::string& filename)
	{
		return AudioSource();
	}

	void Audio::Play(const AudioSource& source)
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
