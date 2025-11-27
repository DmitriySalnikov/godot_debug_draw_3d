#!/usr/bin/env python3

# Ref<godot::T> or godot::Ref<godot::T> is used for Godot types
# and Ref<T> is for DD3D classes

# TODO: add tests to CI

from SCons.Script.SConscript import SConsEnvironment

import os, shutil, json, re, hashlib
import lib_utils


def dev_print(line: str):
    if False:
        print(line)


default_colors_map = {
    "Colors::empty_color": "godot::Color(0, 0, 0, 0)",
    "Colors::white_smoke": "godot::Color(0.96f, 0.96f, 0.96f, 1.0f)",
}


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


def get_api_functions(env: SConsEnvironment, headers: list) -> dict:
    classes = {}
    def_ctx: DefineContext = DefineContext(env)

    for header in headers:
        print(f"Parsing {header} ...")
        text_data = lib_utils.read_all_text(header)
        lines = text_data.replace("\r\n", "\n").replace("\r", "\n").split("\n")
        lines = [line.strip() for line in lines]
        def_ctx.reset()

        functions = {}
        enums = {}
        current_class = ""

        is_singleton = False
        is_refcounted = False

        for idx, line in enumerate(lines):
            if not def_ctx.parse_line(line):
                dev_print(f"\t\tSkipping: {idx + 1}: {line}")
                continue

            docs = []
            if lines[idx - 1] == "*/":
                doc_idx = idx - 1
                while lines[doc_idx] != "/**":
                    doc_idx -= 1
                docs = [
                    line.replace("*", "", 1).strip() if line.startswith("*") else line
                    for line in lines[doc_idx + 1 : idx - 1]
                ]
                # replaces images with placeholder
                docs = [re.sub(r"(\!\[.*\]\(.*\))", "[THERE WAS AN IMAGE]", line) for line in docs]

            class_prefixes = ["NAPI_CLASS class ", "NAPI_CLASS_SINGLETON class ", "NAPI_CLASS_REF class "]
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

                functions = {}
                enums = {}
                classes[current_class] = {"enums": enums, "functions": functions, "type": class_type, "docs": docs}

                if is_singleton:
                    print(f"Found Singleton {current_class}")
                elif is_refcounted:
                    print(f"Found RefCounted {current_class}")
                continue

            if line.startswith("NAPI_ENUM enum "):
                split = line[len("NAPI_ENUM enum ") : line.find("{")].split(":")
                name = split[0].strip()
                type = split[1].strip()

                print(f"\tFound Enum {name} of type {type}")
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
                    value = last_val + 1
                    if len(split) == 2:
                        value = split[1].strip()

                    if value is int:
                        last_val = value
                    elif value is str and value.isdigit():
                        last_val = int(value)
                    values[key] = str(value)

                enums[name] = {"type": type, "values": values}
                continue

            if line.startswith("NAPI "):
                func_name_match = re.search(r"\b(\w+)\s*\(", line)
                func_name = func_name_match.group(1).strip()
                ret_type = line[: line.index(func_name)].replace("NAPI ", "").strip()

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

                args_dict = []
                for aidx, a in enumerate(args):
                    arg_name_match = re.search(r"\b(\w+)\s*=", a)
                    if arg_name_match:
                        new_dict = {
                            "name": arg_name_match.group(1).strip(),
                            "type": a[: a.index(arg_name_match.group(1))].strip(),
                            "default": a[a.find("=") + 1 :].strip(),
                        }

                        tmp_default = new_dict["default"]
                        if tmp_default.startswith("Colors::"):
                            new_dict["default"] = default_colors_map[tmp_default]

                        new_dict["c_type"] = new_dict["type"].replace("&", "").strip()
                    else:
                        arg_name_match = re.search(r"\b(\w+)$", a)
                        if arg_name_match:
                            new_dict = {
                                "name": arg_name_match.group(1).strip(),
                                "type": a[: a.index(arg_name_match.group(1))].strip(),
                            }
                        else:
                            new_dict = {
                                "name": f"arg{aidx}",
                                "type": a.strip(),
                            }
                        new_dict["c_type"] = new_dict["type"].replace("&", "").strip()

                    tmp_type = new_dict["type"]
                    ref_type_match = re.search(r"Ref\<(godot\:\:\w*)\>", tmp_type)

                    if ref_type_match:
                        new_dict["type"] = tmp_type
                        new_dict["c_type"] = "const uint64_t"
                        new_dict["ref_class"] = ref_type_match.group(1).strip()
                        print("\t\tFound Godot's Ref type:", tmp_type)
                    elif new_dict["name"] != "inst_ptr" and tmp_type != "void *" and tmp_type.endswith("*"):
                        new_dict["type"] = "const uint64_t"
                        new_dict["c_type"] = "const uint64_t"
                        new_dict["object_class"] = (
                            tmp_type.replace("const ", "").replace("class ", "").replace("*", "").strip()
                        )
                        print("\t\tFound Godot's Object* type:", tmp_type)

                    args_dict.append(new_dict)

                ret_dict = {
                    "type": ret_type,
                    "c_type": c_ret_type,
                }
                if len(ret_ref_class):
                    ret_dict["ref_class"] = ret_ref_class

                fun_dict = {
                    "return": ret_dict,
                    "self_return": is_self_return,
                    "args": args_dict,
                    "name": func_name,
                    "docs": docs,
                }
                functions[f"{current_class}_{func_name}"] = fun_dict
                print(f"\tFound method {func_name}")

    return classes


## gen/c_api.gen.cpp
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
        new_funcs.append(f'extern "C" static void* {cls}_create() {{')
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst_ptr = new {cls}_NAPIWrapper{{Ref<{cls}>(memnew({cls}))}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst_ptr);")
        new_funcs.append("\treturn inst_ptr;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f'extern "C" static void* {cls}_create_nullptr() {{')
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
        new_funcs.append(f'extern "C" static void {cls}_destroy(void *inst_ptr) {{')
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

        for func_name in functions:
            func = functions[func_name]
            func_orig_name = func["name"]
            ret = func["return"]
            ret_type = ret["c_type"]
            self_ret = func["self_return"]
            is_ret_custom_ref = is_ref_in_api(ret_type)

            args = func["args"]

            # TODO: convert godot::Variant's to void* with _native_ptr
            # TODO: convert godot::String's to something. mb Variant?
            # TODO: or just use const char* utf8 strings

            # TODO: and do the same to other complex types.
            # TODO: or add a new functions with raw pointers with data (e.g. Vector3* points)

            # TODO: replace of the basic godot's types with C Structs
            def process_arg_defines(arg: dict):
                type = arg["c_type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"void *{name}"
                if "ref_class" in arg:
                    return f"{type} /*{arg['ref_class']}*/ {name}"
                if "object_class" in arg:
                    return f"{type} /*{arg['object_class']}*/ {name}"
                return f"{type} {name}"

            def process_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"static_cast<{get_ref_class_name(type)}_NAPIWrapper*>({name})->ref"
                if "ref_class" in arg:
                    return f"godot::Ref<{arg['ref_class']}>(godot::Object::cast_to<{arg['ref_class']}>(godot::ObjectDB::get_instance({name})))"
                if "object_class" in arg:
                    return f"godot::Object::cast_to<{arg['object_class']}>(godot::ObjectDB::get_instance({name}))"
                return f"{name}"

            # func define
            new_func_define_args = ", ".join([process_arg_defines(a) for a in args])
            new_funcs.append(
                f'extern "C" static {"void *" if is_ret_custom_ref else ret_type} {func_name}({new_func_define_args}) {{'
            )
            new_funcs.append("\tZoneScoped;")

            def process_ret_calls(line: str, ret: dict):
                if "ref_class" in ret:
                    return f"{line}->get_instance_id()"
                return line

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
                ret_str = f"static_cast<{cls}_NAPIWrapper*>(inst_ptr)->ref->{func_orig_name}({call_args})"

                if ret_type != "void":
                    new_funcs.append(f"\treturn {process_ret_calls(ret_str, ret)};")
                else:
                    new_funcs.append(f"\t{ret_str};")
            new_funcs.append("}")
            new_funcs.append("")

            new_func_regs.append(f"\t\tADD_FUNC({func_name});")

    c_api_temp_data = lib_utils.read_all_text(c_api_template)
    c_api_lines = c_api_temp_data.replace("\r\n", "\n").replace("\r", "\n").split("\n")

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
    insert_lines_at_mark(c_api_lines, "// GENERATOR_DD3D_FUNCTIONS_DEFINES", new_funcs)
    insert_lines_at_mark(c_api_lines, "// GENERATOR_DD3D_FUNCTIONS_REGISTERS", new_func_regs)
    insert_lines_at_mark(c_api_lines, "// GENERATOR_DD3D_REFS_CLEAR", new_ref_clears)
    c_api_file_name, c_api_file_ext = os.path.splitext(os.path.basename(c_api_template))

    c_api_gen_path = os.path.join("gen", f"{c_api_file_name}.gen{c_api_file_ext}")
    lib_utils.write_all_text(os.path.join("src", c_api_gen_path), "\n".join(c_api_lines))
    src_out.append(c_api_gen_path)

    api_dict = {"hash": hashlib.sha1(json.dumps(classes, sort_keys=True).encode()).hexdigest(), "classes": classes}
    lib_utils.write_all_text(os.path.join(out_folder, "api.json"), json.dumps(api_dict, indent="  "))

    return api_dict


# addons/debug_draw_3d/native_api/cpp/dd3d_cpp_api.hpp
def gen_cpp_api(env: SConsEnvironment, api: dict, out_folder: str, additional_include_classes: list = []) -> bool:
    os.makedirs(out_folder, exist_ok=True)
    classes = dict(api["classes"])
    is_namespace_a_class = {}
    is_class_has_selfreturn = {}
    namespaces = {}
    fwd_decls = []

    for cls in classes:
        print(f"Genearating CPP_API forward declarations for {cls}...")
        functions = classes[cls]["functions"]
        cls_is_class = classes[cls]["type"] != "singleton"
        if cls_is_class:
            fwd_decls.append(f"class {cls};")

    for cls in classes:
        print(f"Genearating CPP_API for {cls}...")

        enums = classes[cls]["enums"]
        functions = classes[cls]["functions"]
        cls_is_class = classes[cls]["type"] != "singleton"
        cls_indent = "\t" if cls_is_class else ""

        if cls not in namespaces:
            is_namespace_a_class[cls] = cls_is_class
            namespaces[cls] = []

        # TODO: convert enum to uint64_t in C API part!!!
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

            functions[f"{cls}_create"] = {
                "return": {"type": "void *", "c_type": "void *"},
                "self_return": False,
                "args": [],
                "name": "create",
                "docs": [],
                "private": True,
            }

            functions[f"{cls}_create_nullptr"] = {
                "return": {"type": "void *", "c_type": "void *"},
                "self_return": False,
                "args": [],
                "name": "create_nullptr",
                "docs": [],
                "private": True,
            }

            functions[f"{cls}_destroy"] = {
                "return": {"type": "void", "c_type": "void"},
                "self_return": False,
                "args": [
                    {
                        "name": "inst_ptr",
                        "type": "void *",
                        "c_type": "void *",
                    },
                ],
                "name": "destroy",
                "docs": [],
                "private": True,
            }

        prev_private_state = False
        new_lines = []

        for func_name in functions:
            func: dict = functions[func_name]
            func_orig_name = func["name"]
            ret = func["return"]
            ret_type = ret["c_type"]
            args = func["args"]
            self_ret = func["self_return"]
            is_private = func.get("private", False)

            if self_ret:
                if ret_type == "void":
                    ret_type = f"std::shared_ptr<{cls}>"

                is_class_has_selfreturn[cls] = True

            def get_default_ret_val(ret_type: str):
                if ret_type.endswith("*") or ret_type.startswith("Ref<"):
                    return "nullptr"
                return "{}"

            def get_ref_class_name(ret_type: str):
                if ret_type.startswith("Ref<"):
                    return ret_type[4:-1]
                return ret_type

            def is_ref_in_api(ret_type: str):
                if ret_type.startswith("Ref<"):
                    if get_ref_class_name(ret_type) in classes:
                        return True
                return False

            def get_ref_to_void_arg_name(ret_type: str):
                if is_ref_in_api(ret_type):
                    return "void *"
                return ret_type

            def get_ref_to_native_name(ret_type: str):
                if is_ref_in_api(ret_type):
                    return f"std::shared_ptr<{get_ref_class_name(ret_type)}>"
                if "ref_class" in ret:
                    return f"godot::Ref<{ret['ref_class']}>"
                return ret_type

            def process_arg_defines(arg: dict):
                type = arg["c_type"].strip()
                name = arg["name"].strip()
                res = f"{type} {name}"

                if not type.endswith("*"):
                    res = f"{type} &{name}"

                if not res.startswith("const ") and not type.endswith("*"):
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

            def process_arg_ptr_defines(arg: dict):
                type = arg["c_type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"void *"
                if "object_class" in arg:
                    return f"{type} /*{arg['object_class']}*/"

                return f"{type}"

            def process_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"*{name}"
                if "ref_class" in arg:
                    return f"{name}.is_valid() ? {name}->get_instance_id() : 0"
                if "object_class" in arg:
                    return f"{name} ? {name}->get_instance_id() : 0"
                return f"{name}"

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
            if cls_is_class and not is_private:
                inner_args = ", ".join([process_arg_defines(a) for a in args[1:]])
                new_lines.append(
                    f'{cls_indent}{get_ref_to_native_name(ret_type)} {func_orig_name.removesuffix("_selfreturn")}({inner_args}) {{'
                )
            else:
                inner_args = ", ".join([process_arg_defines(a) for a in args])
                new_lines.append(
                    f'{cls_indent}static {get_ref_to_native_name(ret_type)} {func_orig_name.removesuffix("_selfreturn")}({inner_args}) {{'
                )

            # Func pointer
            new_lines.append(
                f'{cls_indent}\tstatic {"void" if self_ret else get_ref_to_void_arg_name(ret_type)}(*{func_name})({", ".join([process_arg_ptr_defines(a) for a in args])}) = nullptr;'
            )

            def_ret_val = get_default_ret_val(ret_type)
            call_args = ", ".join([process_arg_calls(a) for a in args])

            if ret_type != "void" and not self_ret:
                if is_ref_in_api(ret_type):
                    new_lines.append(
                        f'{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET_REF_TO_SHARED({func_name}, {get_ref_class_name(ret_type)}, {def_ret_val}{", " if len(args) else ""}{call_args});'
                    )
                else:
                    if "ref_class" in ret:
                        new_lines.append(
                            f'{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET_GODOT_REF({func_name}, {ret["ref_class"]}, {def_ret_val}{", " if len(args) else ""}{call_args});'
                        )
                    else:
                        new_lines.append(
                            f'{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET({func_name}, {def_ret_val}{", " if len(args) else ""}{call_args});'
                        )
            else:
                if self_ret:
                    new_lines.append(f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_SELFRET({func_name}, {call_args});")
                    new_lines.append(f"{cls_indent}\treturn shared_from_this();")
                else:
                    new_lines.append(f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER({func_name}, {call_args});")
            new_lines.append(f"{cls_indent}}}")

            new_lines.append("")

        namespaces[cls] += new_lines

    shutil.copyfile("src/native_api/c_api_shared.hpp", os.path.join(out_folder, "c_api_shared.hpp"))
    text_data = lib_utils.read_all_text("src/native_api/templates/cpp/dd3d_cpp_api.hpp")
    lines = text_data.replace("\r\n", "\n").replace("\r", "\n").split("\n")

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

    api = generate_native_api(
        env, header_files, c_api_template, out_folder, src_out, ["camera3d", "control", "font", "viewport"]
    )
    if api == None:
        print("Couldn't get the Native API")
        return 110

    if not gen_cpp_api(env, api, os.path.join(out_folder, "cpp"), ["camera3d", "control", "font", "viewport"]):
        return 111
    print("The generation is finished!")
    return 0
