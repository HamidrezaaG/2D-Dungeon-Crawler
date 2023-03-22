workspace "Game"
    configurations { "Debug", "Release" }
    location "build"
    platforms { "x64" }

		characterset "MBCS"
		startproject "GameTilemap"

project "Framework"
	kind "StaticLib"
	location "build/Framework"

	files {
		"Framework/Source/**.cpp",
		"Framework/Source/**.h",
		"Framework/Libraries/imgui/**.cpp",
		"Framework/Libraries/imgui/**.h",
		"Framework/Libraries/stb/*.h",
		"Framework/Libraries/rapidjson/**.h",
	}
	
	includedirs{
		"Framework/Source",
	}
	
	pchheader "FrameworkPCH.h"
	pchsource "Framework/Source/Core/FWCore.cpp"
	
	filter "files:Framework/Libraries/imgui/*.cpp"
		flags{'NoPCH'}
	filter "files:Framework/Source/Math/*.cpp"
		flags{'NoPCH'}
	filter "files:Framework/Source/Objects/**.cpp"
		flags{'NoPCH'}

project		 "GameTilemap"
    kind	 "WindowedApp"
	location "build/GameTilemap"
	debugdir "GameTilemap"
	
	files {
		"GameTilemap/Source/**.cpp",
		"GameTilemap/Source/**.h",
		"GameTilemap/Data/**.vert",
		"GameTilemap/Data/**.frag",
		"GameTilemap/Data/**.png",
		"GameTilemap/Data/**.json",
		"Readme.txt",
		"BuildProj.bat",
		"premake5.lua",
		".gitignore",
	}
	
	includedirs {
	"GameTilemap/Source/",
	}
	
	pchheader "GamePCH.h"
	pchsource "GameTilemap/Source/WinMain.cpp"
	
	links {
	"Framework",
	"opengl32",
	}


--project		 "Game"
--    kind	 "WindowedApp"
--	location "build/Game"
--	debugdir "Game"
--	
--	files {
--		"Game/Source/**.cpp",
--		"Game/Source/**.h",
--		"Game/Data/**.vert",
--		"Game/Data/**.frag",
--		"BuildProj.bat",
--		"premake5.lua",
--		".gitignore",
--	}
--	
--	includedirs {
--	"Game/Source/",
--	}
--	
--	pchheader "GamePCH.h"
--	pchsource "Game/Source/WinMain.cpp"
--	
--	links {
--	"Framework",
--	"opengl32",
--	}


project "Framework"
	kind "StaticLib"
	location "build/Framework"

	files {
		"Framework/Source/**.cpp",
		"Framework/Source/**.h",
		"Framework/Libraries/imgui/**.cpp",
		"Framework/Libraries/imgui/**.h",
	}
	
	includedirs{
		"Framework/Source",
	}
	

	pchheader "FrameworkPCH.h"
	pchsource "Framework/Source/Core/FWCore.cpp"
	
	filter "files:Framework/Libraries/imgui/*.cpp"
		flags{'NoPCH'}
	filter "files:Framework/Source/Math/*.cpp"
		flags{'NoPCH'}
	filter "files:Framework/Source/Objects/**.cpp"
		flags{'NoPCH'}