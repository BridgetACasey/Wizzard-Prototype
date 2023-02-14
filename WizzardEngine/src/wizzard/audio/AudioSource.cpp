//@BridgetACasey

#include "wzpch.h"

#include "AudioSource.h"

#include <al.h>
#include <alext.h>

#include "Audio.h"

namespace Wizzard
{
	AudioSource::AudioSource(uint32_t handle, bool loaded, float length) : bufferHandle(handle), loaded(loaded), totalDuration(length)
	{
		WIZ_PROFILE_FUNCTION();
	}

	AudioSource::~AudioSource()
	{
		//TODO: Free OpenAL audio source
	}

	void AudioSource::SetPosition(float x, float y, float z)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;

		alSourcefv(sourceHandle, AL_POSITION, position);
	}

	void AudioSource::SetGain(float g)
	{
		gain = g;

		alSourcef(sourceHandle, AL_GAIN, g);
	}

	void AudioSource::SetPitch(float p)
	{
		pitch = p;

		alSourcef(sourceHandle, AL_PITCH, p);
	}

	void AudioSource::SetSpatial(bool s)
	{
		spatial = s;

		alSourcei(sourceHandle, AL_SOURCE_SPATIALIZE_SOFT, s ? AL_TRUE : AL_FALSE);
		alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	}

	void AudioSource::SetLoop(bool loop)
	{
		looping = loop;

		alSourcei(sourceHandle, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	std::pair<uint32_t, uint32_t> AudioSource::GetLengthMinutesAndSeconds() const
	{
		return { (uint32_t)(totalDuration / 60.0f), (uint32_t)totalDuration % 60 };
	}

	AudioSource AudioSource::LoadFromFile(const std::string& file, bool spatial)
	{
		AudioSource result = Audio::LoadAudioSource(file);
		result.SetSpatial(spatial);
		return result;
	}
}