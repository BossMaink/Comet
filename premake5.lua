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
IncludeDir["stb_image"] = "Comet/third-party/stb_image"

group "Dependencies"
	include "Comet/third-party/GLFW"
	include "Comet/third-party/Glad"
	include "Comet/third-party/ImGui"

group ""

project "Comet"
	location "Comet"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/".. outputdir .. "/%{prj.name}")
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")

	pchheader "cmpch.h"
	pchsource "Comet/src/cmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/third-party/stb_image/**.h",
		"%{prj.name}/third-party/stb_image/**.cpp",
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_ENABLE_ASSERTS",
			"CM_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
		}

		-- postbuildcommands
		-- {
		-- 	("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Stylized\"")
		-- }

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "CM_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
		optimize "on"

project "Stylized"
	location "Stylized"
	kind "ConsoleApp"	
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Comet/third-party",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Comet"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_ENABLE_ASSERTS",
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "CM_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
		optimize "on"