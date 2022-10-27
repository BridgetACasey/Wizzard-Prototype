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
IncludeDir["GLM"] = "Wizzard/lib/glm"

include "Wizzard/lib/glfw"
include "Wizzard/lib/glad"
include "Wizzard/lib/imgui"

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
		"%{prj.name}/lib/glm/glm/**.hpp",
		"%{prj.name}/lib/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}


	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/wizzard",
		"%{prj.name}/lib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}"
	}

	links
	{
		"opengl32.lib",
		"GLFW",
		"Glad",
		"ImGui"
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
		"%{IncludeDir.GLM}"
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

	filter "configurations:Debug"
		defines "WIZZARD_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "WIZZARD_RELEASE"
		optimize "on"

	filter "configurations:Distribution"
		defines "WIZZARD_DISTRIBUTION"
		optimize "on"