workspace "WizzardEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Wizzard"
	location "Wizzard"
	kind "StaticLib"
	language "C++"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	pchheader "wzpch.h"
	pchsource "Wizzard/src/wizzard/wzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src/wizzard",
		"%{prj.name}/lib/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WIZZARD_PLATFORM_WINDOWS",
			"WIZZARD_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../build/" .. outputdir .. "/Application")
		}

	filter "configurations:Debug"
		defines "WIZZARD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "WIZZARD_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "WIZZARD_DISTRIBUTION"
		optimize "On"



project "Application"
	location "Application"
	kind "WindowedApp"
	language "C++"

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
		"Wizzard/lib/spdlog/include"
	}

	links
	{
		"Wizzard"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WIZZARD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "WIZZARD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "WIZZARD_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "WIZZARD_DISTRIBUTION"
		optimize "On"