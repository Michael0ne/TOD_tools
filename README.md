# TOD Tools

## Summary

TOD Tools - this is a reverse engineered engine and game code project for Total Overdose and possibly, other games that use this version of Kapow Engine.
Original game & engine are by Eidos interactive. You NEED to own a game copy to use this tools.

## Current state

Currently, after compilation you get 'dinput8' DLL, that will be put into game's directory (the hooking method).
In future, after skeleton of the engine is done and all basic functions are done, this will become an actual EXE file after compilation that can be ran.

# Q/A

* Is this project a cheat for the game of some sort?
  * No, the goal of this project is completely different, see summary. For 'how to use cheats' see wiki page.
* Are there any tools to extract game assets?
  * There is a 'naztozip' tool by 'CTPAX-X Team' to convert 'naz' archives into zip archives. I have a tool to read game assets, but it's not ready for the public yet.
* Is this modified RenderWare?
  * No, this is completely different engine.

# Current TODO list (important at top):

* Implementation for MemoryAllocators classes.
* Implementation for AssetBlocks.
* Much much more, see 'TODO' scattered all around the source code.

# Building

## Dependencies

* DirectX 9 SDK (set your environment variable 'DXSDK_DIR' to point to SDK directory).
* libvorbis 1.1.0 (included).
* libogg 1.1.2 (included).
* libpng 1.0.5 (included).
* bink (headers included).

Included libraries can be found under 'TOD_tools\ThirdParty'.

## Process

I use Visual Studio 2019 (16.8.5) to build this project. Before compiling and linking, make sure you build included third party libraries.
If any errors occure - check project settings, make sure you use 'Debug' build configuration.
After build succeeded a batch file is executed to copy output file into game directory (updateHookFile.bat), so make sure to replace game path to where your actual game is.

# Contributing

Check 'stdafx' header file for comments about how to write code.
