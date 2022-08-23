tool
extends EditorScript

const debug_draw_header = "debug_draw.h"
const data_graph_header = "data_graphs.h"
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
	
	var dir_path = ""
	if debug_draw_header.is_abs_path() and f.file_exists(debug_draw_header):
		pass
	elif f.file_exists("res://" + debug_draw_header):
		dir_path = "res://"
	elif f.file_exists((get_script().resource_path).get_base_dir() + "/" + debug_draw_header):
		dir_path = (get_script().resource_path).get_base_dir() + "/"
	else:
		printerr("No header file found with name: " + debug_draw_header)
		return
	
	f.open(dir_path + debug_draw_header, File.READ)
	var debug_draw_h_content = f.get_as_text()
	f.close()
	
	f.open(dir_path + data_graph_header, File.READ)
	var data_graphs_h_content = f.get_as_text()
	f.close()
	
	# replace CR by LF
	# on windows now can be LFLF replace by LF
	# replace empty strings by LF
	var debug_draw_lines = debug_draw_h_content.replace('\r', '\n').replace('\n\n', '\n').replace('\n\n', '\n').split('\n', false)
	for i in debug_draw_lines.size():
		debug_draw_lines[i] = debug_draw_lines[i].strip_edges()
	
	var data_graphs_lines = data_graphs_h_content.replace('\r', '\n').replace('\n\n', '\n').replace('\n\n', '\n').split('\n', false)
	for i in data_graphs_lines.size():
		data_graphs_lines[i] = data_graphs_lines[i].strip_edges()
	
	var debug_draw_data = _parse_header(debug_draw_lines)
	var data_graphs_data = _parse_header(data_graphs_lines)
	
	f.open(out_gdscript, File.WRITE)
	f.store_string(_generate_gdscript(debug_draw_data, data_graphs_data).join("\n"))
	f.close()
	
	f.open(out_gdscript_dummy, File.WRITE)
	f.store_string(_generate_gdscript(debug_draw_data, data_graphs_data, true).join("\n"))
	f.close()
	
	f.open(out_csharp, File.WRITE)
	f.store_string(_generate_csharp(debug_draw_data, data_graphs_data).join("\n"))
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
	}
	
	var state: int = ParsingStates.Init
	for l in lines:
		match state:
			ParsingStates.Init:
				if l.begins_with("#define CONST_GET"):
					state = ParsingStates.Constants
				
				# For data graphs
				if l.begins_with("GODOT_CLASS(GraphParameters, Reference);"):
					state = ParsingStates.Params
			
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
				
				# For data graphs
				elif l.begins_with("public:"):
					res.params.remove(res.params.size()-1)
					return res
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




#########################################################################################################################################################################################################################################################################




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
	if type in ["Resource", "Reference", "Font", "Viewport", "CanvasItem", "GraphParameters"]:
		return "null"
	elif type in ["Dictionary", "PoolStringArray", "bool", "Color", "int", "float", "Vector2", "Vector3"]:
		return type + "()"
	elif type == "String":
		return "\"\""
	return "WTF IS THIS??"

func _generate_gdscript(data: DebugDrawHeader, data_graphs_data: DebugDrawHeader, is_dummy_imp = false) -> PoolStringArray:
	var res: PoolStringArray
	res.append(
"""tool
extends Node

const empty_color = Color(0,0,0,0)""")
	
	if !is_dummy_imp:
		res.append("var _debug_draw_3d: Node = null")
	
	
	res.append("\n\n### Constants\n")
	
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
	
	
	res.append("\n\n#######################################################################\n#######################################################################\n\n")
	
	
	res.append("class GraphParameters:")
	if !is_dummy_imp:
		res.append("""	var orig_ref : Reference
	func _init(ref : Reference) -> void:
		orig_ref = ref
	""")
	
	if !is_dummy_imp:
		for p in data_graphs_data.params:
				for c in p.comments:
					res.append("	## %s" % c)
				res.append("	var %s: %s setget %s\n	" % [p.name, p.type, __gdscript_setget_names(p.type, p.name).join(", ")])
		
		res.append("	")
		
		for p in data_graphs_data.params:
			var setgets: PoolStringArray = __gdscript_setget_names(p.type, p.name)
			if !is_dummy_imp:
				res.append("""	func %s(val):
		if orig_ref: orig_ref.%s = val
	
	func %s() -> %s:
		if orig_ref: return orig_ref.%s
		else: return %s
	""" % [setgets[0], p.name, setgets[1], p.type, p.name, __gdscript_get_default_return_val(p.type)]);
		
	else:
		for p in data_graphs_data.params:
			res.append("	var %s: %s" % [p.name, p.type])
	
	return res




#########################################################################################################################################################################################################################################################################





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

var __csharp_remap := {
	"PoolVector3Array": "Vector3[]",
	"PoolVector2Array": "Vector2[]",
	"PoolColorArray": "Color[]",
	"PoolStringArray": "string[]",
	"PoolIntArray": "int[]",
	"PoolRealArray": "float[]",
	"Color": "Color?",
	"String": "string",
}

var __csharp_remap_params := {
	"PoolVector3Array": "Vector3[]",
	"PoolVector2Array": "Vector2[]",
	"PoolColorArray": "Color[]",
	"PoolStringArray": "string[]",
	"PoolIntArray": "int[]",
	"PoolRealArray": "float[]",
	"String": "string",
}

var __csharp_remap_defs := {
	"Dictionary": "new Dictionary()",
	"String": "\"\"",
	"PoolStringArray": "new string[0]",
}

func _csharp_remap_types(type : String) -> String:
	if type in __csharp_remap:
		return __csharp_remap[type]
	return type

func _csharp_remap_params(type : String) -> String:
	if type in __csharp_remap_params:
		return __csharp_remap_params[type]
	return type

func _csharp_remap_ret_conv(type : String) -> String:
	var remap_ret_conv:={
		"GraphParameters": "GraphParameters)(Reference",
	}
	for r in __csharp_remap:
		remap_ret_conv[r] = __csharp_remap[r]
	
	if type in remap_ret_conv:
		return remap_ret_conv[type]
	return type

func _csharp_remap_def(type : String) -> String:
	if type in __csharp_remap_defs:
		return __csharp_remap_defs[type]
	return "default"


func _generate_csharp(data: DebugDrawHeader, data_graphs_data: DebugDrawHeader) -> PoolStringArray:
	var res: PoolStringArray
	res.append(
"""using Godot;
using Godot.Collections;

[Tool]
public class DebugDrawCS : Node
{
#if DEBUG
	static Color empty_color = new Color(0,0,0,0);
	static Node debug_draw_3d = null;
#endif
""")
	
	
	res.append("#region Constants")
	
	res.append("#if DEBUG")
	for c in data.int_constants:
		res.append("	public static int %s { get; private set; } = 0;" % [c])
	res.append("#else")
	for c in data.int_constants:
		res.append("	public const int %s = 0;" % [c])
	res.append("#endif")
	
	res.append("#endregion Constants\n\n")
	
	
	res.append("#region Parameters")
	
	res.append("#if DEBUG")
	
	for p in data.params:
		res.append("	/// <summary>")
		for c in p.comments:
			res.append("	/// %s" % c)
		res.append("	/// </summary>")
		
		res.append("	public static %s %s { get => (%s)debug_draw_3d?.Get(\"%s\"); set => debug_draw_3d?.Set(\"%s\", value); }\n" % [_csharp_remap_params(p.type), __get_pascal_case_name(p.name), p.type, p.name, p.name])
	
	res.append("#else")
	
	for p in data.params:
		res.append("	public static %s %s = default;" % [_csharp_remap_params(p.type), __get_pascal_case_name(p.name)])
	
	res.append("#endif\n")
	res.append("#endregion Parameters\n\n")
	
	
	res.append("#region Init")
	
	res.append("#if DEBUG")
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
	res.append("#endif")
	
	res.append("#endregion Init\n\n")
	
	
	res.append("#region Draw Functions")
	
	var real_impl = PoolStringArray()
	var dummy_impl = PoolStringArray()
	
	for f in data.funcs:
		if f.name.empty():
			continue
		
		real_impl.append("	/// <summary>")
		var end_printed = false
		for c in f.comments:
			var cs : PoolStringArray = c.split(":")
			if cs.size() > 1:
				if !end_printed:
					end_printed = true
					real_impl.append("	/// </summary>")
				real_impl.append("	/// <param name=\"%s\">%s</param>" % [cs[0], cs[1].strip_edges()])
			else:
				real_impl.append("	/// " + c)
		if !end_printed:
					real_impl.append("	/// </summary>")
		
		var args: PoolStringArray
		var args_to_call: PoolStringArray
		if f.args.size():
			args_to_call.append("")
		
		for a in f.args:
			if a.type != "Variant":
				args.append("%s %s%s" % [_csharp_remap_types(a.type), a.name, ("" if a.default == null else " = " +  __csharp_default_value_to_str(a.type, a.default))])
			elif a.type == "Variant" and a.default != null:
				args.append("%s %s%s" % [a.type.replace("Variant", "object"), a.name, " = null"])
			else:
				args.append(a.name)
			
			if a.type == "Color":
				args_to_call.append("%s == null? empty_color : %s" % [a.name, a.name])
			else:
				args_to_call.append(a.name)
		
		real_impl.append("""	public static %s %s(%s){
		%sdebug_draw_3d?.Call(\"%s\"%s);
	}
""" % [
			_csharp_remap_types(f.ret_type),
			__get_pascal_case_name(f.name),
			args.join(", "),
			(("return (%s)" % _csharp_remap_ret_conv(f.ret_type)) if f.ret_type != "void" else ""),
			f.name,
			args_to_call.join(", "),
		])
		
		dummy_impl.append("	public static %s %s(%s) %s" % [
			_csharp_remap_types(f.ret_type),
			__get_pascal_case_name(f.name),
			args.join(", "),
			( "=> %s;" % _csharp_remap_def(f.ret_type) if f.ret_type != "void" else "{}"),
		])
	
	res.append("#if DEBUG")
	res.append_array(real_impl)
	res.append("#else")
	res.append("#pragma warning disable IDE0060 // Remove unused parameter")
	res.append_array(dummy_impl)
	res.append("#pragma warning restore IDE0060 // Remove unused parameter")
	res.append("#endif")
	
	res.append("#endregion Draw Functions\n\n")
	
	
	res.append("	///////////////////////////////////////////////////////////////////////\n	///////////////////////////////////////////////////////////////////////\n	///////////////////////////////////////////////////////////////////////\n	///////////////////////////////////////////////////////////////////////\n\n")	
	
	res.append("	public class GraphParameters\n\t{")
	
	res.append("#if DEBUG")
	res.append(
"""		readonly Reference orig_ref;
		public GraphParameters(Reference reference) { orig_ref = reference; }
		public static explicit operator GraphParameters(Reference reference) => reference != null ? new GraphParameters(reference) : null;
""")
	res.append("#else")
	res.append(
"""#pragma warning disable IDE0060 // Remove unused parameter
		public GraphParameters(Reference reference) { }
#pragma warning restore IDE0060 // Remove unused parameter""")
	res.append("#endif\n\n")
	
	
	res.append("#region Parameters")
	res.append("#if DEBUG")
	
	for p in data_graphs_data.params:
		res.append("		/// <summary>")
		for c in p.comments:
			res.append("		/// %s" % c)
		res.append("		/// </summary>")
		
		res.append("		public %s %s { get => (%s)orig_ref?.Get(\"%s\"); set => orig_ref?.Set(\"%s\", value); }\n" % [_csharp_remap_params(p.type), __get_pascal_case_name(p.name), _csharp_remap_params(p.type), p.name, p.name])
	
	res.append("#else")
	
	for p in data_graphs_data.params:
		res.append("		public %s %s = %s;" % [_csharp_remap_params(p.type), __get_pascal_case_name(p.name), _csharp_remap_def(p.type)])
	
	res.append("#endif")
	res.append("#endregion Parameters")
	
	res.append("	}")
	
	
	res.append("""}

""")
	
	return res
