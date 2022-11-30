import os, re, json
from pathlib import Path

def extract_used_classes(source_folder, file_path="../patches/used_classes.json"):
    print("Performing search inside " + source_folder)
    find_class = re.compile(r"(?:#include <godot_cpp\/classes\/)(.*)(?:.hpp>)", re.MULTILINE)

    found_classes = []
    for dir,subdirs,files in os.walk(source_folder):
        for f in files:
            if not f.endswith((".cpp", ".c", ".inc", ".hpp", ".h")):
                continue

            with open(Path(dir) / f, "r") as file:
                data = file.read()
                matches = find_class.finditer(data)
                
                for matchNum, match in enumerate(matches, start=1):
                    if match.group(1) not in found_classes:
                        found_classes.append(match.group(1))

    found_classes = ["".join([w.title() for w in c.split("_")]) for c in found_classes]

    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(found_classes, f, ensure_ascii=False, indent=4)
