//@BridgetACasey

#include "wzpch.h"

#include "AudioSource.h"

#include <al.h>
#include <alext.h>

#include "Audio.h"
#include "AudioEventListener.h"

namespace Wizzard
{
	AudioSource::AudioSource(uint32_t handle, bool loaded, float length) : bufferHandle(handle), loaded(loaded), totalDuration(length)
	{
	}

	AudioSource::~AudioSource()
	{
		if(markForUnload)
		FreeSource();
	}

	void AudioSource::FreeSource()
	{
		if (alIsSource(sourceHandle))
			alDeleteSources(1, &sourceHandle);

		if (alIsBuffer(bufferHandle))
			alDeleteBuffers(1, &bufferHandle);
	}

	int32_t AudioSource::GetCurrentPlayState()
	{
		alGetSourcei(sourceHandle, AL_SOURCE_STATE, &playState);

		return playState;
	}

	int32_t AudioSource::GetCurrentBufferState()
	{
		alGetBufferi(bufferHandle, AL_BUFFER, &bufferState);

		return bufferState;
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

	AudioSource AudioSource::LoadFromFile(const std::string& file, bool spatial, bool forEditor, const char* assetName)
	{
		AudioSource result = forEditor ? Audio::LoadAudioSourceForEditor(assetName, file) : Audio::LoadAudioSource(file);
		result.SetSpatial(spatial);

		return result;
	}
}