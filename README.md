# Total Overdose researching project. (work in progress)

TOD Tools - project aims to deliver working game hook to ease modification abilities (custom scripts, textures, etc.)

# TODO:

* Make hook to work correctly (don't crash the game), since Window class is complete.
* Further look into MemoryAllocatrs and it's subclasses.
* Figure out how GlobalScripts works.
* Further look into InputGameControllers class.
* Complete class placeholders for all game Nodes.
* Figure out how text values are stored, displayed and localised.
* Further look into Blocks class and how blocks are loaded.

# Information

* FILE TYPES:
It looks like all files in 'data/overdose_the_game/overdose/shared' contain just memory dumps of actual game classes like models, sounds, textures and so on. Overdose.main could contain generic models/sounds/textures, used across ALL of the maps and files like .map, .submap, .mission and such. Also, since these files are just memory dumps, they might contain references (or implementations) of script functions. And by looks of it, game reads these files like 'blocks' with fixed size and puts them somewhere in memory.
* SCRIPTS:
Scripts used all across game for everything. The file 'database.bin' looks like a table of global variables and functions whose later used by game and loaded on startup. PS2 version contains 'scriptresource.bin' with currently unknown purpose.
