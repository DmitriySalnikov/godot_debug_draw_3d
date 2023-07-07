#!/usr/bin/env python

import os
import re
from pathlib import Path


# This patch does not affect the size of the library, but speeds up the build time.
# Use the argument `exclude_unused_classes=no` to generate all classes
# Use the `folder_to_include_classes=path/` argument to scan classes in a specific directory
is_need_to_exclude_classes = True
sources_folder = ""
found_dependencies = set()
temp_engine_class_names = set()


def setup(_is_exclude: bool, _sources_folder: str):
    global is_need_to_exclude_classes
    global sources_folder
    is_need_to_exclude_classes = _is_exclude
    sources_folder = _sources_folder


def extract_used_classes(folder_path: str):
    folder_path = Path(folder_path).resolve().as_posix()
    godot_cpp_src = Path("src").resolve().as_posix()
    godot_cpp_include = Path("include").resolve().as_posix()

    print()
    print("Performing search for used classes inside these paths:")
    print(folder_path)
    print(godot_cpp_src)
    print(godot_cpp_include)
    print()

    find_class = re.compile(
        r'(?:#include [<\"]godot_cpp\/classes\/)(.*)(?:.hpp[>\"])', re.MULTILINE)

    found_classes = set()

    def scan(scan_path):
        skips = 0
        for dir, subdirs, files in os.walk(scan_path):
            for f in files:
                if not f.endswith((".cpp", ".cxx", ".c++", ".c", ".cc", ".inc", ".hpp", ".hxx", ".h", ".hh")):
                    continue

                def read_data(opened_file):
                    data = file.read()

                    matches = find_class.finditer(data)
                    for matchNum, match in enumerate(matches, start=1):
                        found_classes.add(match.group(1))

                try:
                    with open(Path(dir) / f, "r") as file:
                        read_data(file)
                except UnicodeDecodeError:
                    try:
                        with open(Path(dir) / f, "r", encoding="utf-8") as file:
                            read_data(file)
                    except UnicodeDecodeError as e:
                        print("Skipping file due to 'UnicodeDecodeError' exception: " +
                              (Path(dir) / f).resolve().as_posix() + "\nException: " + str(e))
                        skips += 1
                        continue
        return skips

    skips = 0
    skips += scan(godot_cpp_src)
    skips += scan(godot_cpp_include)
    skips += scan(folder_path)

    if skips > 0:
        print()

    # generate array of class names
    return ["".join([w.title() for w in c.split("_")]) for c in found_classes]


def scan_dependencies(api):
    api = dict(api)
    if not is_need_to_exclude_classes:
        return

    used_classes = extract_used_classes(sources_folder)

    for class_api in api["classes"]:
        # It will change the actual value inside the `api`!!!
        #
        # ClassDB Singleton is renamed in godot-cpp.
        # This class cannot appear as an argument or return value, so no other renaming is required yet.
        if class_api["name"] == "ClassDB":
            class_api["name"] = "ClassDBSingleton"

        temp_engine_class_names.add(class_api["name"])

    for name in used_classes:
        _get_dependencies(api, name)

    print("Provided", len(used_classes),
          "explicit classes:", str(sorted(used_classes)))
    print()
    print("A total of", len(found_dependencies),
          "classes were found:", str(sorted(found_dependencies)))
    print()

    temp_engine_class_names.clear()


def _get_dependencies(api, name):
    def _add_dependency_engine_class(_class, _to_scan):
        for start in ["enum::", "typedarray::", "bitfield::"]:
            if _class.startswith(start):
                _class = _class[len(start):].partition(".")[0]
                break

        if _class in temp_engine_class_names:
            _to_scan.add(_class)

    for class_api in api["classes"]:
        if not (class_api["name"] == name or class_api["name"].lower() == name.lower()):
            continue

        if class_api["name"] == "Object":
            break

        need_to_scan = set()
        need_to_scan.add(class_api["name"])
        need_to_scan.add(class_api["inherits"])

        for method in class_api.get("methods", []):
            if "return_value" in method:
                _add_dependency_engine_class(
                    method["return_value"]["type"], need_to_scan)

            for arg in method.get("arguments", []):
                _add_dependency_engine_class(arg["type"], need_to_scan)

        for c in need_to_scan:
            if c not in found_dependencies:
                found_dependencies.add(c)
                _get_dependencies(api, c)
        break


# from binding_generator.py
def camel_to_snake(name):
    name = re.sub("(.)([A-Z][a-z]+)", r"\1_\2", name)
    name = re.sub("([a-z0-9])([A-Z])", r"\1_\2", name)
    return name.replace("2_D", "2D").replace("3_D", "3D").lower()


def need_to_exclude(name):
    return len(found_dependencies) > 0 and name not in found_dependencies


def delete_useless(files):
    if len(found_dependencies) == 0:
        return files

    # convert found class names to file names
    deps_file_names = [camel_to_snake(c) + ".cpp" for c in found_dependencies]

    src_path = "gen/src/classes/"
    print("These", len(deps_file_names), "files from the",
          src_path, "directory will be compiled:", deps_file_names)
    print()

    new_files_list = []
    for f in files:
        split = f.split(src_path)
        if len(split) > 1:
            if split[1] in deps_file_names:
                new_files_list.append(f)
        else:
            new_files_list.append(f)

    return new_files_list
