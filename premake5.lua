workspace "WizzardEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Wizzard/lib/glfw/include"
IncludeDir["Glad"] = "Wizzard/lib/glad/include"
IncludeDir["ImGui"] = "Wizzard/lib/imgui"

include "Wizzard/lib/glfw"
include "Wizzard/lib/glad"
include "Wizzard/lib/imgui"

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
		"%{prj.name}/lib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WIZZARD_PLATFORM_WINDOWS",
			"WIZZARD_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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
	kind "ConsoleApp"
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