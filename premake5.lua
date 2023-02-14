workspace "WizzardEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Wizzard/lib/glfw/include"
IncludeDir["Glad"] = "Wizzard/lib/glad/include"
IncludeDir["ImGui"] = "Wizzard/lib/imgui"
IncludeDir["GLM"] = "Wizzard/lib/glm"
IncludeDir["Tolk"] = "Wizzard/lib/tolk/src"
IncludeDir["stb_image"] = "Wizzard/lib/stb_image"
IncludeDir["Box2D"] = "Wizzard/lib/box2d/include"
IncludeDir["OpenALSoft"] = "Wizzard/lib/openal-soft"
IncludeDir["LibOGG"] = "Wizzard/lib/libogg/include"
IncludeDir["Vorbis"] = "Wizzard/lib/vorbis/include"

include "Wizzard/lib/glfw"
include "Wizzard/lib/glad"
include "Wizzard/lib/imgui"
include "Wizzard/lib/tolk"
include "Wizzard/lib/box2d"
include "Wizzard/lib/openal-soft"
include "Wizzard/lib/libogg"
include "Wizzard/lib/vorbis"

project "Wizzard"
	location "Wizzard"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	pchheader "wzpch.h"
	pchsource "Wizzard/src/wizzard/wzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/lib/stb_image/**.h",
		"%{prj.name}/lib/stb_image/**.cpp",
		"%{prj.name}/lib/glm/glm/**.hpp",
		"%{prj.name}/lib/glm/glm/**.inl",
		"%{prj.name}/lib/minimp3/minimp3.h",
		"%{prj.name}/lib/minimp3/minimp3_ex.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"AL_LIBTYPE_STATIC",
		"RESTRICT=__restrict",
		"_EXPORTING"
	}


	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/wizzard",
		"%{prj.name}/lib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Tolk}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.OpenALSoft}",
		"%{prj.name}/lib/openal-soft/include",
		"%{prj.name}/lib/openal-soft/include/AL",
		"%{prj.name}/lib/openal-soft/common",
		"%{prj.name}/lib/openal-soft/core",
		"%{prj.name}/lib/openal-soft/al",
		"%{prj.name}/lib/openal-soft/alc",
		"%{prj.name}/lib/openal-soft/examples/common",
		"%{prj.name}/lib/minimp3",
		"%{IncludeDir.LibOGG}",
		"%{IncludeDir.Vorbis}"
	}

	links
	{
		"opengl32.lib",
		"GLFW",
		"Glad",
		"ImGui",
		"Tolk",
		"Box2D",
		"OpenALSoft",
		"Vorbis"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WIZZARD_PLATFORM_WINDOWS",
			"WIZZARD_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "WIZZARD_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "WIZZARD_RELEASE"
		optimize "on"

	filter "configurations:Distribution"
		defines "WIZZARD_DISTRIBUTION"
		optimize "on"



project "Application"
	location "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Wizzard/src",
		"Wizzard/lib/spdlog/include",
		"Wizzard/lib",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Tolk}",
		"%{IncludeDir.Box2D}"
	}

	links
	{
		"Wizzard"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WIZZARD_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			"{COPY} ../Wizzard/lib/tolk/libs/x64/**.dll %{cfg.targetdir}"
		}

	filter "configurations:Debug"
		defines "WIZZARD_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "WIZZARD_RELEASE"
		optimize "on"

	filter "configurations:Distribution"
		defines "WIZZARD_DISTRIBUTION"
		optimize "on"