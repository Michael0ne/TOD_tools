# TOD Tools

## Summary

TOD Tools - this is a reverse engineered engine and game code project for Total Overdose and possibly, other games that use this version of Kapow Engine.
Original game & engine are by Deadline Games. You MUST own a game copy to use these tools.

There are 3 'subprojects' you can see:

* TOD1 - reverse engineered game and engine;
* debugmenu - everything for debugging an engine;
* restools - a tool to "view and sometimes extract" some game assets.

## Current state

After compilation using 'Release DLL' or 'Debug DLL' you get 'dinput8.dll' file that can be put against game EXE without touching original game code - this makes possible to call original game functions inside your plugin or inside this DLL.
When compiling using 'Release EXE' or 'Debug EXE' you get 'TOD1.exe' file that's intent is to replace actual game EXE and be used instead.

# Q/A

* Is this project a cheat for the game of some sort?
  * No, the goal of this project is completely different, see summary. For 'how to use cheats' see wiki page.
* Are there any tools to extract game assets?
  * To convert and unpack 'NAZ' archives use 'naztozip' tool by 'CTPAX-X Team' (included in this project). Once unpacked, a 'restools' program can be used to read and extract some assets. Right now, there is ability to read '.main' files that are assets blocks essentially, and extract assets contained.
* Is this modified RenderWare?
  * No, this is completely different engine.
* Can it be built for Linux/PS2/Xbox/etc.?
  * Of course! See the appropriate section of this README for more information.

# Current TODO list (important at top):

* Implementation for MemoryAllocators classes.
* Implementation for AssetBlocks.
* Implementation for loading scripts.
* Implementation for loading resources natively.
* Implementation for rendering with DirectX 9.
* Implementation for game logic.
* Much much more, see 'TODO' scattered all around the source code.

# Building

## Dependencies

* DirectX 9 SDK (set your environment variable 'DXSDK_DIR' to point to SDK directory). (deprecated and will be removed in near future)
* DirectXMath (included).
* libvorbis 1.1.0 (included).
* libogg 1.1.2 (included).
* libpng 1.0.5 (included).

Included libraries can be found under 'TOD_tools\ThirdParty'.

## Building for Windows

I use Visual Studio 2022 (17.0.0) to build this project for Windows.
It's always a good idea to re-compile provided third party libraries under your specific machine and link against them when compiling the rest.
After the build a batch file is executed to copy output file into game directory (updateHookFile.bat), you should edit this batch file to replace game path to where your actual game is. Please, do not rely on this batch file too much, because it will be removed in near future, once cmake/premake project generation is implemented.

## Building for Linux/PS2/etc...

If you want to build for Linux/PS2/Xbox/PS5/etc, then you can't do that right away without tinkering with code. As stated in comments (see contibuting section below), a good idea would be to wrap platform-specific code with ifdef's 'PLATFORM_<platform>'.

Check these classes: 'File', 'Window', 'GfxInternal' ('GfxInternal_Dx9' is an abstraction for DirectX 9 code, so use 'GfxInternal_OGL' for OpenGL related stuff).

The provided files only include Visual Studio-specific "solution" file that is used to build the project. In some near future, it will be replaced with premake/cmake configuration files to build project for your build environment.

# Contributing

Check 'stdafx' header file for comments about how to write code.
Also, join my discord server if you have any questions or suggestions: https://discord.gg/RvUpHDVTct

# License

All third party libraries come with their license included in appropriate header files, I do not own any rights to these libraries.
I do not own any copyrights to original game code, this project code is not 100% accurate but aims to be so.
