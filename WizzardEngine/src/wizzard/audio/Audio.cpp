//@BridgetACasey

#include "wzpch.h"

#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <thread>
#include <filesystem>

#include "examples/common/alhelpers.h"

#include "AudioSource.h"
#include "AudioEventListener.h"

#include "alc/context.h"
#include "alc/device.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

#include "vorbis/vorbisfile.h"
#include "vorbis/codec.h"

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

	static mp3dec_t mp3d;

	static uint8_t* audioScratchBuffer;
	static uint32_t audioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially

	static bool isActive = false;

	static AudioFileFormat GetFileFormat(const std::string& fileName)
	{
		WIZ_PROFILE_FUNCTION();

		std::filesystem::path path = fileName;
		std::string extension = path.extension().string();

		if (extension == ".mp3")  return AudioFileFormat::MP3;
		if (extension == ".ogg")  return AudioFileFormat::OGG;

		return AudioFileFormat::NONE;
	}

	static ALenum GetOpenALFormat(uint32_t channels)
	{
		WIZ_PROFILE_FUNCTION();

		// Note: sample size is always 2 bytes (16-bits) with
		// both the .mp3 and .ogg decoders that we're using
		switch (channels)
		{
			case 1:  return AL_FORMAT_MONO16;
			case 2:  return AL_FORMAT_STEREO16;
		}

		// assert
		return 0;
	}

	void Audio::Init()
	{
		WIZ_PROFILE_FUNCTION();

		if (InitAL(nullptr, nullptr) != 0)
			WIZ_ERROR("Audio device error! Could not initialise OpenAL!");
		else
		{
			audioData->context = alcGetCurrentContext();
			audioData->device = alcGetContextsDevice(audioData->context);

			if (audioData->device)
				PrintAudioDeviceInfo();

			mp3dec_init(&mp3d);

			audioScratchBuffer = new uint8_t[audioScratchBufferSize];

			eventListener = new AudioEventListener();

			WIZ_INFO("Successfully initalised OpenAL...");

			isActive = true;
		}
	}

	void Audio::Shutdown()
	{
		WIZ_PROFILE_FUNCTION();

		if(eventListener)
		{
			delete eventListener;
			eventListener = nullptr;
		}

		if(audioScratchBuffer)
		{
			delete audioScratchBuffer;
			audioScratchBuffer = nullptr;
		}

		CloseAL();
	}

	void Audio::SetDebugLogging(bool log)
	{
		WIZ_PROFILE_FUNCTION();

		debugLogging = log;
	}

	void Audio::PrintAudioDeviceInfo()
	{
		WIZ_PROFILE_FUNCTION();

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

	bool Audio::IsActive()
	{
		return isActive;
	}

	AudioSource Audio::LoadAudioSource(const std::string& fileName)
	{
		WIZ_PROFILE_FUNCTION();

		auto format = GetFileFormat(fileName);
		switch (format)
		{
			case AudioFileFormat::MP3:  return LoadAudioSourceMP3(fileName);
			case AudioFileFormat::OGG:  return LoadAudioSourceOgg(fileName);
		}

		// Loading failed or unsupported file type
		return { 0, false, 0 };
	}

	AudioSource Audio::LoadAudioSourceForEditor(const char* assetName, const std::string& fileName)
	{
		AudioSource source = LoadAudioSource(fileName);

		if (source.IsLoaded())
			editorAudioSources.emplace(assetName, source);

		return source;
	}

	void Audio::Play(const AudioSource& audioSource)
	{
		WIZ_PROFILE_FUNCTION();

		// Play the sound until it finishes
		alSourcePlay(audioSource.sourceHandle);

		// TODO: current playback time and playback finished callback
		// eg.
		// ALfloat offset;
		// alGetSourcei(audioSource.sourceHandle, AL_SOURCE_STATE, &playState);
		// ALenum playState;
		// alGetSourcef(audioSource.sourceHandle, AL_SEC_OFFSET, &offset);
	}

	void Audio::Pause(const AudioSource& audioSource)
	{
		WIZ_PROFILE_FUNCTION();

		// Stop the sound if it is currently playing
		alSourcePause(audioSource.sourceHandle);
	}

	void Audio::Stop(const AudioSource& audioSource)
	{
		WIZ_PROFILE_FUNCTION();

		// Stop the sound if it is currently playing
		alSourceStop(audioSource.sourceHandle);
	}

	void Audio::QueuePlay(const AudioSource& audioSource)
	{
		eventListener->QueueAudioSource(audioSource);
	}

	AudioSource Audio::LoadAudioSourceMP3(const std::string& fileName)
	{
		WIZ_PROFILE_FUNCTION();

		mp3dec_file_info_t info;
		int loadResult = mp3dec_load(&mp3d, fileName.c_str(), &info, NULL, NULL);
		uint32_t size = info.samples * sizeof(mp3d_sample_t);

		auto sampleRate = info.hz;
		auto channels = info.channels;
		auto alFormat = GetOpenALFormat(channels);
		float lengthSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

		AudioSource result = { buffer, true, lengthSeconds };
		alGenSources(1, &result.sourceHandle);
		alSourcei(result.sourceHandle, AL_BUFFER, buffer);

		if (debugLogging)
		{
			WIZ_INFO("File Info - {0}", fileName);
			WIZ_INFO("Channels: {0}", channels);
			WIZ_INFO("Sample Rate: {0}", sampleRate);
			WIZ_INFO("Size: {0} bytes", size);

			auto [mins, secs] = result.GetLengthMinutesAndSeconds();
			WIZ_INFO("Length: {0} mins {1} seconds", mins, secs);
		}

		if (alGetError() != AL_NO_ERROR)
		{
			WIZ_ERROR("Failed to setup sound source.");

			if(!fileName.empty())
			WIZ_ERROR("Could not find sound source at {0}", fileName);
		}

		result.fileName = fileName;	//TODO: Fix this not passing to AudioSource properly

		return result;
	}

	AudioSource Audio::LoadAudioSourceOgg(const std::string& fileName)
	{
		WIZ_PROFILE_FUNCTION();

		FILE* file = fopen(fileName.c_str(), "rb");

		OggVorbis_File vorbisFile;
		if (ov_open_callbacks(file, &vorbisFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
			WIZ_ERROR("Could not open ogg stream!");

		// Useful info
		vorbis_info* vorbisInfo = ov_info(&vorbisFile, -1);
		auto sampleRate = vorbisInfo->rate;
		auto channels = vorbisInfo->channels;
		auto alFormat = GetOpenALFormat(channels);

		uint64_t samples = ov_pcm_total(&vorbisFile, -1);
		float trackLength = (float)samples / (float)sampleRate; // in seconds
		uint32_t bufferSize = 2 * channels * samples; // 2 bytes per sample (I'm guessing...)

		if (debugLogging)
		{
			WIZ_INFO("File Info - {0}:", fileName);
			WIZ_INFO("Channels: {0}", channels);
			WIZ_INFO("Sample Rate: {0}", sampleRate);
			WIZ_INFO("Expected size: {0}", bufferSize);
		}

		// TODO: Replace with Wizzard::Buffer
		if (audioScratchBufferSize < bufferSize)
		{
			audioScratchBufferSize = bufferSize;
			delete[] audioScratchBuffer;
			audioScratchBuffer = new uint8_t[audioScratchBufferSize];
		}

		uint8_t* oggBuffer = audioScratchBuffer;
		uint8_t* bufferPtr = oggBuffer;
		int eof = 0;
		while (!eof)
		{
			int currentSection;
			long length = ov_read(&vorbisFile, (char*)bufferPtr, 4096, 0, 2, 1, &currentSection);
			bufferPtr += length;
			if (length == 0)
			{
				eof = 1;
			}
			else if (length < 0)
			{
				if (length == OV_EBADLINK)
				{
					WIZ_ERROR("Corrupt bitstream section! Exiting...");
					exit(1);
				}
			}
		}

		// assert bufferSize == size
		uint32_t size = bufferPtr - oggBuffer;

		if (debugLogging)
			WIZ_INFO("Read {0} bytes", size);

		// Release file
		ov_clear(&vorbisFile);
		fclose(file);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, oggBuffer, size, sampleRate);

		AudioSource audioResult = { buffer, true, trackLength };
		alGenSources(1, &audioResult.sourceHandle);
		alSourcei(audioResult.sourceHandle, AL_BUFFER, buffer);

		if (alGetError() != AL_NO_ERROR)
			WIZ_ERROR("Failed to setup sound source!");

		return audioResult;
	}
}