# Total Overdose researching project. (work in progress)

This project includes:
* working hook for the game with basic classes, got by researching game's exe. w.i.p.
* tools to research game's files - currently only 'database.bin' scripts database decoder is present, w.i.p.

I'm using IDA Pro with HexRays decompiler and a lot of patience.
If you would like to help - get in touch!

# TODO:

* Finish "KapowWindow" class since it's easiest one.
* Begin to look into "MemoryAllocators" since it's the heaviest part.
* Begin to look into scripting engine (game uses unknown-for-me TNT script language), entry point could be to figure out how game processes 'scriptdatabase.bin' file. Decompiled version is now present.
* Since game resources are 'blocks' (fragments), write tool to see information about that kind of files.
* ???

# Information

* Regarding file types:
It looks like all files in 'data/overdose_the_game/overdose/shared' contain just memory dumps of actual game classes like models, sounds, textures and so on. Overdose.main could contain generic models/sounds/textures, used across ALL of the maps and files like .map, .submap, .mission and such contain specific models/sounds/textures. Also, since these files are just memory dumps, they might contain references (or implementations) of script function. And by looks of it, game reads these files like 'blocks' with fixed size and puts them somewhere in memory.
* Regarding scripts:
Scripts used all across game for everything. The file 'database.bin' looks like a table of all 'variables' used by game and loaded on startup. PS2 version contains 'scriptresource.bin' with currently unknown purpose.
