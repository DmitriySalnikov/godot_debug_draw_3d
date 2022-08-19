tool
extends EditorScript

const input_header = "debug_draw.h"
const out_gdscript = "res://addons/debug_draw_3d/debug_draw.gd"
const out_csharp = "res://addons/debug_draw_3d/DebugDrawCS.cs"
const out_gdscript_dummy = "res://addons/debug_draw_3d/debug_draw_dummy.gd"

class DebugDrawHeader:
	var int_constants: PoolStringArray
	var params: Array # Parameters
	var funcs: Array # Functions
	
	func _init() -> void:
		params.append(Parameters.new())
		funcs.append(Functions.new())
	
	func add_constant(s: String):
		int_constants.append(s)

class Parameters:
	var comments: PoolStringArray
	var type: String
	var name: String
	
	func add_comment(s: String):
		comments.append(s)

class Functions:
	var comments: PoolStringArray
	var ret_type: String
	var name: String
	var args: Array # Arguments
	
	func add_comment(s: String):
		comments.append(s)

class Arguments:
	var name: String
	var type: String
	var default = null

class CustomDefaultValue:
	var value
	func _init(val) -> void:
		value = val
	func _to_string() -> String:
		return str(value)

func _run() -> void:
	var start_time = OS.get_ticks_msec()
	
	var d = Directory.new()
	var f = File.new()
	
	if input_header.is_abs_path() and f.file_exists(input_header):
		f.open(input_header, File.READ)
	elif f.file_exists("res://" + input_header):
		f.open("res://" + input_header, File.READ)
	elif f.file_exists((get_script().resource_path).get_base_dir() + "/" + input_header):
		f.open((get_script().resource_path).get_base_dir() + "/"  + input_header, File.READ)
	else:
		printerr("No header file found with name: " + input_header)
		return
	
	var debug_draw_h_content = f.get_as_text()
	f.close()
	
	# replace CR by LF
	# on windows now can be LFLF replace by LF
	# replace empty strings by LF
	var header_lines = debug_draw_h_content.replace('\r', '\n').replace('\n\n', '\n').replace('\n\n', '\n').split('\n', false)
	for i in header_lines.size():
		header_lines[i] = header_lines[i].strip_edges()
	
	var data = _parse_header(header_lines)
	
	f.open(out_gdscript, File.WRITE)
	f.store_string(_generate_gdscript(data).join("\n"))
	f.close()
	
	f.open(out_gdscript_dummy, File.WRITE)
	f.store_string(_generate_gdscript(data, true).join("\n"))
	f.close()
	
	f.open(out_csharp, File.WRITE)
	f.store_string(_generate_csharp(data).join("\n"))
	f.close()
	
	print("Generation finished. Time elapsed: %s ms" % [OS.get_ticks_msec() - start_time])

enum ParsingStates{
	Init,
	Constants,
	Params,
	FindFuncs,
	Funcs,
}

func __remove_ref(s: String) -> String:
	if s.begins_with("Ref<"):
		return s.replace("Ref<", "").replace(">", "")
	return s

func __get_default_value(type: String, str_val: String):
	match type:
		"Color":
			return CustomDefaultValue.new("empty_color")
		"float":
			return float(str_val.replace("f", ""))
		"int":
			return int(str_val)
		"bool":
			return str_val == "true"
		"Variant":
			return str2var(str_val)
	
	return null

func _parse_header(lines: PoolStringArray) -> DebugDrawHeader:
	var res: DebugDrawHeader = DebugDrawHeader.new()
	
	var remap_types:= {
		"BlockPosition": "int",
		"FPSGraphTextFlags": "int",
		"real_t": "float",
		"int64_t": "int",
		"GraphParameters": "Reference",
	}
	
	var state: int = ParsingStates.Init
	for l in lines:
		match state:
			ParsingStates.Init:
				if l.begins_with("#define CONST_GET"):
					state = ParsingStates.Constants
			
			ParsingStates.Constants:
				if l.begins_with("CONST_GET("):
					var pair: PoolStringArray = l.replace("CONST_GET(", "").replace(");", "").split(", ")
					res.add_constant(pair.join("_"))
				elif l.begins_with("#pragma region Exposed Parameter Values"):
					state = ParsingStates.Params
				
			ParsingStates.Params:
				if l.begins_with("///"):
					res.params[res.params.size()-1].add_comment(l.replace("/// ", ""))
				elif l.begins_with("#pragma endregion // Exposed Parameter Values"):
					state = ParsingStates.FindFuncs
					res.params.remove(res.params.size()-1)
				elif not l.begins_with("//"):
					var split = l.split(" = ")
					if split.size() == 2:
						var param: Parameters = res.params[res.params.size()-1]
						var type_n_name = split[0].split(" ")
						param.type = __remove_ref(type_n_name[0] if not type_n_name[0] in remap_types else remap_types[type_n_name[0]])
						param.name = type_n_name[1].replace("*", "")
						
						res.params.append(Parameters.new())
			
			ParsingStates.FindFuncs:
				if l.begins_with("#pragma region Exposed Draw Functions"):
					state = ParsingStates.Funcs
			
			ParsingStates.Funcs:
				if l.begins_with("///"):
					res.funcs[res.funcs.size()-1].add_comment(l.replace("/// ", ""))
				elif l.begins_with("#"):
					pass
				else:
					var split = l.split(" ")
					if split.size() > 1:
						var fu: Functions = res.funcs[res.funcs.size()-1]
						fu.ret_type = __remove_ref(split[0])
						fu.ret_type = remap_types[fu.ret_type] if fu.ret_type in remap_types else fu.ret_type
						
						var func_def: String = l.substr(split[0].length() + 1)
						fu.name = func_def.split("(")[0]
						
						if fu.name.ends_with("_c"):
							continue
						
						var args: PoolStringArray = func_def.substr(fu.name.length() + 1).replace(");", "").replace("class ", "").replace("*", "").split(", ", false)
						
						for a in args:
							var arg = Arguments.new()
							var parts: PoolStringArray = a.split(" ", false)
							
							arg.type = parts[0]
							arg.type = remap_types[arg.type] if arg.type in remap_types else arg.type
							
							arg.name = parts[1]
							
							if parts.size() == 4:
								arg.default = __get_default_value(arg.type, parts[3])
							
							fu.args.append(arg)
						
						res.funcs.append(Functions.new())
	
	return res

func __gdscript_setget_names(type: String, name: String) -> PoolStringArray:
	if type == "bool":
		return PoolStringArray(["set_" + name, "is_" + name])
	else:
		return PoolStringArray(["set_" + name, "get_" + name])

func __gdscript_default_value_to_str(type: String, value) -> String:
	if type == "bool":
		return str(value).to_lower()
	if type == "Variant":
		if typeof(value) == TYPE_STRING:
			return "\"%s\"" % value
	
	return str(value)

func __gdscript_get_default_return_val(type: String) -> String:
	if type in ["Resource", "Reference", "Font", "Viewport", "CanvasItem"]:
		return "null"
	elif type in ["Dictionary", "PoolStringArray", "bool", "Color", "int", "float", "Vector2", "Vector3"]:
		return type + "()"
	return "WTF IS THIS??"

func _generate_gdscript(data: DebugDrawHeader, is_dummy_imp = false) -> PoolStringArray:
	var res: PoolStringArray
	res.append(
"""tool
extends Node

const empty_color = Color(0,0,0,0)
""")
	
	if !is_dummy_imp:
		res.append("var _debug_draw_3d: Node = null")
	
	res.append("\n### Constants\n")
	
	for c in data.int_constants:
		res.append("var %s = 0" % [c])
	
	if !is_dummy_imp:
		res.append(
"""

### Init

func _init() -> void:
	var f = File.new()
	if f.file_exists("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns"):
		_debug_draw_3d = load("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns").new()
		""")
		
		for c in data.int_constants:
			res.append("		%s = _debug_draw_3d.%s" % [c, c])
		
		
		res.append("""
func _enter_tree() -> void:
	if !Engine.editor_hint and _debug_draw_3d:
		if !_debug_draw_3d.get_singleton():
			add_child(_debug_draw_3d)

func _exit_tree() -> void:
	if _debug_draw_3d:
		_debug_draw_3d.queue_free()

func get_singleton() -> Node:
	if _debug_draw_3d:
		return _debug_draw_3d.get_singleton()
	else:
		return null""")
		
	else:
		res.append("""
func get_singleton() -> Node:
	return null""")
	
	res.append("\n\n### Parameters\n")
	if !is_dummy_imp:
		for p in data.params:
			for c in p.comments:
				res.append("## %s" % c)
			res.append("var %s: %s setget %s\n" % [p.name, p.type, __gdscript_setget_names(p.type, p.name).join(", ")])
	else:
		for p in data.params:
			res.append("var %s: %s" % [p.name, p.type])
	
	res.append("\n### Draw Functions\n")
	
	for f in data.funcs:
		if f.name.empty():
			continue
		
		if !is_dummy_imp:
			for c in f.comments:
				res.append("## " + c)
		
		var args: PoolStringArray
		var args_to_call: PoolStringArray
		for a in f.args:
			if a.type != "Variant":
				args.append("%s: %s%s" % [a.name, a.type, ("" if a.default == null else " = " +  __gdscript_default_value_to_str(a.type, a.default))])
			elif a.type == "Variant" and a.default != null:
				args.append("%s = %s" % [a.name, __gdscript_default_value_to_str(a.type, a.default)])
			else:
				args.append(a.name)
			args_to_call.append(a.name)
		
		if !is_dummy_imp:
			res.append("func %s(%s) -> %s:\n\tif _debug_draw_3d: %s_debug_draw_3d.%s(%s)\n%s" % [f.name, args.join(", "), f.ret_type, ("return " if f.ret_type != "void" else ""), f.name, args_to_call.join(", "), (("\telse: return %s\n" % [__gdscript_get_default_return_val(f.ret_type)]) if f.ret_type != "void" else "")])
		else:
			res.append("func %s(%s) -> %s:\n\t%s" % [f.name, args.join(", "), f.ret_type, (("return %s" % [__gdscript_get_default_return_val(f.ret_type)]) if f.ret_type != "void" else "pass")])
	
	if !is_dummy_imp:
		res.append("\n### Parameters Setget's\n")
		for p in data.params:
			var setgets: PoolStringArray = __gdscript_setget_names(p.type, p.name)
			res.append("""func %s(val):
	if _debug_draw_3d: _debug_draw_3d.%s = val

func %s() -> %s:
	if _debug_draw_3d: return _debug_draw_3d.%s
	else: return %s
""" % [setgets[0], p.name, setgets[1], p.type, p.name, __gdscript_get_default_return_val(p.type)]);
	
	return res


func __get_pascal_case_name(s : String) -> String:
	var split = s.split("_")
	for i in split.size():
		split[i][0] = split[i][0].to_upper()
	
	return split.join("")

func __csharp_default_value_to_str(type: String, value) -> String:
	if type == "bool":
		return str(value).to_lower()
	if type == "float":
		return str(value).to_lower()+"f"
	if type == "Variant":
		if typeof(value) == TYPE_STRING:
			return "\"%s\"" % value
	
	if value is CustomDefaultValue:
		if value.value == "empty_color":
			return "null"
	
	return str(value)

func _generate_csharp(data: DebugDrawHeader, is_dummy_imp = false) -> PoolStringArray:
	var remap_types:= {
		"PoolVector3Array": "Vector3[]",
		"PoolVector2Array": "Vector2[]",
		"PoolColorArray": "Color[]",
		"PoolStringArray": "string[]",
		"PoolIntArray": "int[]",
		"PoolRealArray": "float[]",
		"Color": "Color?",
		"String": "string",
	}
	
	var res: PoolStringArray
	res.append(
"""using Godot;
using Godot.Collections;

[Tool]
public class DebugDrawCS : Node
{
	static Color empty_color = new Color(0,0,0,0);
	static Node debug_draw_3d = null;
""")
	
	res.append("\n	/// Constants\n")
	
	for c in data.int_constants:
		res.append("	public static int %s { get; private set; } = 0;" % [c])
	
	res.append("\n\n	/// Parameters\n")
	
	for p in data.params:
		res.append("	/// <summary>")
		for c in p.comments:
			res.append("	/// %s" % c)
		res.append("	/// </summary>")
		res.append("	public static %s %s { get => (%s)debug_draw_3d?.Get(\"%s\"); set => debug_draw_3d?.Set(\"%s\", value); }\n" % [p.type, __get_pascal_case_name(p.name), p.type, p.name, p.name])
	
	res.append("\n	/// Init")
	
	res.append("""	public DebugDrawCS(){
		var f = new File();
		if (f.FileExists("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns")){
			debug_draw_3d = ResourceLoader.Load<NativeScript>(\"res://addons/debug_draw_3d/libs/debug_draw_3d.gdns\").New() as Node;
""")
	
	for c in data.int_constants:
		res.append("			%s = (int)debug_draw_3d.Get(\"%s\");" % [c, c])
	
	res.append("		}")
	res.append("	}")
	res.append("""
	public override void _EnterTree(){
		if (!Engine.EditorHint){
			if(debug_draw_3d.Call("get_singleton") == null)
				AddChild(debug_draw_3d);
		}
	}""")
	
	
	res.append("""
	protected override void Dispose(bool disposing){
		debug_draw_3d?.QueueFree();
		debug_draw_3d = null;
	}

	public override void _ExitTree(){
		debug_draw_3d?.QueueFree();
		debug_draw_3d = null;
	}

	public static Node GetSingleton(){
		return (Node)debug_draw_3d?.Call("get_singleton");
	}""")
	
	res.append("\n\n	/// Draw Functions\n")
	for f in data.funcs:
		if f.name.empty():
			continue
		
		res.append("	/// <summary>")
		var end_printed = false
		for c in f.comments:
			var cs : PoolStringArray = c.split(":")
			if cs.size() > 1:
				if !end_printed:
					end_printed = true
					res.append("	/// </summary>")
				res.append("	/// <param name=\"%s\">%s</param>" % [cs[0], cs[1].strip_edges()])
			else:
				res.append("	/// " + c)
		if !end_printed:
					res.append("	/// </summary>")
		
		var args: PoolStringArray
		var args_to_call: PoolStringArray
		if f.args.size():
			args_to_call.append("")
		
		for a in f.args:
			if a.type != "Variant":
				args.append("%s %s%s" % [(remap_types[a.type] if a.type in remap_types else a.type), a.name, ("" if a.default == null else " = " +  __csharp_default_value_to_str(a.type, a.default))])
			elif a.type == "Variant" and a.default != null:
				args.append("%s %s%s" % [a.type.replace("Variant", "object"), a.name, " = null"])
			else:
				args.append(a.name)
			
			if a.type == "Color":
				args_to_call.append("%s == null? empty_color : %s" % [a.name, a.name])
			else:
				args_to_call.append(a.name)
		res.append("	public static %s %s(%s){\n		%sdebug_draw_3d?.Call(\"%s\"%s);\n	}\n" % [(remap_types[f.ret_type] if f.ret_type in remap_types else f.ret_type), __get_pascal_case_name(f.name), args.join(", "), (("return (%s)" % (remap_types[f.ret_type] if f.ret_type in remap_types else f.ret_type)) if f.ret_type != "void" else ""), f.name, args_to_call.join(", ")])
	
	res.append("""}

""")
	
	return res
