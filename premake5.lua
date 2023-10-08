include "./vendor/premake/premake_customization/solution_items.lua"

workspace "ATMega328 Emulator"
    architecture "x86_64"
    startproject "ATMega328 Emulator"

    configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

    solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ATMega328 Emulator"
