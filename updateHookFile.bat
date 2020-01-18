@echo off
set PATH=%~1
set OUTNAME=%~2
set PDBNAME=%~3
set MAPNAME=%~4
set DEST_PATH=C:\Program Files\Eidos\Total Overdose\
copy /b "%PATH%%OUTNAME%" "%DEST_PATH%%OUTNAME%"
copy /b "%PATH%%PDBNAME%" "%DEST_PATH%%PDBNAME%"
copy /b "%PATH%%MAPNAME%" "%DEST_PATH%%MAPNAME%"