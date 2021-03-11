# TOD Tools

## Summary

TOD Tools - this is a reverse engineered engine and game code project for Total Overdose and possibly, other games that use this version of Kapow Engine.
Original game & engine are by Eidos interactive. You MUST to own a game copy to use these tools.

There are 3 'subprojects' you can see:

* TOD1 - reverse engineered game and engine;
* debugmenu - everything for debugging an engine;
* restools - a tool to "view and sometimes extract" some game assets.

## Current state

After compilation using 'Release/Debug DLL' you get 'dinput8.dll' file that can be put against game EXE without touching original game code - this makes possible to call original game functions inside your plugin or inside this DLL.
When compiling using 'Release/Debug EXE' you get 'TOD1.exe' file that's intent is to replace actual game EXE and be used instead.

# Q/A

* Is this project a cheat for the game of some sort?
  * No, the goal of this project is completely different, see summary. For 'how to use cheats' see wiki page.
* Are there any tools to extract game assets?
  * To convert and unpack 'NAZ' archives use 'naztozip' tool by 'CTPAX-X Team'. To read information about already extracted assets (that are supported by the tool), use 'restools' found in appropriate folder.
* Is this modified RenderWare?
  * No, this is completely different engine.

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

* DirectX 9 SDK (set your environment variable 'DXSDK_DIR' to point to SDK directory).
* libvorbis 1.1.0 (included).
* libogg 1.1.2 (included).
* libpng 1.0.5 (included).
* bink (headers included).

Included libraries can be found under 'TOD_tools\ThirdParty'.

## Process for Windows

I use Visual Studio 2019 (16.9.1) to build this project.
It's always a good idea to re-compile provided third party libraries under your specific machine and link against them when compiling the rest.
If you're using 'Release DLL' or 'Debug DLL' build configuration, then after successfull build a batch file is executed to copy output file into game directory (updateHookFile.bat), you should edit this file to replace game path to where your actual game is.

## Process for Linux/PS2/etc...

If you want to build for Linux/PS2/coffepot/etc, then you can't do that right away without tinkering with code. As stated in comments (see contibuting section below), a good idea would be to wrap platform-specific code around 'PLATFORM_<platform>'.

Check these classes: 'File', 'Window', 'GfxInternal' ('GfxInternal_Dx9' is an abstraction for DirectX 9 code, so good idea is to create 'GfxInternal_OpenGL' and do stuff there).

As for project itself - right now I'm using Visual Studio to build code, but see no problem to use premake/cmake/etc to generate project for specific target.

# Contributing

Check 'stdafx' header file for comments about how to write code.

# License

All third party libraries come with their license included in appropriate header files, I do not own any rights to these libraries.
