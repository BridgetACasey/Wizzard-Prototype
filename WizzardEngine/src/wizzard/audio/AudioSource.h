//@BridgetACasey

#pragma once

#include <iostream>

namespace Wizzard
{
	class AudioSource
	{
	public:
		AudioSource() = default;
		~AudioSource();

		void FreeSource();	//TODO: Make this private

		bool IsLoaded() const { return loaded; }

		void SetPosition(float x, float y, float z);
		void SetGain(float g);
		void SetPitch(float p);
		void SetSpatial(bool s);
		void SetLoop(bool loop);

		[[nodiscard]] std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;

		static AudioSource LoadFromFile(const std::string& file, bool spatial = false);

		AudioSource& operator=(AudioSource source) noexcept
		{
			if (&source != this)
			{
				this->bufferHandle = source.bufferHandle;
				this->sourceHandle = source.sourceHandle;
				this->loaded = source.loaded;
				this->markForUnload = source.loaded;	//When assigning AudioSource via static function LoadFromFile
				this->spatial = source.spatial;
				this->totalDuration = source.totalDuration;
				this->position[0] = source.position[0];
				this->position[1] = source.position[1];
				this->position[2] = source.position[2];
				this->gain = source.gain;
				this->pitch = source.pitch;
				this->looping = source.looping;
			}

		    return *this;
		}

	private:
		AudioSource(uint32_t handle, bool loaded, float length);

		uint32_t bufferHandle = 0;
		uint32_t sourceHandle = 0;
		bool loaded = false;
		bool markForUnload = false;	//Enabled after audio source successfully loaded, used to check if destructor should remove OpenAL data
		bool spatial = false;

		float totalDuration = 0; // in seconds

		float position[3] = { 0.0f, 0.0f, 0.0f };
		float gain = 1.0f;
		float pitch = 1.0f;
		bool looping = false;

		friend class Audio;
	};
}