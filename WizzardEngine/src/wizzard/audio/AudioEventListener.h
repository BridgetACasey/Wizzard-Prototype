//@BridgetACasey

#pragma once

#include <list>

#include "wizzard/event/AudioEvent.h"

namespace Wizzard
{
	class AudioEventListener
	{
	public:
		AudioEventListener() = default;
		~AudioEventListener();

		void OnUpdate();
		void OnEvent(Event& event);

	private:
		void QueueAudioSource(const AudioSource& source);

		bool OnAudioTrackStarted(AudioTrackStartedEvent& audioEvent);
		bool OnAudioTrackEnded(AudioTrackEndedEvent& audioEvent);

		std::list<AudioSource> audioSourceList;

		bool playingTrack = false;

		friend class Audio;
	};
}