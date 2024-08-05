#pragma once
#ifdef TOOLS_ENABLED
#include "utils/compiler.h"

#include <map>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/typed_array.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class GenerateCSharpBindingsPlugin {
	class IndentGuard {
		GenerateCSharpBindingsPlugin *owner = nullptr;

	public:
		IndentGuard(){};
		IndentGuard(GenerateCSharpBindingsPlugin *_owner);
		~IndentGuard();
	};

	struct ArgumentData {
		String name;
		String type_name;
		Variant::Type type;
		bool is_void;
		bool is_enum;

		ArgumentData() :
				name("[NOT INITIALIZED]"),
				type_name("[NOT INITIALIZED]"),
				type(Variant::NIL),
				is_void(false),
				is_enum(false) {}

		ArgumentData(const String &_name, Variant::Type _type, const String &_type_name, bool _enum = false) :
				name(_name),
				type_name(_type_name),
				type(_type),
				is_void(_type_name == "void"),
				is_enum(_enum) {}
	};

	struct DefaultData {
		String name;
		String type_name;
		String arg_string;
		bool is_need_remap;
		bool is_nullable;

		DefaultData() :
				name("[NOT INITIALIZED]"),
				type_name("[NOT INITIALIZED]"),
				arg_string("[NOT INITIALIZED]"),
				is_need_remap(false),
				is_nullable(false) {}

		DefaultData(const String &_name, const String &_type_name, bool need_remap, const String &_arg_str, bool nullable = false) :
				name(_name),
				type_name(_type_name),
				arg_string(_arg_str),
				is_need_remap(need_remap),
				is_nullable(nullable) {}

		DefaultData(const ArgumentData &arg, bool need_remap, const String &_arg_str, bool nullable = false) :
				name(arg.name),
				type_name(arg.type_name),
				arg_string(_arg_str),
				is_need_remap(need_remap),
				is_nullable(nullable) {}

		bool is_equal_data(const DefaultData &other) const {
			return type_name == other.type_name &&
				   arg_string == other.arg_string &&
				   is_need_remap == other.is_need_remap &&
				   is_nullable == other.is_nullable;
		}
	};

	String output_directory = "res://addons/debug_draw_3d/gen/csharp";
	String api_file_name = "DebugDrawGeneratedAPI.generated.cs";
	String log_file_name = "log.txt";
	String indent_template = "    ";
	String indent;
	Ref<FileAccess> opened_file;
	Ref<FileAccess> opened_log_file;
	TypedArray<String> property_method_prefix;

	TypedArray<StringName> generate_for_classes;
	TypedArray<StringName> avoid_caching_for_classes;

	typedef std::map<StringName, std::vector<String> > extend_class_strings;
	extend_class_strings additional_statics_for_classes;
	extend_class_strings override_disposable_for_classes;

	PackedStringArray singletons;
	bool is_shift_pressed = false;
	bool is_generate_unload_event = false;
	typedef std::map<String, DefaultData> remap_data;

	std::map<Variant::Type, String> types_map = {
		{ Variant::NIL, "NILL" },
		{ Variant::BOOL, "bool" },
		{ Variant::INT, "int" },
		{ Variant::FLOAT, "float" },
		{ Variant::VECTOR2, "Vector2" },
		{ Variant::RECT2, "Rect2" },
		{ Variant::VECTOR3, "Vector3" },
		{ Variant::TRANSFORM2D, "Transform2D" },
		{ Variant::VECTOR4, "Vector4" },
		{ Variant::PLANE, "Plane" },
		{ Variant::QUATERNION, "Quaternion" },
		{ Variant::AABB, "Aabb" },
		{ Variant::BASIS, "Basis" },
		{ Variant::COLOR, "Color" },
		{ Variant::PROJECTION, "Projection" },
		{ Variant::TRANSFORM3D, "Transform3D" },
		{ Variant::STRING_NAME, "StringName" },
		{ Variant::NODE_PATH, "NodePath" },
		{ Variant::RID, "Rid" },
		{ Variant::CALLABLE, "Callable" },
		{ Variant::SIGNAL, "Signal" },

		// not matched
		{ Variant::STRING, "string" },
		{ Variant::VECTOR2I, "Vector2I" },
		{ Variant::RECT2I, "Rect2I" },
		{ Variant::VECTOR3I, "Vector3I" },
		{ Variant::VECTOR4I, "Vector4I" },
		{ Variant::OBJECT, "GodotObject" },
		{ Variant::DICTIONARY, "Godot.Collections.Dictionary" },
		{ Variant::ARRAY, "Godot.Collections.Array" },
		{ Variant::PACKED_BYTE_ARRAY, "byte[]" },
		{ Variant::PACKED_INT32_ARRAY, "int[]" },
		{ Variant::PACKED_INT64_ARRAY, "long[]" },
		{ Variant::PACKED_FLOAT32_ARRAY, "float[]" },
		{ Variant::PACKED_FLOAT64_ARRAY, "double[]" },
		{ Variant::PACKED_STRING_ARRAY, "string[]" },
		{ Variant::PACKED_VECTOR2_ARRAY, "Vector2[]" },
		{ Variant::PACKED_VECTOR3_ARRAY, "Vector3[]" },
		{ Variant::PACKED_COLOR_ARRAY, "Color[]" },
	};

public:
	bool is_need_to_update();
	void generate();

private:
	void generate_header();

	void generate_class(const StringName &cls, remap_data &remapped_data);
	void generate_class_utilities(const remap_data &remapped_data);

	void generate_wrapper(const StringName &cls, bool is_static, bool inheritance = false);
	void generate_constants(const StringName &cls);
	void generate_enum(const StringName &cls, const StringName &enm);
	void generate_method(const StringName &cls, const Dictionary &method, bool is_static, remap_data &remapped_data);
	void generate_default_arguments_remap(const remap_data &remapped_data);
	void generate_properties(const StringName &cls, const TypedArray<Dictionary> &props, std::map<String, ArgumentData> setget_map, bool is_static);
	ArgumentData argument_parse(const Dictionary &arg, bool is_return = false);
	ArgumentData argument_parse(const StringName &class_name, const String &name, const Variant::Type type);
	std::vector<DefaultData> arguments_parse_values(const TypedArray<Dictionary> &args, const Array &def_args, remap_data &remapped_data);
	DefaultData arguments_get_formatted_value(const ArgumentData &arg_data, const Variant &def_val);
	String arguments_string_decl(const TypedArray<Dictionary> &args, bool with_defaults, std::vector<DefaultData> def_args_data = {});
	String arguments_string_call(const TypedArray<Dictionary> &args, const std::vector<DefaultData> &def_remap);
	void line(const String &str = "", int indent_override = -1);
	void log(const String &str = "", const int &indent = 0);
	void log_warning(const String &str = "", const int &indent = 0);
	IndentGuard tab();
};

#endif
