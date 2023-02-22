//@BridgetACasey

#include "wzpch.h"

#include "ScreenReaderSupport.h"

#include <comdef.h>
#include "Tolk.h"

namespace Wizzard
{
	void ScreenReaderSupport::Init()
	{
		if (Tolk_IsLoaded())
		{
			WIZ_WARN("Calling ScreenReaderSupport::Init when Tolk is already initialised! Ignoring process.");
			return;
		}

		Tolk_TrySAPI(preferSAPI);	//Calling this before Tolk_Load for best performance

		//Tolk will also initialise COM if it has not been initialized on the calling thread
		Tolk_Load();

		if (Tolk_IsLoaded())
		{
			WIZ_INFO("Initialised Tolk Screen Reader Abstraction Library.");

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
		}
		else
			WIZ_ERROR("Failed to initialise Tolk Screen Reader Abstraction Library!");
	}

	void ScreenReaderSupport::Shutdown()
	{
		if (Tolk_IsLoaded())
		{
			WIZ_TRACE("Unloading Tolk Screen Reader Abstraction Library...");
			Tolk_Unload();
		}
		else
			WIZ_ERROR("Trying to disable Screen Reader Support and unload Tolk multiple times!");
	}

	bool ScreenReaderSupport::Stop()
	{
		return Tolk_Silence();
	}

	bool ScreenReaderSupport::DetectScreenReader()
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
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderSupport::Init before using screen reader functions.");
		}

		return false;
	}

	std::string ScreenReaderSupport::GetActiveScreenReaderName()
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
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderSupport::Init before using screen reader functions.");

		return "NONE";
	}

	bool ScreenReaderSupport::QuerySpeechSupport()
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
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderSupport::Init before using screen reader functions.");

		return false;
	}

	bool ScreenReaderSupport::QueryBrailleSupport()
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
			WIZ_ERROR("Tolk library not loaded! Call ScreenReaderSupport::Init before using screen reader functions.");

		return false;
	}

	bool ScreenReaderSupport::OutputAll(const std::string& message, bool shouldInterrupt)
	{
		std::wstring tolkMessage(message.begin(), message.end());

		return Tolk_Output(tolkMessage.c_str(), shouldInterrupt);
	}

	bool ScreenReaderSupport::OutputSpeech(const std::string& message, bool shouldInterrupt)
	{
		std::wstring tolkMessage(message.begin(), message.end());

		return Tolk_Speak(tolkMessage.c_str(), shouldInterrupt);
	}

	bool ScreenReaderSupport::OutputBraille(const std::string& message)
	{
		std::wstring tolkMessage(message.begin(), message.end());

		return Tolk_Braille(tolkMessage.c_str());
	}

	bool ScreenReaderSupport::IsSpeaking()
	{
		return Tolk_IsSpeaking();
	}
}