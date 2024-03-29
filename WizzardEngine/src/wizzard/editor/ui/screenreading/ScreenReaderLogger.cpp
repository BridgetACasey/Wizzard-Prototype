//@BridgetACasey

#include "wzpch.h"

#include "ScreenReaderLogger.h"

#include <comdef.h>
#include "Tolk.h"
#include "wizzard/common/Application.h"
#include "wizzard/event/EventHandler.h"

namespace Wizzard
{
	void ScreenReaderLogger::Init()
	{
		if (Tolk_IsLoaded())
		{
			WIZ_WARN("Calling ScreenReaderLogger::Init when Tolk is already initialised! Ignoring process.");
			return;
		}

		//TODO: Fix exception "the RPC server is unavailable" when using SAPI with Tolk library
		Tolk_TrySAPI(preferSAPI);	//Calling this before Tolk_Load for best performance

		//Tolk will also initialise COM if it has not been initialized on the calling thread
		Tolk_Load();

		if (Tolk_IsLoaded())
		{
			WIZ_TRACE("Initialised Tolk Screen Reader Abstraction Library.");

			if(DetectScreenReader())
			{
				if (Tolk_HasSpeech())
					WIZ_INFO("This screen reader driver supports speech.");

				if (Tolk_HasBraille())
					WIZ_INFO("This screen reader driver supports braille.");
			}
			else
			{
				WIZ_TRACE("Attempting to initialise Microsoft SAPI backup drivers...");

				preferSAPI = true;
				Tolk_TrySAPI(preferSAPI);
			}

			Application::Get().GetAppShutdownDelegate().Bind(OnAppShutdown);
		}
		else
			WIZ_ERROR("Failed to initialise Tolk Screen Reader Abstraction Library!");
	}

	void ScreenReaderLogger::Shutdown()
	{
		if (Tolk_IsLoaded())
		{
			WIZ_TRACE("Unloading Tolk Screen Reader Abstraction Library...");
			Tolk_Unload();
		}
		else
			WIZ_ERROR("Trying to disable Screen Reader Support and unload Tolk multiple times!");
	}

	bool ScreenReaderLogger::Stop()
	{
		return Tolk_Silence();
	}

	void ScreenReaderLogger::OnUpdate()
	{
		if (!messageBackLog.empty())
		{
			if (!IsSpeaking())
			{
				if (!startedMessage)
				{
					//priorityMessageTriggered = messageBackLog.front().second;

					OutputAll(messageBackLog.front().first, false);

					ScreenReaderMessageStartedEvent srEvent(messageBackLog.front().first, messageBackLog.front().second);
					OnScreenReaderMessageStarted(srEvent);

					startedMessage = true;
				}
				else// if (startedMessage)
				{
					ScreenReaderMessageEndedEvent srEvent(messageBackLog.front().first, messageBackLog.front().second);
					OnScreenReaderMessageEnded(srEvent);

					startedMessage = false;
					priorityMessageTriggered = false;
					messageBackLog.pop_front();		//Pop off queen! (painting nails emoji)
				}
			}
			
		}
	}

	void ScreenReaderLogger::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);

		eventHandler.HandleEvent<ScreenReaderMessageStartedEvent>(OnScreenReaderMessageStarted);
		eventHandler.HandleEvent<ScreenReaderMessageEndedEvent>(OnScreenReaderMessageEnded);
	}

	void ScreenReaderLogger::ForceQueueOutput(const std::string& message)
	{
		if(!messageBackLog.empty() && startedMessage && IsSpeaking())
		{
			ScreenReaderMessageEndedEvent srEvent(messageBackLog.front().first, messageBackLog.front().second);
			OnScreenReaderMessageEnded(srEvent);
		}

		//if(!priorityMessageTriggered)
		//{
		//	Stop();
		//}

		Stop();

		messageBackLog.clear();
		startedMessage = false;
		messageBackLog.emplace_back(std::pair(std::string(message), true));
	}

	void ScreenReaderLogger::QueueOutput(const std::string& message, bool shouldInterrupt, bool isPriority)
	{
		//priorityMessageTriggered = isPriority;

		if ((!shouldInterrupt && isPriority) || (isPriority && IsSpeaking()) || startedMessage)
			messageBackLog.emplace_back(std::pair(std::string(message), shouldInterrupt));
		else if (shouldInterrupt && isPriority)
			ForceQueueOutput(message);
		else if (!IsSpeaking())
			OutputAll(message, shouldInterrupt);
	}

	bool ScreenReaderLogger::DetectScreenReader()
	{
		if(Tolk_IsLoaded())
		{
			std::string screenReaderName = GetActiveScreenReaderName();

			if (screenReaderName != "NONE")
			{
				WIZ_INFO("The current active screen reader driver is: {0}", screenReaderName);
				return true;
			}

			WIZ_WARN("No compatible screen readers were detected as running!");
		}
		else
		{
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderLogger::Init before using screen reader functions.");
		}

		return false;
	}

	std::string ScreenReaderLogger::GetActiveScreenReaderName()
	{
		if (Tolk_IsLoaded())
		{
			if (const wchar_t* screenReaderName = Tolk_DetectScreenReader())
			{
				//Converting from wchar_t* to a format supported by the logger, such as std::string
				std::wstring srNameAsWString(screenReaderName);

				std::string srNameAsString(srNameAsWString.begin(), srNameAsWString.end());

				return srNameAsString;
			}
		}
		else
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderLogger::Init before using screen reader functions.");

		return "NONE";
	}

	bool ScreenReaderLogger::QuerySpeechSupport()
	{
		if(Tolk_IsLoaded())
		{
			if (Tolk_HasSpeech())
			{
				WIZ_INFO("The current screen reader driver has speech support.");
				return true;
			}
		}
		else
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderLogger::Init before using screen reader functions.");

		return false;
	}

	bool ScreenReaderLogger::QueryBrailleSupport()
	{
		if (Tolk_IsLoaded())
		{
			if (Tolk_HasBraille())
			{
				WIZ_INFO("The current screen reader driver has braille support.");
				return true;
			}
		}
		else
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderLogger::Init before using screen reader functions.");

		return false;
	}

	bool ScreenReaderLogger::OutputAll(const std::string& message, bool shouldInterrupt)
	{
		std::wstring tolkMessage(message.begin(), message.end());

		return Tolk_Output(tolkMessage.c_str(), shouldInterrupt);
	}

	bool ScreenReaderLogger::OutputSpeech(const std::string& message, bool shouldInterrupt)
	{
		std::wstring tolkMessage(message.begin(), message.end());

		return Tolk_Speak(tolkMessage.c_str(), shouldInterrupt);
	}

	bool ScreenReaderLogger::OutputBraille(const std::string& message)
	{
		std::wstring tolkMessage(message.begin(), message.end());

		return Tolk_Braille(tolkMessage.c_str());
	}

	bool ScreenReaderLogger::IsSpeaking()
	{
		return Tolk_IsSpeaking();
	}

	bool ScreenReaderLogger::OnScreenReaderMessageStarted(ScreenReaderMessageStartedEvent& srEvent)
	{
		//WIZ_TRACE(srEvent);

		return false;
	}

	bool ScreenReaderLogger::OnScreenReaderMessageEnded(ScreenReaderMessageEndedEvent& srEvent)
	{
		//WIZ_TRACE(srEvent);

		return false;
	}
	bool ScreenReaderLogger::OnAppShutdown(AppShutdownEvent& appEvent)
	{
		ForceQueueOutput("Exiting editor application.");

		return true;
	}
}