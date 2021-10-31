workspace "Comet"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Comet"
	location "Comet"
	kind "SharedLib"
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
		"Comet/src",
		"%{prj.name}/third-party/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Stylized")
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "CM_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
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
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "CM_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		optimize "On"