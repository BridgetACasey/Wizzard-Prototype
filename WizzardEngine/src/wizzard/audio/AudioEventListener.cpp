//@BridgetACasey

#include "wzpch.h"

#include "AudioEventListener.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Audio.h"
#include "wizzard/event/EventHandler.h"

namespace Wizzard
{
	AudioEventListener::~AudioEventListener()
	{
	}

	void AudioEventListener::OnUpdate()
	{
		if(!audioSourceList.empty())
		{
			AudioSource& source = audioSourceList.front();

			switch (source.GetCurrentPlayState())
			{
				case AL_INITIAL:
				{
					Audio::Play(source);
				}
				break;
				case AL_PLAYING:
				{
					if (!playingTrack)
					{
						//TODO: Fix audio callback for track begin, also currently event callbacks in other classes are overwritten by this class's
						if (alGetError() == AL_NO_ERROR)
						{
							WIZ_TRACE("Source is playing");

							//if (alIsSource(source.sourceHandle))
							//{
							//AudioTrackStartedEvent audioEvent(source);
							//OnAudioTrackStarted(audioEvent);

							playingTrack = true;
						}
					}
				}
				break;
				case AL_PAUSED:
					WIZ_TRACE("Source is paused");
					break;
				case AL_STOPPED:
				{
					AudioTrackEndedEvent audioEvent(source);
					OnAudioTrackEnded(audioEvent);

					playingTrack = false;
					audioSourceList.pop_front();
				}
				break;
				default: WIZ_TRACE("No appropriate state for audio source");
			}
		}
	}

	void AudioEventListener::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);

		eventHandler.HandleEvent<AudioTrackStartedEvent>(WIZ_BIND_EVENT_FN(OnAudioTrackStarted));
		eventHandler.HandleEvent<AudioTrackEndedEvent>(WIZ_BIND_EVENT_FN(OnAudioTrackEnded));
	}

	void AudioEventListener::QueueAudioSource(const AudioSource& source)
	{
		audioSourceList.push_back(source);
	}

	bool AudioEventListener::OnAudioTrackStarted(AudioTrackStartedEvent& audioEvent)
	{
		WIZ_TRACE(audioEvent);

		return false;
	}

	bool AudioEventListener::OnAudioTrackEnded(AudioTrackEndedEvent& audioEvent)
	{
		WIZ_TRACE(audioEvent);

		return false;
	}
}