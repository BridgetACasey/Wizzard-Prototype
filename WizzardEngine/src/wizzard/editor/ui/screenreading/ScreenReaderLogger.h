//@BridgetACasey

#pragma once

#include <list>

#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/AudioEvent.h"

namespace Wizzard
{
	class ScreenReaderLogger
	{
	public:
		static void Init();
		static void Shutdown();

		static bool Stop();

		static void OnUpdate();
		static void OnEvent(Event& event);

		static void ForceQueueOutput(const std::string& message);
		static void QueueOutput(const std::string& message, bool shouldInterrupt = true);

		static bool DetectScreenReader();
		static std::string GetActiveScreenReaderName();

		static bool QuerySpeechSupport();
		static bool QueryBrailleSupport();

		static bool IsSpeaking();

		static int MessageQueueCount() { return messageBackLog.size(); }

	private:
		static bool OnScreenReaderMessageStarted(ScreenReaderMessageStartedEvent& srEvent);
		static bool OnScreenReaderMessageEnded(ScreenReaderMessageEndedEvent& srEvent);
		static bool OnAppShutdown(AppShutdownEvent& appEvent);

		static bool OutputAll(const std::string& message, bool shouldInterrupt);
		static bool OutputSpeech(const std::string& message, bool shouldInterrupt);
		static bool OutputBraille(const std::string& message);

		inline static std::list<std::string> messageBackLog;

		inline static bool startedMessage = false;
		inline static bool preferSAPI = false;	//Flag to determine if Tolk should look for SAPI first or last.
	};
}