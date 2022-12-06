import os, re, json
from pathlib import Path

def extract_used_classes(source_folder, file_path="../patches/used_classes.json"):
    print("Performing search inside: \"" + source_folder + "\"")
    find_class = re.compile(r"(?:#include <godot_cpp\/classes\/)(.*)(?:.hpp>)", re.MULTILINE)

    # some predefined classes may contain "#include <godot_cpp/classes/*>"
    additional_classes = [
        (re.compile(r"#include \"godot_cpp\/templates\/cowdata.hpp\"", re.MULTILINE), ["global_constants"]),
        (re.compile(r"#include \"godot_cpp\/templates\/thread_work_pool.hpp\"", re.MULTILINE), ["os", "semaphore"]),
    ]

    found_classes = set()
    for dir,subdirs,files in os.walk(source_folder):
        for f in files:
            if not f.endswith((".cpp", ".c", ".inc", ".hpp", ".h")):
                continue

            with open(Path(dir) / f, "r") as file:
                data = file.read()

                matches = find_class.finditer(data)
                for matchNum, match in enumerate(matches, start=1):
                    found_classes.add(match.group(1))

                for ac in additional_classes:
                    match = ac[0].search(data)
                    if match:
                        for i in ac[1]:
                            found_classes.add(i)

    found_classes = ["".join([w.title() for w in c.split("_")]) for c in found_classes]

    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(found_classes, f, ensure_ascii=False, indent=4)
