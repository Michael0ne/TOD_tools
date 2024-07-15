import os
import re

with open("scripts_list.txt", "r") as f:
    scripts = [ line.strip() for line in f.readlines() ]

for script in scripts:
    filename_without_ext = os.path.splitext(script)[0]
    
    with open("script_placeholder.txt", "r") as f:
        contents = f.read()
    replaced_contents = re.sub(r"##filename##", filename_without_ext, contents)
    
    with open(os.path.join(os.getcwd(), script), "w") as f:
        f.write(replaced_contents)

print("Script files created successfully!")