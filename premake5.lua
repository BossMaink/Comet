workspace "Comet"
	architecture "x64"
	startproject "Stylized"
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
IncludeDir["ImGui"] = "Comet/third-party/ImGui"
IncludeDir["glm"] = "Comet/third-party/glm"

group "Dependencies"
	include "Comet/third-party/GLFW"
	include "Comet/third-party/Glad"
	include "Comet/third-party/ImGui"

group ""

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
		"%{prj.name}/third-party/glm/glm/**.hpp",
		"%{prj.name}/third-party/glm/glm/**.inl"
	}

	includedirs
	{
		"Comet/src",
		"%{prj.name}/third-party/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_ENABLE_ASSERTS",
			"CM_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Stylized\"")
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "CM_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
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
		"Comet/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Comet"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_ENABLE_ASSERTS",
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "CM_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
		optimize "On"