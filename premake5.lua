workspace "GluttonousSnake"	--解决方案名称

architecture "x64"

configurations
{
	"Debug",
	"Release",
	"Dist"
}

-- 输出路径  只是一个字符串变量 后面使用
outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "GluttonousSnake"
	location "GluttonousSnake"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/"..outputdir.."/%{prj.name}" )
	objdir ("bin-int/"..outputdir.."/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Dependence/include",
		"ImGui/src"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	links
	{
		"glad",
		"opengl32.lib",
		"Dependence/glfw3.lib",
		"ImGui"
		
	}
	
	filter "system:windows"	--过滤器 对于特定的设定
		cppdialect "c++14"
		staticruntime "On"
		systemversion "10.0.22000.0" 
		
		--postbuildcommands	-- 后构建指令  dll使用
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/App")
		--}
	
	filter "configurations:Debug"
		defines "_Debug"
		runtime "Release"
		symbols "On"
		
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "On"
		
	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "On"
		
project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "c++"
	staticruntime "off"
	
	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Dependence/include"
	}
	
	filter "system:windows"	--过滤器 对于特定的设定
		cppdialect "c++14"
		staticruntime "On"
		systemversion "10.0.22000.0" 
	
	filter "configurations:Debug"
		defines "_Debug"
		runtime "Release"
		symbols "On"
		
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "On"
		
	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "On"
		
project "glad"
	location "glad"
	kind "StaticLib"
	language "c++"
	staticruntime "off"
	
	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}
	
	includedirs
	{
		"Dependence/include"
	}
	
	filter "configurations:Debug"
		defines "_Debug"
		runtime "Release"
		symbols "On"
		
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "On"
		
	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "On"