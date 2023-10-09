project "ATMega328-Emulator-Tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
        "include/**.h",
        "src/**.cpp",
    }

	includedirs {
        "include",
		"%{IncludeDir.ATMega328EmulatorCore}",
    }

	links {
        "ATMega328-Emulator-Core"
    }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "KOM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KOM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "KOM_DIST"
		runtime "Release"
		optimize "on"
