# Native API

`DebugDraw3D` can also be used in languages other than `GDScript`.

For quite some time now, `DebugDraw3D` has been available for use from `C#`. However, prior to version `1.7.0`, this was a very slow solution implemented using `GodotObject.Call`.

Starting with version `1.7.0`, a native API was implemented for interaction with other languages. This interface is a `C API` that is initialized via `Object.Call` and obtains function addresses from `Dictionary`. Descriptions of all functions are stored in `addons/debug_draw_3d/native_api/api.json` after building the project via `scons`. So it should be possible to generate an API for any language that supports the `C API` and is integrated into Godot.

Currently, support for `C#` and `C++` has been implemented. Both bindings are stored in the corresponding subfolders in the `addons/debug_draw_3d/native_api` folder and consists of standalone files that can be easily imported into your project to start using them.

For `C++`, include the header `addons/debug_draw_3d/native_api/cpp/dd3d_cpp_api.hpp` in your code and you will immediately have access to all `DebugDraw3D` classes. In the repository, you can find a test project utilizing C++ bindings.

For `C#`, all `DebugDraw3D` classes should be available to you immediately after installing the addon, since the `C# project` is located in the same folder as the `Godot project`.

> [!NOTE]
> Please note that methods that return objects may return `null` if `DebugDraw3D` has not been loaded, which may cause a crash.
