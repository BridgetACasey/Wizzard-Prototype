//@BridgetACasey

#include "wzpch.h"

#include "AudioSource.h"

namespace Wizzard
{
	AudioSource::AudioSource(uint32_t handle, bool loaded, float length)
	{
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::SetPosition(float x, float y, float z)
	{
	}

	void AudioSource::SetGain(float gain)
	{
	}

	void AudioSource::SetPitch(float pitch)
	{
	}

	void AudioSource::SetSpatial(bool spatial)
	{
	}

	void AudioSource::SetLoop(bool loop)
	{
	}

	std::pair<uint32_t, uint32_t> AudioSource::GetLengthMinutesAndSeconds() const
	{
		return std::pair<uint32_t, uint32_t>();
	}

	AudioSource AudioSource::LoadFromFile(const std::string& file, bool spatial)
	{
		return AudioSource();
	}
}