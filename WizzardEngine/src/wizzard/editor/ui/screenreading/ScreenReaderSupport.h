//@BridgetACasey

#pragma once

namespace Wizzard
{
	class ScreenReaderSupport
	{
	public:
		static void Init();
		static void Shutdown();

		static bool Stop();

		static bool DetectScreenReader();
		static std::string GetActiveScreenReaderName();

		static bool QuerySpeechSupport();
		static bool QueryBrailleSupport();

		static bool OutputAll(const std::string& message, bool shouldInterrupt = true);
		static bool OutputSpeech(const std::string& message, bool shouldInterrupt = true);
		static bool OutputBraille(const std::string& message);

		static bool IsSpeaking();

	private:
		inline static bool preferSAPI = false;	//Flag to determine if Tolk should look for SAPI first or last.
	};
}