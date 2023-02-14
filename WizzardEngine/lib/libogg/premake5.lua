project "LibOGG"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"include"
	}

	files
	{
		"include/**.h",
		"src/**.c"
	}

	defines
	{
		"LIBOGG_EXPORTS"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WIN32",
			"NDEBUG",
			"_WINDOWS",
			"_USRDLL"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"