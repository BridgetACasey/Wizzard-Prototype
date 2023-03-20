//@BridgetACasey

#pragma once

#include "Event.h"

#include "wizzard/audio/AudioSource.h"

namespace Wizzard
{
	class AudioEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryAudio)
	};

	class AudioTrackStartedEvent : public AudioEvent
	{
	public:
		AudioTrackStartedEvent(const AudioSource& source) : audioSource(source) {}

		EVENT_CLASS_TYPE(AudioTrackStarted)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AudioTrackStartedEvent: File - ";// << audioSource.GetFileName() << " Duration - " << audioSource.GetTotalDuration();
			return ss.str();
		}

		const AudioSource& GetAudioSource() { return audioSource; }

	protected:
		AudioSource audioSource;
	};

	class AudioTrackEndedEvent : public AudioEvent
	{
	public:
		AudioTrackEndedEvent(const AudioSource& source) : audioSource(source) {}

		EVENT_CLASS_TYPE(AudioTrackEnded)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AudioTrackEndedEvent: File - " << audioSource.GetFileName() << " Duration - " << audioSource.GetTotalDuration();
			return ss.str();
		}

		const AudioSource& GetAudioSource() { return audioSource; }

	protected:
		AudioSource audioSource;
	};

	class ScreenReaderMessageStartedEvent : public AudioEvent
	{
	public:
		ScreenReaderMessageStartedEvent(std::string& message) : message(message){}

		EVENT_CLASS_TYPE(ScreenReaderMessageStarted)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScreenReaderMessageStartedEvent: " << message;
			return ss.str();
		}

		const std::string& GetSRMessage() { return message; }

	protected:
		std::string message;
	};

	class ScreenReaderMessageEndedEvent : public AudioEvent
	{
	public:
		ScreenReaderMessageEndedEvent(std::string& message) : message(message){}

		EVENT_CLASS_TYPE(ScreenReaderMessageEnded)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScreenReaderMessageEndedEvent: " << message;
			return ss.str();
		}

		const std::string& GetSRMessage() { return message; }

	protected:
		std::string message;
	};
}