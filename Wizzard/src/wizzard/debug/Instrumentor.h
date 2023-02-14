#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

namespace Wizzard
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string name;

		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;
		std::thread::id threadID;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	class Instrumentor
	{
	private:
		std::mutex mutex;
		InstrumentationSession* currentSession;
		std::ofstream outputStream;

	public:
		Instrumentor() : currentSession(nullptr)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(mutex);

			if (currentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetWizzardLogger())
				{ // Edge case: BeginSession() might be before Log::Init()
					WIZ_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, currentSession->name);
				}
				InternalEndSession();
			}
			outputStream.open(filepath);

			if (outputStream.is_open())
			{
				currentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetWizzardLogger())
				{ // Edge case: BeginSession() might be before Log::Init()
					WIZ_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.elapsedTime.count()) << ',';
			json << "\"name\":\"" << name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ",";
			json << "\"ts\":" << result.start.count();
			json << "}";

			std::lock_guard lock(mutex);

			if (currentSession)
			{
				outputStream << json.str();
				outputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		void WriteHeader()
		{
			outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			outputStream.flush();
		}

		void WriteFooter()
		{
			outputStream << "]}";
			outputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (currentSession)
			{
				WriteFooter();
				outputStream.close();
				delete currentSession;
				currentSession = nullptr;
			}
		}

	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) : name(name), stopped(false)
		{
			startTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ startTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ name, highResStart, elapsedTime, std::this_thread::get_id() });

			stopped = true;
		}
	private:
		const char* name;
		std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
		bool stopped;
	};
}

#define WIZ_PROFILE 0
#if WIZ_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define WIZ_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define WIZ_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define WIZ_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define WIZ_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define WIZ_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define WIZ_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define WIZ_FUNC_SIG __func__
#else
#define WIZ_FUNC_SIG "WIZ_FUNC_SIG unknown!"
#endif

#define WIZ_PROFILE_BEGIN_SESSION(name, filepath) ::Wizzard::Instrumentor::Get().BeginSession(name, filepath)
#define WIZ_PROFILE_END_SESSION()				  ::Wizzard::Instrumentor::Get().EndSession()
#define WIZ_PROFILE_SCOPE(name)					  ::Wizzard::InstrumentationTimer timer##__LINE__(name);
#define WIZ_PROFILE_FUNCTION() WIZ_PROFILE_SCOPE(WIZ_FUNC_SIG)
#else
#define WIZ_PROFILE_BEGIN_SESSION(name, filepath)
#define WIZ_PROFILE_END_SESSION()
#define WIZ_PROFILE_SCOPE(name)
#define WIZ_PROFILE_FUNCTION()
#endif