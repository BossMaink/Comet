workspace "Comet"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Comet/third-party/GLFW/include"
IncludeDir["Glad"] = "Comet/third-party/Glad/include"

include "Comet/third-party/GLFW"
include "Comet/third-party/Glad"

project "Comet"
	location "Comet"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/".. outputdir .. "/%{prj.name}")
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")

	pchheader "cmpch.h"
	pchsource "Comet/src/cmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Comet/src",
		"%{prj.name}/third-party/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_ENABLE_ASSERTS",
			"CM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Stylized")
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "CM_Release"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		buildoptions "/MD"
		optimize "On"

project "Stylized"
	location "Stylized"
	kind "ConsoleApp"	
	language "C++"

	targetdir ("bin/".. outputdir .. "/%{prj.name}")
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Comet/third-party/spdlog/include",
		"Comet/src"
	}

	links
	{
		"Comet"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_ENABLE_ASSERTS",
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "CM_Release"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		buildoptions "/MD"
		optimize "On"