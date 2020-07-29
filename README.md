# TOD Tools

TOD Tools - (Total Overdose Tools) - project goal is to make game modifiable with ease.

# Current TODO list (important at top):

* MemoryAllocators class needs to be implemented correctly.
* Config class needs to be fixed.
* How text values (localization) are loaded?
* How resources are loaded?
* Project has to compile successfully!

# Information

* FILE TYPES:
It looks like all files in 'data/overdose_the_game/overdose/shared' contain just memory dumps of actual game classes like models, sounds, textures and so on. Overdose.main could contain generic models/sounds/textures, used across ALL of the maps and files like .map, .submap, .mission and such. Also, since these files are just memory dumps, they might contain references (or implementations) of script functions. And by looks of it, game reads these files like 'blocks' with fixed size and puts them somewhere in memory.
* SCRIPTS:
Scripts used all across game for everything. The file 'database.bin' looks like a table of global variables and functions whose later used by game and loaded on startup. PS2 version contains 'scriptresource.bin' with currently unknown purpose.
