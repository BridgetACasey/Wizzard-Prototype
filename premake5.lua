workspace "Wizzard"
	architecture "x64"
	startproject "LuggageEditor"

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
IncludeDir["GLFW"] = "WizzardEngine/lib/glfw/include"
IncludeDir["Glad"] = "WizzardEngine/lib/glad/include"
IncludeDir["ImGui"] = "WizzardEngine/lib/imgui"
IncludeDir["ImGuizmo"] = "WizzardEngine/lib/imguizmo"
IncludeDir["GLM"] = "WizzardEngine/lib/glm"
IncludeDir["Tolk"] = "WizzardEngine/lib/tolk/src"
IncludeDir["stb_image"] = "WizzardEngine/lib/stb_image"
IncludeDir["Box2D"] = "WizzardEngine/lib/box2d/include"
IncludeDir["OpenALSoft"] = "WizzardEngine/lib/openal-soft"
IncludeDir["LibOGG"] = "WizzardEngine/lib/libogg/include"
IncludeDir["Vorbis"] = "WizzardEngine/lib/vorbis/include"
IncludeDir["ENTT"] = "WizzardEngine/lib/entt/src"
IncludeDir["YAMLCPP"] = "WizzardEngine/lib/yaml-cpp/include"

include "WizzardEngine/lib/glfw"
include "WizzardEngine/lib/glad"
include "WizzardEngine/lib/imgui"
include "WizzardEngine/lib/imguizmo"
include "WizzardEngine/lib/tolk"
include "WizzardEngine/lib/box2d"
include "WizzardEngine/lib/openal-soft"
include "WizzardEngine/lib/libogg"
include "WizzardEngine/lib/vorbis"
include "WizzardEngine/lib/entt"
include "WizzardEngine/lib/yaml-cpp"

project "WizzardEngine"
	location "WizzardEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	pchheader "wzpch.h"
	pchsource "WizzardEngine/src/wizzard/wzpch.cpp"

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
		"_EXPORTING",
		"YAML_CPP_STATIC_DEFINE"
	}


	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/wizzard",
		"%{prj.name}/lib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
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
		"%{IncludeDir.Vorbis}",
		"%{IncludeDir.ENTT}",
		"%{prj.name}/lib/entt/single_include",
		"%{IncludeDir.YAMLCPP}"
	}

	links
	{
		"opengl32.lib",
		"GLFW",
		"Glad",
		"ImGui",
		"ImGuizmo",
		"Tolk",
		"Box2D",
		"OpenALSoft",
		"Vorbis",
		"YAML-CPP"
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



project "LuggageEditor"
	location "LuggageEditor"
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
		"WizzardEngine/src",
		"WizzardEngine/lib/spdlog/include",
		"WizzardEngine/lib",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Tolk}",
		"%{IncludeDir.ENTT}",
		"WizzardEngine/lib/entt/single_include",
		"%{IncludeDir.YAMLCPP}"
	}

	links
	{
		"WizzardEngine"
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
		"WizzardEngine/src",
		"WizzardEngine/lib/spdlog/include",
		"WizzardEngine/lib",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.Tolk}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.YAMLCPP}"
	}

	links
	{
		"WizzardEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WIZZARD_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			"{COPY} ../WizzardEngine/lib/tolk/libs/x64/**.dll %{cfg.targetdir}"
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