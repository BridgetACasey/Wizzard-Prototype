//@BridgetACasey

#pragma once

namespace Wizzard
{
	class AudioSource
	{
	public:
		~AudioSource();

		bool IsLoaded() const { return loaded; }

		void SetPosition(float x, float y, float z);
		void SetGain(float g);
		void SetPitch(float p);
		void SetSpatial(bool s);
		void SetLoop(bool loop);

		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;

		static AudioSource LoadFromFile(const std::string& file, bool spatial = false);

	private:
		AudioSource() = default;
		AudioSource(uint32_t handle, bool loaded, float length);

		uint32_t bufferHandle = 0;
		uint32_t sourceHandle = 0;
		bool loaded = false;
		bool spatial = false;

		float totalDuration = 0; // in seconds

		float position[3] = { 0.0f, 0.0f, 0.0f };
		float gain = 1.0f;
		float pitch = 1.0f;
		bool looping = false;

		friend class Audio;
	};
}