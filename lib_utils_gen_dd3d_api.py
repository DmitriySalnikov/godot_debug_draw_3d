#!/usr/bin/env python3

# There are several macros here that you need to use to mark functions, classes, and enums for adding them to the Native API.
#
# NAPI_CLASS_REF - before the keyword "class" for a RefCounted, e.g.: NAPI_CLASS_REF class DebugDraw3DConfig
# NAPI_CLASS_SINGLETON - before the keyword "class" for a singleton, e.g.: NAPI_CLASS_SINGLETON class DebugDraw3D
# NAPI_ENUM - before the keyword “enum” for enums with explicit constant types, e.g.: NAPI_ENUM enum PointType : uint32_t
#   It is not necessary to explicitly specify enum values; the generator will attempt to substitute them automatically.
# NAPI - used to mark functions.
# NSELF_RETURN - should be used immediately after "NAPI" instead of the return type "void",
#   and the function name should end with "_selfreturn", for example: NAPI NSELF_RETURN set_thickness_selfreturn(real_t _value)
#
# Arguments of the function with Ref<godot::T> or godot::Ref<godot::T> is used for Godot types
# and Ref<T> is for DD3D classes
#
# All Packed*Arrays are not available in the Native API. StringName, NodePath, Array and Dictionary too.
# If you need to pass an array to the Native API, then you need to:
#   1) add the suffix "_c" to the functions,
#   2) replace the argument with the supported array type with two separate arguments with the same name
#       but different suffixes ("_data" and "_size") and pointer and "uint64_t" types.
#   Example before:
#       static void draw_lines(const godot::PackedVector3Array &lines, const real_t &duration = 0)
#   after:
#       NAPI static void draw_lines_c(const godot::Vector3 * lines_data, const uint64_t &lines_size, const real_t &duration = 0)
# If you need to pass a string to the Native API,
#   then you also need to add the suffix "_c" and replace "String" with "char*" and add the suffix "_string" to the argument name.
#   Example before:
#       void set_text(godot::String key, real_t duration = -1)
#   after:
#       NAPI void set_text_c(const char* key_string, real_t duration = -1)
#   In this case, the string will be converted to a utf8 char array.


from SCons.Script.SConscript import SConsEnvironment

import os, shutil, json, re, hashlib, copy
import lib_utils


def dev_print(line: str):
    if False:
        print(line)


default_colors_map = {
    "Colors::empty_color": "godot::Color(0, 0, 0, 0)",
    "Colors::white_smoke": "godot::Color(0.96f, 0.96f, 0.96f, 1.0f)",
}

allowed_enum_types = [
    "char",
    "short",
    "int",
    "uint8_t",
    "uint16_t",
    "uint32_t",
    "uint64_t",
    "int8_t",
    "int16_t",
    "int32_t",
    "int64_t",
]

basic_godot_types = [
    "godot::Vector2",
    "godot::Vector2i",
    "godot::Rect2",
    "godot::Rect2i",
    "godot::Vector3",
    "godot::Vector3i",
    "godot::Transform2D",
    "godot::Vector4",
    "godot::Vector4i",
    "godot::Plane",
    "godot::Quaternion",
    "godot::AABB",
    "godot::Basis",
    "godot::Transform3D",
    "godot::Projection",
    "godot::Color",
    #
    # Restricted
    "godot::Variant",
    "godot::RID",
    "godot::ObjectID",
    "godot::Callable",
    "godot::Signal",
    "godot::Dictionary",
    "godot::Array",
]

disallowed_godot_types = [
    "godot::Variant",
    "godot::RID",
    "godot::ObjectID",
    "godot::Callable",
    "godot::Signal",
    "godot::Dictionary",
    "godot::Array",
]

basic_types_to_arrays = {
    "char": "godot::String",
    "uint8_t": "godot::PackedByteArray",
    "int32_t": "godot::PackedInt32Array",
    "int64_t": "godot::PackedInt64Array",
    "float": "godot::PackedFloat32Array",
    "double": "godot::PackedFloat64Array",
    "godot::Vector2": "godot::PackedVector2Array",
    "godot::Vector3": "godot::PackedVector3Array",
    "godot::Color": "godot::PackedColorArray",
    "godot::Vector4": "godot::PackedVector4Array",
}

godot_array_types = [
    "StringName",
    "NodePath",
    "PackedByteArray",
    "PackedInt32Array",
    "PackedInt64Array",
    "PackedFloat32Array",
    "PackedFloat64Array",
    "PackedStringArray",
    "PackedVector2Array",
    "PackedVector3Array",
    "PackedColorArray",
    "PackedVector4Array",
    "String",
]


class DefineContext:
    default_defines: set = []
    active_defines: set = []
    if_blocks: list = []

    def __init__(self, env: SConsEnvironment):
        # use only defines, not macros
        self.default_defines = [
            d for d in env["CPPDEFINES"] if (re.search(r"\w+", d)[0] == d if type(d) is not tuple else False)
        ]
        self.active_defines = set(self.default_defines)
        dev_print(f"Global defines: {self.default_defines}")

    def reset(self):
        self.active_defines = set(self.default_defines)
        if_blocks: list = []

    def parse_line(self, line: str) -> bool:
        stripped_line: str = line.strip()
        if stripped_line.startswith("#define "):
            def_line = line[len("#define ") :]
            split = def_line.split(" ")
            if def_line == re.search(r"\w+", split[0])[0]:
                dev_print(f"\tFound define: {def_line}")
                self.active_defines.add(def_line)
            return False

        # TODO: support #if
        # TODO: support #elif
        if stripped_line.startswith("#ifdef ") or stripped_line.startswith("#ifndef "):
            is_inverted = stripped_line.startswith("#ifndef ")
            if_name = stripped_line[: stripped_line.find(" ")]
            define_name = stripped_line[stripped_line.find(" ") + 1 :]

            is_defined = define_name in self.active_defines
            self.if_blocks.append(not is_defined if is_inverted else is_defined)
            if self.if_blocks[-1]:
                dev_print(
                    f"\tFound {if_name} with '{define_name}', which is {'active' if is_defined else 'inactive'}: next lines will be parsed."
                )
            else:
                dev_print(
                    f"\tFound {if_name} with '{define_name}', which is {'active' if is_defined else 'inactive'}: next lines will be ignored."
                )
            return False

        if stripped_line.startswith("#if "):
            self.if_blocks.append("ignored")
            dev_print("\tFound #if. Ignoring...")
            return False

        if stripped_line.startswith("#else"):
            if self.if_blocks[-1] == "ignored":
                return False

            self.if_blocks[-1] = not self.if_blocks[-1]
            dev_print(
                f"\tFound #else: inverting the last if block: {'next lines will be parsed.' if self.if_blocks[-1] else 'next lines will be ignored.'}"
            )
            return False

        if stripped_line.startswith("#endif"):
            self.if_blocks.pop()
            dev_print(f"\tFound #endif: removing the last if block.")
            if len(self.if_blocks):
                if self.if_blocks[-1] != "ignored":
                    dev_print(
                        "\t\tNext lines will be parsed." if self.if_blocks[-1] else "\t\tNext lines will be ignored."
                    )
            return False

        if len(self.if_blocks):
            val = self.if_blocks[-1]
            if val == "ignored":
                return True
            return val

        return True


def insert_lines_at_mark(lines: list, mark: str, insert_lines: list):
    insert_index = -1
    for idx, line in enumerate(lines):
        if line.endswith(mark):
            insert_index = idx
            break
    lines.pop(insert_index)
    lines[insert_index:insert_index] = insert_lines


def split_text_into_lines(text) -> list:
    return text.replace("\r\n", "\n").replace("\r", "\n").split("\n")


####################################
####################################


## api.json


####################################
####################################
def get_api_functions(env: SConsEnvironment, headers: list) -> dict:
    classes = {}
    def_ctx: DefineContext = DefineContext(env)

    for header in headers:
        print(f"  Parsing {header} ...")
        lines = split_text_into_lines(lib_utils.read_all_text(header))
        lines = [line.strip() for line in lines]
        def_ctx.reset()

        functions = []
        enums = {}
        current_class = ""

        is_singleton = False
        is_refcounted = False

        for idx, line in enumerate(lines):
            if not def_ctx.parse_line(line):
                dev_print(f"\t\tSkipping: {idx + 1}: {line}")
                continue

            def get_doc_lines(start_idx: int):
                found_docs = []
                if lines[start_idx - 1] == "*/":
                    doc_idx = start_idx - 1
                    while lines[doc_idx] != "/**" and doc_idx >= 0:
                        doc_idx -= 1
                    found_docs = [
                        line.replace("*", "", 1).strip() if line.startswith("*") else line
                        for line in lines[doc_idx + 1 : start_idx - 1]
                    ]
                    # replaces images with placeholder
                    found_docs = [re.sub(r"(\!\[.*\]\(.*\))", "[THERE WAS AN IMAGE]", line) for line in found_docs]
                return found_docs

            def search_for_docs_override(start_idx: int):
                new_docs = []
                cur_idx = start_idx - 1
                while (lines[cur_idx].startswith("//") or len(lines[cur_idx]) == 0) and cur_idx >= 0:
                    match = re.search(r"\#docs_func (\w*)", lines[cur_idx])
                    if match:
                        override_func = match.group(1)
                        for o_idx, l in enumerate(lines):
                            o_match = re.search(f" {override_func}\\(", l)
                            if o_match:
                                new_docs = get_doc_lines(o_idx)
                                break
                        break
                    cur_idx -= 1
                return new_docs

            docs = get_doc_lines(idx)

            class_prefixes = ["NAPI_CLASS_REF class ", "NAPI_CLASS_SINGLETON class "]
            class_prefix = ""
            for p in class_prefixes:
                if line.startswith(p):
                    class_prefix = p
                    break
            if len(class_prefix):
                class_type = "regular"

                if line.startswith("NAPI_CLASS_SINGLETON "):
                    is_singleton = True
                    class_type = "singleton"
                elif line.startswith("NAPI_CLASS_REF "):
                    is_refcounted = True
                    class_type = "refcounted"
                current_class = re.search(r"\w+", line[len(class_prefix) :])[0].strip()

                functions = []
                enums = {}
                classes[current_class] = {"enums": enums, "functions": functions, "type": class_type, "docs": docs}

                if is_singleton:
                    print(f"Found Singleton {current_class}")
                elif is_refcounted:
                    print(f"Found RefCounted {current_class}")
                continue

            """
            Enum fields:
                "type": str
                "values": dict
            """

            if line.startswith("NAPI_ENUM enum "):
                split = line[len("NAPI_ENUM enum ") : line.find("{")].split(":")
                name = split[0].strip()
                e_type = split[1].strip()
                if e_type not in allowed_enum_types:
                    raise Exception(f'Enum must be one of these types: {allowed_enum_types}, but "{e_type}" was found.')

                print(f"\tFound Enum {name} of type {e_type}")
                enum_lines = [line[line.rfind("{") + 1 :]]
                eidx = idx + 1
                while True:
                    el = lines[eidx]
                    bpos = el.rfind("}")
                    if bpos != -1:
                        el = el[:bpos]
                    enum_lines.append(el)

                    if bpos != -1:
                        break
                    eidx += 1

                # remove comments and newlines
                enum_lines = [
                    (l[: l.rfind("//")] if l.rfind("//") != -1 else l).strip() for l in enum_lines if len(l.strip())
                ]

                enum_lines = "".join(enum_lines)
                # split by comma and remove empty
                enum_lines = [l.strip() for l in enum_lines.split(",") if len(l.strip())]
                print("\t\tEnum lines:", enum_lines)

                values = {}
                last_val = -1
                for e in enum_lines:
                    split = e.split("=")
                    key = split[0].strip()

                    last_val += 1
                    value = last_val
                    if len(split) == 2:
                        value = split[1].strip()
                        if value.isdigit():
                            last_val = int(value)

                    values[key] = str(value)

                enums[name] = {"type": e_type, "values": values}
                continue

            """
            Function fields:
                "name": str - original name
                "c_name": str - C API name
                "docs": list
                "args": list
                "return": dict
                "self_return": bool
                "arg_arrays" (opt): list
            """

            if line.startswith("NAPI "):

                def check_array_usage(type_to_check: str, is_return: bool = False):
                    for arr_t in godot_array_types:
                        if arr_t in type_to_check:
                            if not is_return:
                                raise Exception(
                                    f"Arguments with array types ({arr_t}) must be exposed using a pointer and size, "
                                    + 'e.g. "const godot::Vector3 *lines_data, const uint64_t lines_size" (suffixes "_data" and "_size" are required).'
                                )
                            else:
                                raise Exception(f"Returning array types ({arr_t}) is not supported.")

                def check_argument_types(arg_dict: dict):
                    for basic_t in basic_godot_types:
                        words = re.findall(r"\w+", arg_dict["type"])

                        if basic_t in arg_dict["type"]:
                            if basic_t in disallowed_godot_types:
                                raise Exception(
                                    f'Use of the "{basic_t}" type in the Native API is currently disallowed.'
                                )
                            else:
                                arg_dict["basic_godot_type"] = basic_t
                        elif len(words) and basic_t.replace("godot::", "") == words[-1]:
                            if "name" in arg_dict:
                                msg = f'Type "{basic_t}" is incorrectly specified for argument "{arg_dict["name"]}": "{arg_dict["type"]}".'
                            else:
                                msg = f'Type "{basic_t}" is incorrectly specified for the return value "{arg_dict["type"]}".'
                            raise Exception(msg + ' Godot types must explicitly specify the namespace "godot::".')

                """
                Return fields:
                    "type": str
                    "c_type": str
                    "basic_godot_type": str
                    "ref_class" (opt): str
                    "enum_type" (opt): dict
                """

                if len(docs) == 0:
                    docs = search_for_docs_override(idx)

                func_name_match = re.search(r"\b(\w+)\s*\(", line)
                func_name = func_name_match.group(1).strip()
                ret_type = line[: line.index(func_name)].replace("NAPI ", "").strip()
                print(f"\tFound method {func_name}")

                is_self_return = False
                if is_refcounted:
                    if ret_type == "NSELF_RETURN" or ret_type == f"Ref<{current_class}>":
                        ret_type = "void"
                        is_self_return = True

                c_ret_type = ret_type
                ret_ref_class = ""
                ret_type_match = re.search(r"Ref\<(godot\:\:\w*)\>", ret_type)
                if ret_type_match:
                    c_ret_type = "const uint64_t"
                    ret_ref_class = ret_type_match.group(1).strip()

                ret_dict = {
                    "type": ret_type,
                    "c_type": c_ret_type,
                }
                if len(ret_ref_class):
                    ret_dict["ref_class"] = ret_ref_class

                check_array_usage(ret_type, True)
                check_argument_types(ret_dict)

                # get all args even in one-line functions
                cbrace_end = line.rfind("{")
                args_str = line[
                    line.find("(") + 1 : line.rfind(")", 0, cbrace_end if cbrace_end != -1 else len(line) - 1)
                ]
                args = []

                if not is_singleton:
                    args.append("void *inst_ptr")

                if len(args_str):
                    # args = [a.strip() for a in args]
                    tmp_args_str = args_str
                    while len(tmp_args_str):
                        found_comma = False
                        nesting = 0
                        for idx, c in enumerate(tmp_args_str):
                            if c in ["(", "[", "{", "<"]:
                                nesting += 1
                            if c in [")", "]", "}", ">"]:
                                nesting -= 1
                            if nesting == 0 and c == ",":
                                args.append(tmp_args_str[:idx].strip())
                                tmp_args_str = tmp_args_str[idx + 1 :]
                                found_comma = True
                                break
                            if nesting < 0:
                                raise Exception("There are more closing brackets than opening ones:\n" + tmp_args_str)
                        if nesting > 0:
                            raise Exception("There are more opening brackets than closing ones:\n" + tmp_args_str)

                        if not found_comma:
                            args.append(tmp_args_str)
                            break

                """
                Arg fields:
                    "name": str
                    "type": str
                    "c_type": str
                    "default" (opt): str
                    "basic_godot_type": str
                    "ref_class" (opt) or "object_class" (opt): str
                    "enum_type" (opt): dict
                """

                args_dict = []
                for aidx, a in enumerate(args):
                    arg_name_match = re.search(r"\b(\w+)\s*=", a)
                    if arg_name_match:
                        new_arg = {
                            "name": arg_name_match.group(1).strip(),
                            "type": a[: a.index(arg_name_match.group(1))].strip(),
                            "default": a[a.find("=") + 1 :].strip(),
                        }

                        tmp_default = new_arg["default"]
                        if tmp_default.startswith("Colors::"):
                            new_arg["default"] = default_colors_map[tmp_default]

                        new_arg["c_type"] = new_arg["type"].replace("&", "").strip()
                    else:
                        arg_name_match = re.search(r"\b(\w+)$", a)
                        if arg_name_match:
                            new_arg = {
                                "name": arg_name_match.group(1).strip(),
                                "type": a[: a.index(arg_name_match.group(1))].strip(),
                            }
                        else:
                            new_arg = {
                                "name": f"arg{aidx}",
                                "type": a.strip(),
                            }
                        new_arg["c_type"] = new_arg["type"].replace("&", "").strip()

                    # Search for PackedArray
                    check_array_usage(new_arg["type"])

                    # Search for basic Variant types
                    check_argument_types(new_arg)

                    tmp_type = new_arg["type"]
                    ref_type_match = re.search(r"Ref\<(godot\:\:\w*)\>", tmp_type)

                    if ref_type_match:
                        new_arg["type"] = tmp_type
                        new_arg["c_type"] = "const uint64_t"
                        new_arg["ref_class"] = ref_type_match.group(1).strip()
                        print("\t\tFound Godot's Ref type:", tmp_type)
                    elif (
                        new_arg["name"] != "inst_ptr"
                        and tmp_type != "void *"
                        and "basic_godot_type" not in new_arg
                        and tmp_type.endswith("*")
                        and "godot::" in tmp_type
                    ):
                        new_arg["type"] = "const uint64_t"
                        new_arg["c_type"] = "const uint64_t"
                        new_arg["object_class"] = (
                            tmp_type.replace("const ", "").replace("class ", "").replace("*", "").strip()
                        )
                        print("\t\tFound Godot's Object* type:", tmp_type)

                    args_dict.append(new_arg)

                fun_dict = {
                    "name": func_name,
                    "c_name": f"{current_class}_{func_name}",
                    "docs": docs,
                    "args": args_dict,
                    "return": ret_dict,
                    "self_return": is_self_return,
                }

                """
                Array fields:
                    "data_name": str
                    "data_idx": int
                    "size_name" (opt): str
                    "size_idx" (opt): int
                    "array_type": str
                    "is_const": bool
                """

                found_arrays = []
                for idx in range(len(args_dict)):
                    a1 = args_dict[idx]
                    const_array = a1["type"].startswith("const")
                    words1 = re.findall(r"\w+", a1["type"])
                    array_type = None

                    if a1.get("basic_godot_type", "") in basic_types_to_arrays:
                        array_type = basic_types_to_arrays[a1["basic_godot_type"]]
                    elif words1[-1] in basic_types_to_arrays:
                        array_type = basic_types_to_arrays[words1[-1]]

                    if (
                        (
                            (a1["name"].endswith("_data") and idx + 1 < len(args_dict))
                            or (a1["name"].endswith("_string"))
                        )
                        and "ref_class" not in a1
                        and "object_class" not in a1
                        and "enum_type" not in a1
                        and a1["type"].endswith("*")
                        and array_type
                    ):
                        if a1["name"].endswith("_data"):
                            a2 = args_dict[idx + 1]
                            if a2["name"].endswith("_size"):
                                if not a2["name"].startswith(a1["name"].removesuffix("_data")):
                                    print(
                                        f'WARN: Argument name for array size "{a2["name"]}" does not match pointer name "{a1["name"]}".'
                                    )

                                words2 = re.findall(r"\w+", a2["type"])
                                size_types = ["uint64_t", "int64_t"]
                                if words2[-1] in size_types:
                                    arr_dict = {
                                        "data_name": a1["name"],
                                        "data_idx": idx,
                                        "size_name": a2["name"],
                                        "size_idx": idx + 1,
                                        "array_type": array_type,
                                        "is_const": const_array,
                                    }
                                    found_arrays.append(arr_dict)
                                    print(
                                        "\t\tArguments suitable for the PackedArray wrapper have been found: "
                                        + f'{idx} - {a1["name"]} and {idx + 1} - {a2["name"]}'
                                    )

                                    if not func_name.endswith("_c"):
                                        raise Exception('Function that use C arrays should have name ending in "_c".')
                                else:
                                    raise Exception(
                                        "Arguments suitable for the PackedArray wrapper have been found, "
                                        + f'but the "{a2["type"]} {a2["name"]}" argument type must be one of: {size_types}'
                                    )
                        else:
                            arr_dict = {
                                "data_name": a1["name"],
                                "data_idx": idx,
                                "array_type": array_type,
                                "is_const": const_array,
                            }
                            found_arrays.append(arr_dict)

                            print(
                                f'\t\tArguments suitable for the String wrapper have been found: {idx} - {a1["name"]}'
                            )

                            if not func_name.endswith("_c"):
                                raise Exception('Function that use C strings should have name ending in "_c".')
                if len(found_arrays):
                    fun_dict["arg_arrays"] = found_arrays

                # Name of the generated C API function

                functions.append(fun_dict)
                print(f"\t  Parsing of method {func_name} completed")

    ###
    # Mark all Enum's in args and return types

    """
    Enum arg fields:
        "class": str
        "name": str
        "type": str
    """

    exposed_enums = {}
    exposed_enums_only_names = []
    exposed_enums_only_names_map = {}
    for cls_name in classes:
        cls = classes[cls_name]
        for e_name in cls["enums"]:
            e = cls["enums"][e_name]
            new_dict = {"class": cls_name, "name": e_name, "type": e["type"]}
            name = f"{cls_name}::{e_name}"
            exposed_enums[name] = new_dict
            exposed_enums_only_names.append(e_name)

            if e_name in exposed_enums_only_names_map:
                raise Exception(
                    f'The enum "{e_name}" already exists in another class "{exposed_enums_only_names_map[e_name]}".'
                )
            exposed_enums_only_names_map[e_name] = name

    for cls_name in classes:
        cls = classes[cls_name]
        for f in cls["functions"]:

            def check_type_name(type: str):
                for e in exposed_enums_only_names:
                    if e in type:
                        if exposed_enums_only_names_map[e] not in type:
                            print(
                                f'ERROR: "{exposed_enums_only_names_map[e]}" is named incorrectly in the "{f["name"]}" function.'
                                + f' "{e}" should be called "{exposed_enums_only_names_map[e]}".'
                            )
                            return None
                        else:
                            return exposed_enums_only_names_map[e]
                return None

            e_name = check_type_name(f["return"]["type"])
            if e_name:
                f["return"]["enum_type"] = exposed_enums[e_name]

            for a in f["args"]:
                e_name = check_type_name(a["type"])
                if e_name:
                    a["enum_type"] = exposed_enums[e_name]

    return classes


####################################
####################################


## c_api.gen.cpp


####################################
####################################
def generate_native_api(
    env: SConsEnvironment,
    header_files: list,
    c_api_template: str,
    out_folder: str,
    src_out: list,
    additional_include_classes: list = [],
) -> dict:
    classes = get_api_functions(env, header_files)

    new_funcs = []
    new_func_regs = []
    new_ref_clears = []

    extern_c_dbg = 'extern "C"'
    extern_c_dbg = ""

    for cls in classes:
        print(f"Genearating DD3D C_API constructors for {cls}...")

        is_refcounted = classes[cls]["type"] == "refcounted"
        if not is_refcounted:
            continue

        new_funcs.append(f"struct {cls}_NAPIWrapper {{")
        new_funcs.append(f"\tRef<{cls}> ref;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"static std::unordered_set<{cls}_NAPIWrapper*> {cls}_NAPIWrapper_storage;")
        new_funcs.append("")
        new_funcs.append(f"{extern_c_dbg}static void* {cls}_create() {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst_ptr = new {cls}_NAPIWrapper{{Ref<{cls}>(memnew({cls}))}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst_ptr);")
        new_funcs.append("\treturn inst_ptr;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"{extern_c_dbg}static void* {cls}_create_nullptr() {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst_ptr = new {cls}_NAPIWrapper{{}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst_ptr);")
        new_funcs.append("\treturn inst_ptr;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"static void* {cls}_create_from_ref(Ref<{cls}> ref) {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst_ptr = new {cls}_NAPIWrapper{{ref}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst_ptr);")
        new_funcs.append("\treturn inst_ptr;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"{extern_c_dbg}static void {cls}_destroy(void *inst_ptr) {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(
            f"\tif (const auto it = {cls}_NAPIWrapper_storage.find(static_cast<{cls}_NAPIWrapper*>(inst_ptr)); it != {cls}_NAPIWrapper_storage.end()){{"
        )
        new_funcs.append(f"\t\t{cls}_NAPIWrapper_storage.erase(it);")
        new_funcs.append(f"\t\tdelete *it;")
        new_funcs.append("\t};")
        new_funcs.append("};")
        new_funcs.append("")

        new_func_regs.append(f"\t\tADD_FUNC({cls}_create);")
        new_func_regs.append(f"\t\tADD_FUNC({cls}_create_nullptr);")
        new_func_regs.append(f"\t\tADD_FUNC({cls}_destroy);")

        new_ref_clears.append(f"\tCLEAR_REFS({cls}, {cls}_NAPIWrapper_storage);")

    for cls in classes:
        print(f"Genearating DD3D C_API for {cls}...")

        class_type = classes[cls]["type"]
        is_singleton = class_type == "singleton"
        is_refcounted = class_type == "refcounted"
        functions = classes[cls]["functions"]

        def get_ref_class_name(ret_type: str):
            if ret_type.startswith("Ref<"):
                return ret_type[4:-1]
            return ret_type

        def is_ref_in_api(ret_type: str):
            if ret_type.startswith("Ref<"):
                if get_ref_class_name(ret_type) in classes:
                    return True
            return False

        for func in functions:

            def process_ret_type(ret: dict):
                type = ret["c_type"]
                if "enum_type" in ret:
                    e = ret["enum_type"]
                    return f"{e['type']} /*{e['class']}::{e['name']}*/"

                return type

            def process_ret_calls(line: str, ret: dict):
                if "ref_class" in ret:
                    return f"{line}->get_instance_id()"
                if "enum_type" in ret:
                    e = ret["enum_type"]
                    return f"static_cast<{e['type']} /*{e['class']}::{e['name']}*/>({line})"

                return line

            def process_arg_decl(arg: dict):
                type = arg["c_type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"void *{name}"
                if "ref_class" in arg:
                    return f"{type} /*{arg['ref_class']}*/ {name}"
                if "object_class" in arg:
                    return f"{type} /*{arg['object_class']}*/ {name}"
                if "enum_type" in arg:
                    e = arg["enum_type"]
                    return f"{e['type']} /*{e['class']}::{e['name']}*/ {name}"
                return f"{type} {name}"

            def process_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"static_cast<{get_ref_class_name(type)}_NAPIWrapper *>({name})->ref"
                if "ref_class" in arg:
                    return f"godot::Ref<{arg['ref_class']}>(godot::Object::cast_to<{arg['ref_class']}>(godot::ObjectDB::get_instance({name})))"
                if "object_class" in arg:
                    return f"godot::Object::cast_to<{arg['object_class']}>(godot::ObjectDB::get_instance({name}))"
                if "enum_type" in arg:
                    e = arg["enum_type"]
                    return f"static_cast<{e['class']}::{e['name']}>({name})"

                return f"{name}"

            func_name = func["c_name"]
            func_orig_name = func["name"]
            ret = func["return"]
            ret_type = process_ret_type(ret)
            self_ret = func["self_return"]
            is_ret_custom_ref = is_ref_in_api(ret_type)

            args = func["args"]

            # func define
            new_func_define_args = ", ".join([process_arg_decl(a) for a in args])
            new_funcs.append(
                f'{extern_c_dbg}static {"void *" if is_ret_custom_ref else ret_type} {func_name}({new_func_define_args}) {{'
            )
            new_funcs.append("\tZoneScoped;")

            if is_singleton:
                call_args = ", ".join([process_arg_calls(a) for a in args])
                ret_str = f"{cls}::get_singleton()->{func_orig_name}({call_args})"

                if ret_type != "void":
                    if is_ret_custom_ref:
                        new_funcs.append(f"\treturn {get_ref_class_name(ret_type)}_create_from_ref({ret_str});")
                    else:
                        new_funcs.append(f"\treturn {process_ret_calls(ret_str, ret)};")
                else:
                    new_funcs.append(f"\t{ret_str};")
            else:
                call_args = ", ".join([process_arg_calls(a) for a in args[1:]])
                ret_str = f"static_cast<{cls}_NAPIWrapper *>(inst_ptr)->ref->{func_orig_name}({call_args})"

                if ret_type != "void":
                    new_funcs.append(f"\treturn {process_ret_calls(ret_str, ret)};")
                else:
                    new_funcs.append(f"\t{ret_str};")
            new_funcs.append("}")
            new_funcs.append("")

            new_func_regs.append(f"\t\tADD_FUNC({func_name});")

    c_api_lines = split_text_into_lines(lib_utils.read_all_text(c_api_template))

    insert_lines_at_mark(
        c_api_lines,
        "// GENERATOR_DD3D_API_INCLUDES",
        [f'#include "{file.replace("src/","")}"' for file in header_files],
    )
    insert_lines_at_mark(
        c_api_lines,
        "// GENERATOR_DD3D_GODOT_API_INCLUDES",
        [f"#include <godot_cpp/classes/{i}.hpp>" for i in additional_include_classes],
    )
    insert_lines_at_mark(c_api_lines, "// GENERATOR_DD3D_FUNCTIONS", new_funcs)
    insert_lines_at_mark(c_api_lines, "// GENERATOR_DD3D_FUNCTIONS_REGISTRATIONS", new_func_regs)
    insert_lines_at_mark(c_api_lines, "// GENERATOR_DD3D_REFS_CLEAR", new_ref_clears)
    c_api_file_name, c_api_file_ext = os.path.splitext(os.path.basename(c_api_template))

    c_api_gen_path = os.path.join("gen", f"{c_api_file_name}.gen{c_api_file_ext}")
    lib_utils.write_all_text(os.path.join("src", c_api_gen_path), "\n".join(c_api_lines))
    src_out.append(c_api_gen_path)

    api_dict = {"hash": hashlib.sha1(json.dumps(classes, sort_keys=True).encode()).hexdigest(), "classes": classes}
    lib_utils.write_all_text(os.path.join(out_folder, "api.json"), json.dumps(api_dict, indent="  "))

    return api_dict


####################################
####################################


## dd3d_cpp_api.hpp


####################################
####################################
def gen_cpp_api(env: SConsEnvironment, api: dict, out_folder: str, additional_include_classes: list = []) -> bool:
    os.makedirs(out_folder, exist_ok=True)
    classes: dict = api["classes"]
    is_namespace_a_class = {}
    is_class_has_selfreturn = {}
    namespaces = {}
    fwd_decls = []

    for cls in classes:
        print(f"Genearating CPP_API forward declarations for {cls}...")
        cls_is_class = classes[cls]["type"] != "singleton"
        if cls_is_class:
            fwd_decls.append(f"class {cls};")

    for cls in classes:
        print(f"Genearating CPP_API for {cls}...")

        enums = classes[cls]["enums"]
        functions: list = classes[cls]["functions"]
        cls_is_class = classes[cls]["type"] != "singleton"
        cls_indent = "\t" if cls_is_class else ""

        if cls not in namespaces:
            is_namespace_a_class[cls] = cls_is_class
            namespaces[cls] = []

        for enum_name in enums:
            type = enums[enum_name]["type"]
            values = enums[enum_name]["values"]
            enum_lines = []
            if cls_is_class:
                enum_lines.append("public:")
            enum_lines.append(f"{cls_indent}enum {enum_name} : {type} {{")
            for value_name in values:
                enum_lines.append(f"{cls_indent}\t{value_name} = {values[value_name]},")
            enum_lines.append(f"{cls_indent}}};")
            enum_lines.append("")

            namespaces[cls] += enum_lines

        #
        # Class wrapper
        if cls_is_class:
            con_lines = []
            con_lines.append(f"{cls_indent}void *inst_ptr;")
            con_lines.append("")
            con_lines.append(f"public:")
            con_lines.append(f"{cls_indent}{cls}(void *inst_ptr) :")
            con_lines.append(f"{cls_indent}\t\tinst_ptr(inst_ptr) {{}}")
            con_lines.append("")
            con_lines.append(f"{cls_indent}{cls}(bool instantiate = true) :")
            con_lines.append(f"{cls_indent}\t\tinst_ptr(instantiate ? create() : create_nullptr()) {{}}")
            con_lines.append("")
            con_lines.append(f"{cls_indent}~{cls}() {{ destroy(inst_ptr); }}")
            con_lines.append("")
            con_lines.append(f"{cls_indent}operator void *() const {{ return inst_ptr; }}")
            con_lines.append("")
            namespaces[cls] += con_lines

            functions.append(
                {
                    "name": "create",
                    "c_name": f"{cls}_create",
                    "docs": [],
                    "args": [],
                    "return": {"type": "void *", "c_type": "void *"},
                    "self_return": False,
                    "private": True,
                }
            )

            functions.append(
                {
                    "name": "create_nullptr",
                    "c_name": f"{cls}_create_nullptr",
                    "docs": [],
                    "args": [],
                    "return": {"type": "void *", "c_type": "void *"},
                    "self_return": False,
                    "private": True,
                }
            )

            functions.append(
                {
                    "name": "destroy",
                    "c_name": f"{cls}_destroy",
                    "docs": [],
                    "args": [
                        {
                            "name": "inst_ptr",
                            "type": "void *",
                            "c_type": "void *",
                        },
                    ],
                    "return": {"type": "void", "c_type": "void"},
                    "self_return": False,
                    "private": True,
                }
            )

        #
        # Array function wrappers
        """
        New fields for args:
            "is_const_array": bool
            "custom_call_args": bool
        New fields for function:
            "wrapper_orig_name": str
            "wrapper_args": list
        """
        functions_to_add = []
        for idx, func in enumerate(functions):
            if "arg_arrays" in func:
                arrays_info = func["arg_arrays"]
                new_func = copy.deepcopy(func)
                new_func.pop("arg_arrays")
                wrapper_args: list = copy.deepcopy(new_func["args"])

                for arr in reversed(arrays_info):
                    if "size_idx" in arr:
                        wrapper_args.pop(arr["size_idx"])
                    data_arg = wrapper_args.pop(arr["data_idx"])

                    new_arg = {
                        "name": data_arg["name"].removesuffix("_data").removesuffix("_string"),
                        "type": arr["array_type"],
                        "c_type": arr["array_type"],
                        "is_const_array": arr["is_const"],
                    }

                    if "default" in data_arg and "size_idx" not in arr:
                        new_arg["default"] = data_arg["default"]

                    arr_args = []
                    new_arg["custom_call_args"] = arr_args

                    ptr_suffix = "ptr()" if arr["is_const"] else "ptrw()"
                    if arr["array_type"] in ["godot::String"]:
                        arr_args.append(f'{new_arg["name"]}.utf8().' + ptr_suffix)
                    else:
                        arr_args.append(f'{new_arg["name"]}.' + ptr_suffix)

                    if "size_idx" in arr:
                        arr_args.append(f'{new_arg["name"]}.size()')

                    wrapper_args.insert(arr["data_idx"], new_arg)

                new_func["wrapper_args"] = wrapper_args
                new_func["wrapper_orig_name"] = func["name"]
                new_func["name"] = func["name"].removesuffix("_c")
                functions_to_add.append((idx + 1, new_func))

        for f_pair in reversed(functions_to_add):
            functions.insert(f_pair[0], f_pair[1])

        prev_private_state = False
        new_lines = []

        for func in functions:

            def get_default_ret_val(r_type: str):
                if r_type.endswith("*") or r_type.startswith("Ref<"):
                    return "nullptr"
                return "{}"

            def get_ref_class_name(r_type: str):
                if r_type.startswith("Ref<"):
                    return r_type[4:-1]
                return r_type

            def is_ref_in_api(r_type: str):
                if r_type.startswith("Ref<"):
                    if get_ref_class_name(r_type) in classes:
                        return True
                return False

            def process_ret_type_decl(r_type: str, ret: dict):
                if is_ref_in_api(r_type):
                    return f"std::shared_ptr<{get_ref_class_name(r_type)}>"
                if "ref_class" in ret:
                    return f"godot::Ref<{ret['ref_class']}>"

                return r_type

            def process_ret_func_ptr_decl(r_type: str, ret: dict):
                if is_ref_in_api(r_type):
                    return "void *"
                if "enum_type" in ret:
                    e = ret["enum_type"]
                    return f"{e['type']} /*{e["class"]}::{e["name"]}*/"

                return r_type

            def get_ret_required_cast_type(ret: dict):
                if "enum_type" in ret:
                    e = ret["enum_type"]
                    return f"{e["class"]}::{e["name"]}"

                return None

            def process_arg_decl(arg: dict):
                type = arg["c_type"].strip()
                name = arg["name"].strip()
                res = f"{type} {name}"

                if not type.endswith("*"):
                    res = f"{type} &{name}"

                if not res.startswith("const ") and not type.endswith("*") and arg.get("is_const_array", True):
                    res = "const " + res

                if is_ref_in_api(type):
                    res = f"const std::shared_ptr<{get_ref_class_name(type)}> &{name}"
                if "ref_class" in arg:
                    res = f"const godot::Ref<{arg['ref_class']}> &{name}"
                if "object_class" in arg:
                    res = f"const {arg['object_class']} * {name}"

                if "default" in arg:
                    res += f" = {arg['default']}"
                return res

            def process_arg_funcptr_decl(arg: dict):
                type = arg["c_type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"void * /*{name}*/"
                if "object_class" in arg:
                    return f"{type} /*{arg['object_class']} {name}*/"
                if "enum_type" in arg:
                    e = arg["enum_type"]
                    return f"{e['type']} /*{e["class"]}::{e["name"]} {name}*/"

                return f"{type} /*{name}*/"

            def process_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"*{name}"
                if "ref_class" in arg:
                    return f"{name}.is_valid() ? {name}->get_instance_id() : 0"
                if "object_class" in arg:
                    return f"{name} ? {name}->get_instance_id() : 0"
                if "enum_type" in arg:
                    e = arg["enum_type"]
                    return f"static_cast<{e['type']}>({name})"

                if "custom_call_args" in arg:
                    return ", ".join(arg["custom_call_args"])

                return name

            def process_array_wrapper_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]

                if "custom_call_args" in arg:
                    return ", ".join(arg["custom_call_args"])

                return name

            func_name = func["c_name"]
            func_orig_name = func["name"]
            ret = func["return"]
            ret_type = ret["c_type"]
            args = func["args"]
            wrapper_args = func.get("wrapper_args", None)
            self_ret = func["self_return"]
            is_private = func.get("private", False)

            if self_ret:
                if ret_type == "void":
                    ret_type = f"std::shared_ptr<{cls}>"

                is_class_has_selfreturn[cls] = True

            if prev_private_state != is_private:
                prev_private_state = is_private
                if is_private:
                    new_lines.append("private:")
                else:
                    new_lines.append("public:")

            docs = func["docs"]
            if len(docs):
                docs = [cls_indent + " * " + line for line in docs]
                docs.insert(0, f"{cls_indent}/**")
                docs.insert(len(docs), f"{cls_indent} */")

            new_lines += docs

            # Function Declaration
            is_wrapper = wrapper_args != None
            wrapper_orig_name = func.get("wrapper_orig_name", None)
            use_args = wrapper_args if is_wrapper else args

            if cls_is_class and not is_private:
                inner_args = ", ".join([process_arg_decl(a) for a in use_args[1:]])
                new_lines.append(
                    f'{cls_indent}{process_ret_type_decl(ret_type, ret)} {func_orig_name.removesuffix("_selfreturn")}({inner_args}) {{'
                )
            else:
                inner_args = ", ".join([process_arg_decl(a) for a in use_args])
                new_lines.append(
                    f'{cls_indent}static {process_ret_type_decl(ret_type, ret)} {func_orig_name.removesuffix("_selfreturn")}({inner_args}) {{'
                )

            if not is_wrapper:
                # Function pointer
                new_lines.append(
                    f'{cls_indent}\tstatic {"void" if self_ret else process_ret_func_ptr_decl(ret_type, ret)}(*{func_name})({", ".join([process_arg_funcptr_decl(a) for a in args])}) = nullptr;'
                )

                # Function calls
                def_ret_val = get_default_ret_val(ret_type)
                call_args = ", ".join([process_arg_calls(a) for a in use_args])
                if len(call_args):
                    call_args = ", " + call_args

                if ret_type != "void" and not self_ret:
                    if is_ref_in_api(ret_type):
                        new_lines.append(
                            f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET_REF_TO_SHARED({func_name}, {get_ref_class_name(ret_type)}, {def_ret_val}{call_args});"
                        )
                    else:
                        if "ref_class" in ret:
                            new_lines.append(
                                f'{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET_GODOT_REF({func_name}, {ret["ref_class"]}, {def_ret_val}{call_args});'
                            )
                        else:
                            cast_type = get_ret_required_cast_type(ret)
                            if cast_type:
                                new_lines.append(
                                    f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET_CAST({func_name}, {cast_type}, {def_ret_val}{call_args});"
                                )
                            else:
                                new_lines.append(
                                    f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET({func_name}, {def_ret_val}{call_args});"
                                )
                else:
                    if self_ret:
                        new_lines.append(f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_SELFRET({func_name}{call_args});")
                        new_lines.append(f"{cls_indent}\treturn shared_from_this();")
                    else:
                        new_lines.append(f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER({func_name}{call_args});")
            else:
                # Array wrapper calls
                call_args = ", ".join([process_array_wrapper_arg_calls(a) for a in use_args])

                if ret_type != "void" or self_ret:
                    new_lines.append(f"{cls_indent}\treturn {wrapper_orig_name}({call_args});")
                else:
                    new_lines.append(f"{cls_indent}\t{wrapper_orig_name}({call_args});")

            new_lines.append(f"{cls_indent}}}")
            new_lines.append("")

        namespaces[cls] += new_lines

    # Copy the contents of the "c_api_shared.hpp"
    c_api_shared_content = split_text_into_lines(lib_utils.read_all_text("src/native_api/c_api_shared.hpp"))
    c_api_shared_content_start = -1
    for idx, l in enumerate(c_api_shared_content):
        if l.endswith("// GENERATOR_DD3D_API_SHARED_EMBED_START"):
            c_api_shared_content_start = idx
            break
    c_api_shared_content = c_api_shared_content[c_api_shared_content_start + 1 :]

    lines = split_text_into_lines(lib_utils.read_all_text("src/native_api/templates/cpp/dd3d_cpp_api.hpp"))

    remove_lines = []
    for idx, l in enumerate(lines):
        if l.endswith("// GENERATOR_DD3D_API_REMOVE_LINE"):
            remove_lines.append(idx)
    for i in reversed(remove_lines):
        lines.pop(i)

    # Combine class lines
    result_arr = [""]
    for key in namespaces:
        docs: list = classes[key]["docs"]
        if len(docs):
            docs = [" * " + line for line in docs]
            docs.insert(0, "/**")
            docs.insert(len(docs), " */")
        result_arr = result_arr + docs

        if is_namespace_a_class[key]:
            if key in is_class_has_selfreturn:
                # class has self return functions
                result_arr.append(f"class {key} : public std::enable_shared_from_this<{key}> {{")
            else:
                # regular class
                result_arr.append(f"class {key} {{")
            result_arr += namespaces[key]
            result_arr.append(f"}}; // class {key}")
        else:
            result_arr.append(f"namespace {key} {{")
            result_arr += namespaces[key]
            result_arr.append(f"}} // namespace {key}")
        result_arr.append("")

    insert_lines_at_mark(lines, "// GENERATOR_DD3D_API_SHARED_EMBED", c_api_shared_content)
    insert_lines_at_mark(
        lines,
        "// GENERATOR_DD3D_API_INCLUDES",
        [f"#include <godot_cpp/classes/{i}.hpp>" for i in additional_include_classes],
    )
    insert_lines_at_mark(lines, "// GENERATOR_DD3D_API_FORWARD_DECLARATIONS", fwd_decls)
    insert_lines_at_mark(lines, "// GENERATOR_DD3D_API_FUNCTIONS", result_arr)
    return lib_utils.write_all_text(os.path.join(out_folder, "dd3d_cpp_api.hpp"), "\n".join(lines))


def gen_apis(env: SConsEnvironment, c_api_template: str, out_folder: str, src_folder: str, src_out: list):
    print("Generating native API!")
    os.makedirs(out_folder, exist_ok=True)

    src = json.loads(lib_utils.read_all_text(src_folder + "/default_sources.json"))
    header_files = [os.path.join(src_folder, f.replace(".cpp", ".h")).replace("\\", "/") for f in src]
    header_files = [h for h in header_files if os.path.exists(h)]
    additional_includes = ["camera3d", "control", "font", "viewport"]

    api = generate_native_api(env, header_files, c_api_template, out_folder, src_out, additional_includes)
    if api == None:
        print("Couldn't get the Native API")
        return 110

    if not gen_cpp_api(env, copy.deepcopy(api), os.path.join(out_folder, "cpp"), additional_includes):
        return 111
    print("The generation is finished!")
    return 0
