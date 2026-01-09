/// //////////////////////////////////////////////////////////////
/// THIS FILE HAS BEEN GENERATED.
/// THE CHANGES IN THIS FILE WILL BE OVERWRITTEN AFTER THE UPDATE!
/// //////////////////////////////////////////////////////////////

#if !DEBUG && !FORCED_DD3D
#define _DD3D_RUNTIME_CHECK_ENABLED
#endif

#if (!DEBUG || FORCED_DD3D) || (DEBUG && !FORCED_DD3D)
#define _DD3D_COMPILETIME_CHECK_ENABLED
#endif

using Godot;
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using DD3DFuncLoadingResult = InternalDD3DApiLoaderUtils_.LoadingResult;

// Precision is replaced during generation, but can be replaced manually if necessary.
using real_t = float;

internal static class InternalDD3DApiLoaderUtils_
{
    const bool is_debug_enabled =
#if DEBUG
    true;
#else
    false;
#endif
    public static readonly bool IsCallEnabled = is_debug_enabled || OS.HasFeature("forced_dd3d");

    static readonly string log_prefix = "[DD3D] ";
    static readonly string get_funcs_is_double_name = "_get_native_functions_is_double";
    static readonly string get_funcs_hash_name = "_get_native_functions_hash";
    static readonly string get_classes_name = "_get_native_classes";
    static readonly string get_funcs_name = "_get_native_functions";

    public enum LoadingResult
    {
        None,
        Success,
        Failure
    };

    static GodotObject dd3d_c = null;
    static bool failed_to_load = false;
    static System.Collections.Generic.Dictionary<Type, int> dd3d_class_sizes = new();

    static GodotObject get_dd3d()
    {
        if (failed_to_load)
            return null;

        if (dd3d_c != null)
            return dd3d_c;

        if (Engine.HasSingleton("DebugDrawManager"))
        {
            GodotObject dd3d = Engine.GetSingleton("DebugDrawManager");

            if (!dd3d.HasMethod(get_funcs_is_double_name))
            {
                GD.PrintErr(log_prefix, get_funcs_is_double_name, " not found!");
                failed_to_load = true;
                return null;
            }

            bool is_double = OS.HasFeature("double");

            if (dd3d.Call(get_funcs_is_double_name).AsBool() != is_double)
            {
                GD.PrintErr(log_prefix, "The precision of Vectors and Matrices of DD3D and the current library do not match!");
                failed_to_load = true;
                return null;
            }

            foreach (string name in new string[] { get_funcs_hash_name, get_classes_name, get_funcs_name })
            {
                if (!dd3d.HasMethod(name))
                {
                    GD.PrintErr(log_prefix, name, " not found!");
                    failed_to_load = true;
                    return null;
                }
            }

            dd3d_c = dd3d;
            return dd3d_c;
        }

        return null;
    }

    static bool _load_function<dlgt_T>(string name, ref dlgt_T func)
    {
        GodotObject dd3d = get_dd3d();
        if (dd3d != null)
        {
            Godot.Collections.Dictionary api = dd3d.Call(get_funcs_name).AsGodotDictionary();
            if (api.ContainsKey(name))
            {
                Godot.Collections.Dictionary func_dict = api[name].AsGodotDictionary();

                // TODO: signature check?

                var ptr = new IntPtr(func_dict["ptr"].AsInt64());
                func = Marshal.GetDelegateForFunctionPointer<dlgt_T>(ptr);
                return true;
            }
            else
            {
                GD.PrintErr(log_prefix, "!!! FUNCTION NOT FOUND !!! function name: ", name);
                return false;
            }
        }
        return false;
    }

    static int _get_class_size(Type cls)
    {
        GodotObject dd3d = get_dd3d();
        if (dd3d != null)
        {
            Godot.Collections.Dictionary api = dd3d.Call(get_classes_name).AsGodotDictionary();
            if (api.ContainsKey(cls.Name))
            {
                Godot.Collections.Dictionary cls_dict = api[cls.Name].AsGodotDictionary();

                var size = cls_dict["size"].AsInt32();
                dd3d_class_sizes[cls] = size;
                return size;
            }
            else
            {
                GD.PrintErr(log_prefix, "!!! CLASS NOT FOUND !!! class name: ", cls.Name);
                return -1; // crash
            }
        }
        return -1; // crash
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool LoadFunction<dlgt_T>(string name, ref dlgt_T func, ref DD3DFuncLoadingResult result)
    {
        if (result == DD3DFuncLoadingResult.None)
            result = _load_function(name, ref func) ? DD3DFuncLoadingResult.Success : DD3DFuncLoadingResult.Failure;
        if (result == DD3DFuncLoadingResult.Failure)
            return false;
        return true;
    }

    public static int GetDD3DClassSize(Type cls)
    {
        if (dd3d_class_sizes.TryGetValue(cls, out int val))
            return val;
        else
            return _get_class_size(cls);
    }

    // GENERATOR_DD3D_API_DEFAULT_VALUES
}

// Start of the generated API
// GENERATOR_DD3D_API_FUNCTIONS
// End of the generated API
