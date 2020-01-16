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
