/// //////////////////////////////////////////////////////////////
/// THIS FILE HAS BEEN GENERATED.
/// THE CHANGES IN THIS FILE WILL BE OVERWRITTEN AFTER THE UPDATE!
/// //////////////////////////////////////////////////////////////

using Godot;
using Godot.Collections;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

using real_t = float;
using DD3DFuncLoadingResult = _DebugDrawUtils_.LoadingResult;
using System.Runtime.CompilerServices;

internal static class _DebugDrawUtils_
{
    static readonly string log_prefix = "[DD3D] ";
    static readonly string get_funcs_is_double_name = "_get_native_functions_is_double";
    static readonly string get_funcs_hash_name = "_get_native_functions_hash";
    static readonly string get_funcs_name = "_get_native_functions";

    public enum LoadingResult
    {
        None,
        Success,
        Failure
    };

    static GodotObject dd3d_c = null;
    static bool failed_to_load = false;

    public static GodotObject get_dd3d()
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

            if (!dd3d.HasMethod(get_funcs_hash_name))
            {
                GD.PrintErr(log_prefix, get_funcs_hash_name, " not found!");
                failed_to_load = true;
                return null;
            }

            if (!dd3d.HasMethod(get_funcs_name))
            {
                GD.PrintErr(log_prefix, get_funcs_name, " not found!");
                failed_to_load = true;
                return null;
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
            Dictionary api = dd3d.Call(get_funcs_name).AsGodotDictionary();
            if (api.ContainsKey(name))
            {
                Dictionary func_dict = api[name].AsGodotDictionary();

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

    [MethodImpl(MethodImplOptions.AggressiveInlining)] // TODO: compare performance without the attribute
    public static bool LoadFunction<dlgt_T>(string name, ref dlgt_T func, ref DD3DFuncLoadingResult result)
    {
        if (result == DD3DFuncLoadingResult.None)
            result = _load_function(name, ref func) ? DD3DFuncLoadingResult.Success : DD3DFuncLoadingResult.Failure;
        if (result == DD3DFuncLoadingResult.Failure)
            return false;
        return true;
    }

    // GENERATOR_DD3D_API_DEFAULT_VALUES
}

// Start of the generated API
// GENERATOR_DD3D_API_FUNCTIONS
// End of the generated API
