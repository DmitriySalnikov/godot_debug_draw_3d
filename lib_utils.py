#!/usr/bin/env python3

from SCons.Script.SConscript import SConsEnvironment
from patches import unity_tools

import SCons
import os, json, re


def get_sources(src: list, src_folder: str, lib_name: str = "unity_"):
    res = [src_folder + "/" + file for file in src]
    res = unity_tools.generate_unity_build(res, lib_name + "_")
    return res


def get_library_object(env: SConsEnvironment, project_name: str, lib_name: str, extra_tags: str, output_path: str, src_folder: str, additional_src: list) -> str:
    env.Append(CPPPATH=src_folder)

    src = []
    with open(src_folder + "/default_sources.json") as f:
        src = json.load(f)

    scons_cache_path = os.environ.get("SCONS_CACHE")
    if scons_cache_path is None:
        # store all obj's in a dedicated folder
        env["SHOBJPREFIX"] = "#obj/"
    else:
        env.CacheDir(scons_cache_path)
        env.Decider("MD5")

    # some additional tags if needed
    additional_tags = ""

    if env["platform"] == "web" and env.get("threads", True):
        additional_tags = ".threads"

    lib_filename = "lib{}.{}.{}.{}{}".format(lib_name, env["platform"], env["target"], env["arch"], additional_tags + extra_tags) + env["SHLIBSUFFIX"]

    if env["platform"] == "macos":
        generate_framework_folder(env, project_name, lib_name, lib_filename, output_path)
        lib_filename = os.path.join(output_path, os.path.splitext(lib_filename)[0] + ".framework", lib_filename)
    else:
        lib_filename = os.path.join(output_path, lib_filename)

    env.Default(
        env.SharedLibrary(
            target=env.File(lib_filename),
            source=get_sources(additional_src + src, src_folder, lib_name)
        )
    )

    return lib_filename


def get_library_version():
    with open("src/version.h", "r") as f:
        header_content = f.read()

    major_match = re.search(r"#define .*_MAJOR (\d+)", header_content)
    minor_match = re.search(r"#define .*_MINOR (\d+)", header_content)
    patch_match = re.search(r"#define .*_PATCH (\d+)", header_content)

    major_value = int(major_match.group(1)) if major_match else 0
    minor_value = int(minor_match.group(1)) if minor_match else 0
    patch_value = int(patch_match.group(1)) if patch_match else 0

    return f"{major_value}.{minor_value}.{patch_value}"


def generate_framework_folder(env: SConsEnvironment, project_name: str, lib_name: str, lib_filename: str, output_path: str):
    min_version = env.get("macos_deployment_target", "10.15")
    lib_version = get_library_version()
    lib_filename_noext = os.path.splitext(lib_filename)[0]

    info_plist = f"""<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>{lib_filename}</string>
	<key>CFBundleName</key>
	<string>{project_name}</string>
	<key>CFBundleDisplayName</key>
	<string>{project_name}</string>
	<key>CFBundleIdentifier</key>
	<string>ru.dmitriysalnikov.{lib_name}</string>
	<key>NSHumanReadableCopyright</key>
	<string>Copyright (c) Dmitriy Salnikov.</string>
	<key>CFBundleVersion</key>
	<string>{lib_version}</string>
	<key>CFBundleShortVersionString</key>
	<string>{lib_version}</string>
	<key>CFBundlePackageType</key>
	<string>FMWK</string>
	<key>CSResourcesFileMapped</key>
	<true/>
	<key>DTPlatformName</key>
	<string>macosx</string>
	<key>LSMinimumSystemVersion</key>
	<string>{min_version}</string>
</dict>
</plist>
    """

    res_folder = os.path.join(output_path, lib_filename_noext + ".framework", "Resources")
    os.makedirs(res_folder, exist_ok=True)

    with open(os.path.join(res_folder, "Info.plist"), "w") as info_plist_file:
        info_plist_file.write(info_plist)


def generate_resources_cpp_h_files(input_files: list, namespace: str, src_folder: str, output_no_ext: str, src_out: list):
    """
    input_files : list of tuples
        (path: str, is_text: bool)
    namespace : str
        source code namespace
    src_folder : str
        source folder in which the 'gen/' folder will be created
    output_no_ext : str
        name of the source pair (cpp/h)
    src_out : list of str
        array which will be extended with generated sources
    """

    print(f"Generating sources '{output_no_ext}.[cpp/h]' with content from resources:")
    print("\n".join([f"{i[0]} as {'text' if i[1] else 'binary'}" for i in input_files]))
    print()

    gen_dir = "gen/"
    out_dir = src_folder + "/" + gen_dir
    os.makedirs(out_dir, exist_ok=True)

    cpp_name = output_no_ext + ".cpp"
    h_name = output_no_ext + ".h"

    src_out.append(gen_dir + cpp_name)

    with open(out_dir + cpp_name, "w+") as cpp_file, open(out_dir + h_name, "w+") as h_file:
        h_file.write("#pragma once\n\n")
        h_file.write("#include <array>\n\n")
        h_file.write(f"namespace {namespace} {{\n")

        cpp_file.write(f'#include "{h_name}"\n\n')
        cpp_file.write(f"namespace {namespace} {{\n")

        for input_file_touple in input_files:
            is_text = input_file_touple[1]
            input_file_path = input_file_touple[0]
            file_name_escaped = input_file_path.replace(".", "_").replace("/", "_").replace("\\", "_").replace(":", "_")

            if is_text:
                try:
                    with open(input_file_path, "r") as file:
                        text_data = file.read()
                except UnicodeDecodeError:
                    try:
                        with open(input_file_path, "r", encoding="utf-8") as file:
                            text_data = file.read()
                    except UnicodeDecodeError as e:
                        print(
                            "Skipping file due to 'UnicodeDecodeError' exception: "
                            + (input_file_path).resolve().as_posix()
                            + "\nException: "
                            + str(e)
                        )
                        continue

                cpp_file.write(f'\tconst char *{file_name_escaped} = R"({text_data})";\n\n')

                h_file.write(f"\textern const char *{file_name_escaped};\n")
            else:
                with open(input_file_touple[0], "rb") as input_file:
                    binary_data = input_file.read()

                cpp_array = ", ".join([f"{byte}" for byte in binary_data])
                cpp_file.write(
                    f"\tconst std::array<unsigned char, {len(binary_data)}> {file_name_escaped} = {{ {cpp_array} }};\n\n"
                )

                h_file.write(f"\textern const std::array<unsigned char, {len(binary_data)}> {file_name_escaped};\n")

        h_file.write("}\n")
        cpp_file.write("}\n")
