#!/usr/bin/env python3

from SCons.Script.SConscript import SConsEnvironment
from patches import unity_tools

import SCons
import os, shutil, json, re, hashlib
import lib_utils


def insert_lines_at_mark(lines: list, mark: str, insert_lines: list):
    insert_mark = mark
    insert_index = -1
    for idx, line in enumerate(lines):
        if line.endswith(mark):
            insert_index = idx
            break
    lines.pop(insert_index)
    lines[insert_index:insert_index] = insert_lines


def get_api_functions(headers: list) -> dict:
    classes = {}

    for header in headers:
        print(header)
        text_data = lib_utils.read_all_text(header)
        lines = text_data.replace("\r\n", "\n").replace("\r", "\n").split("\n")
        lines = [line.strip() for line in lines if len(line.strip())]

        functions = {}
        current_class = ""

        is_singleton = False
        is_refcounted = False

        for idx, line in enumerate(lines):
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
                classes[current_class] = {"functions": functions, "type": class_type, "docs": docs}
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

                args_str = line[line.find("(") + 1 : line.rfind(")")]
                args = []

                if not is_singleton:
                    args.append("void *inst")

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
                for a in args:
                    arg_name_match = re.search(r"\b(\w+)\s*=", a)
                    if arg_name_match:
                        new_dict = {
                            "name": arg_name_match.group(1).strip(),
                            "type": a[: a.index(arg_name_match.group(1))].strip(),
                            "default": a[a.find("=") + 1 :].strip(),
                        }
                        new_dict["c_type"] = new_dict["type"].replace("&", "").strip()
                        args_dict.append(new_dict)
                    else:
                        arg_name_match = re.search(r"\b(\w+)$", a)
                        new_dict = {
                            "name": arg_name_match.group(1).strip(),
                            "type": a[: a.index(arg_name_match.group(1))].strip(),
                        }
                        new_dict["c_type"] = new_dict["type"].replace("&", "").strip()
                        args_dict.append(new_dict)

                fun_dict = {
                    "return": ret_type,
                    "self_return": is_self_return,
                    "args": args_dict,
                    "name": func_name,
                    "docs": docs,
                }
                functions[f"{current_class}_{func_name}"] = fun_dict
    return classes


def generate_native_api(c_api_template: str, out_folder: str, src_out: list) -> dict:
    # TODO load all headers
    # and better to load all processed headers
    header_files = ["src/3d/config_3d.h", "src/3d/config_scope_3d.h", "src/3d/debug_draw_3d.h", "src/3d/stats_3d.h",]
    classes = get_api_functions(header_files)

    new_funcs = []
    new_func_regs = []
    new_ref_clears = []

    for cls in classes:
        is_refcounted = classes[cls]["type"] == "refcounted"
        if not is_refcounted:
            continue

        new_funcs.append(f"struct {cls}_NAPIWrapper {{")
        new_funcs.append(f"\tRef<{cls}> ref;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"static std::unordered_set<{cls}_NAPIWrapper*> {cls}_NAPIWrapper_storage;")
        new_funcs.append("")
        new_funcs.append(f"static void* {cls}_create() {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst = new {cls}_NAPIWrapper{{Ref<{cls}>(memnew({cls}))}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst);")
        new_funcs.append("\treturn inst;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"static void* {cls}_create_nullptr() {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst = new {cls}_NAPIWrapper{{}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst);")
        new_funcs.append("\treturn inst;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"static void* {cls}_create_from_ref(Ref<{cls}> ref) {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(f"\tauto* inst = new {cls}_NAPIWrapper{{ref}};")
        new_funcs.append(f"\t{cls}_NAPIWrapper_storage.insert(inst);")
        new_funcs.append("\treturn inst;")
        new_funcs.append("};")
        new_funcs.append("")
        new_funcs.append(f"static void {cls}_destroy(void *inst) {{")
        new_funcs.append("\tZoneScoped;")
        new_funcs.append(
            f"\tif (const auto it = {cls}_NAPIWrapper_storage.find(static_cast<{cls}_NAPIWrapper*>(inst)); it != {cls}_NAPIWrapper_storage.end()){{"
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
            ret_type = func["return"]
            self_ret = func["self_return"]
            is_ret_custom_ref = is_ref_in_api(ret_type)

            args = func["args"]

            def process_arg_defines(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"void *{name}"
                return f"{type} {name}"

            def process_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"static_cast<{get_ref_class_name(type)}_NAPIWrapper*>({name})->ref"
                return f"{name}"

            # func define
            new_func_define_args = ", ".join([process_arg_defines(a) for a in args])
            new_funcs.append(
                f'static {"void *" if is_ret_custom_ref else ret_type} {func_name}({new_func_define_args}) {{'
            )
            new_funcs.append("\tZoneScoped;")

            if is_singleton:
                call_args = ", ".join([process_arg_calls(a) for a in args])

                if ret_type != "void":
                    if is_ret_custom_ref:
                        new_funcs.append(
                            f"\treturn {get_ref_class_name(ret_type)}_create_from_ref({cls}::get_singleton()->{func_orig_name}({call_args}));"
                        )
                    else:
                        new_funcs.append(f"\treturn {cls}::get_singleton()->{func_orig_name}({call_args});")
                else:
                    new_funcs.append(f"\t{cls}::get_singleton()->{func_orig_name}({call_args});")
            else:
                call_args = ", ".join([process_arg_calls(a) for a in args[1:]])

                if ret_type != "void":
                    new_funcs.append(
                        f"\treturn static_cast<{cls}_NAPIWrapper*>(inst)->ref->{func_orig_name}({call_args});"
                    )
                else:
                    new_funcs.append(f"\tstatic_cast<{cls}_NAPIWrapper*>(inst)->ref->{func_orig_name}({call_args});")
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


def gen_cpp_api(env: SConsEnvironment, api: dict, out_folder: str, additional_include_classes: list = []) -> bool:
    os.makedirs(out_folder, exist_ok=True)
    classes = dict(api["classes"])
    is_namespace_a_class = {}
    namespaces = {}
    fwd_decls = []


    for cls in classes:
        functions = classes[cls]["functions"]
        cls_is_class = classes[cls]["type"] != "singleton"
        if cls_is_class:
            fwd_decls.append(f'class {cls};')

    for cls in classes:
        functions = classes[cls]["functions"]
        cls_is_class = classes[cls]["type"] != "singleton"
        cls_indent = "\t" if cls_is_class else ""

        if cls not in namespaces:
            is_namespace_a_class[cls] = cls_is_class
            namespaces[cls] = []

        if cls_is_class:
            con_lines = []
            con_lines.append(f"{cls_indent}void *inst;")
            con_lines.append("")
            con_lines.append(f"public:")
            con_lines.append(f"{cls_indent}{cls}(void *inst) :")
            con_lines.append(f"{cls_indent}\t\tinst(inst) {{}}")
            con_lines.append("")
            con_lines.append(f"{cls_indent}{cls}(bool instantiate = true) :")
            con_lines.append(f"{cls_indent}\t\tinst(instantiate ? create() : create_nullptr()) {{}}")
            con_lines.append("")
            con_lines.append(f"{cls_indent}~{cls}() {{ destroy(inst); }}")
            con_lines.append("")
            con_lines.append(f"{cls_indent}operator void *() const {{ return inst; }}")
            con_lines.append("")
            namespaces[cls] += con_lines

            functions[f"{cls}_create"] = {
                "return": "void *",
                "self_return": False,
                "args": [],
                "name": "create",
                "docs": [],
                "private": True,
            }

            functions[f"{cls}_create_nullptr"] = {
                "return": "void *",
                "self_return": False,
                "args": [],
                "name": "create_nullptr",
                "docs": [],
                "private": True,
            }

            functions[f"{cls}_destroy"] = {
                "return": "void",
                "self_return": False,
                "args": [
                    {
                        "name": "inst",
                        "type": "void *",
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
            args = func["args"]
            self_ret = func["self_return"]
            is_private = func.get("private", False)

            if self_ret and ret == "void":
                ret = f"{cls} *"

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
                return ret_type

            def process_arg_defines(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"std::shared_ptr<{get_ref_class_name(type)}>({name})"
                return f"{type} {name}"

            def process_arg_ptr_defines(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"void *"
                return f"{type}"

            def process_arg_calls(arg: dict):
                type = arg["type"]
                name = arg["name"]
                if is_ref_in_api(type):
                    return f"*{name}"
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

            # Name
            if cls_is_class and not is_private:
                inner_args = ", ".join([process_arg_defines(a) for a in args[1:]])
                new_lines.append(
                    f'{cls_indent}{get_ref_to_native_name(ret)} {func_orig_name.removesuffix("_selfreturn")}({inner_args}) {{'
                )
            else:
                inner_args = ", ".join([process_arg_defines(a) for a in args])
                new_lines.append(
                    f'{cls_indent}static {get_ref_to_native_name(ret)} {func_orig_name.removesuffix("_selfreturn")}({inner_args}) {{'
                )

            # Func pointer
            new_lines.append(
                f'{cls_indent}\tstatic {"void" if self_ret else get_ref_to_void_arg_name(ret)}(*{func_name})({", ".join([process_arg_ptr_defines(a) for a in args])}) = nullptr;'
            )

            def_ret_val = get_default_ret_val(ret)
            call_args = ", ".join([process_arg_calls(a) for a in args])

            if ret != "void" and not self_ret:
                if is_ref_in_api(ret):
                    new_lines.append(
                        f'{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET_REF_TO_SHARED({func_name}, {get_ref_class_name(ret)}, {def_ret_val}{", " if len(args) else ""}{call_args});'
                    )
                else:
                    new_lines.append(
                        f'{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_RET({func_name}, {def_ret_val}{", " if len(args) else ""}{call_args});'
                    )
            else:
                if self_ret:
                    new_lines.append(f"{cls_indent}\tLOAD_AND_CALL_FUNC_POINTER_SELFRET({func_name}, {call_args});")
                    new_lines.append(f"{cls_indent}\treturn this;")
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


def gen_apis(env: SConsEnvironment, c_api_template: str, out_folder: str, src_out: list):
    print("Generating native API!")
    os.makedirs(out_folder, exist_ok=True)

    api = generate_native_api(c_api_template, out_folder, src_out)
    if api == None:
        print("Couldn't get the Native API")
        return 110

    if not gen_cpp_api(env, api, os.path.join(out_folder, "cpp"), ["camera3d"]):
        return 111
    return 0
