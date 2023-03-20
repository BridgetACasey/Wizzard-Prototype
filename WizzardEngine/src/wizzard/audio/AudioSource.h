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

		int32_t GetCurrentPlayState();
		int32_t GetCurrentBufferState();

		bool IsLoaded() const { return loaded; }

		std::string GetFileName() const { return fileName; }
		float GetCurrentPlayback() const { return currentPlayback; }
		float GetTotalDuration() const { return totalDuration; }

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
				this->currentPlayback = source.currentPlayback;
				this->position[0] = source.position[0];
				this->position[1] = source.position[1];
				this->position[2] = source.position[2];
				this->gain = source.gain;
				this->pitch = source.pitch;
				this->looping = source.looping;
			}

		    return *this;
		}

		bool operator==(AudioSource source) const noexcept
		{
			return sourceHandle == source.sourceHandle;
		}

	private:
		AudioSource(uint32_t handle, bool loaded, float length);

		std::string fileName;

		uint32_t bufferHandle = 0;
		uint32_t sourceHandle = 0;
		int32_t playState = 0;
		int32_t bufferState = 0;

		bool loaded = false;
		bool markForUnload = false;	//Enabled after audio source successfully loaded, used to check if destructor should remove OpenAL data
		bool spatial = false;

		float totalDuration = 0.0f; //In seconds
		float currentPlayback = 0.0f;	//The current progress of the audio playback in seconds

		float position[3] = { 0.0f, 0.0f, 0.0f };
		float gain = 1.0f;
		float pitch = 1.0f;
		bool looping = false;

		friend class Audio;
		friend class AudioEventListener;
	};
}