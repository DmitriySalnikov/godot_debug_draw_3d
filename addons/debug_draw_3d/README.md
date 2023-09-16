![icon](/images/icon.png)

# Debug drawing utility for Godot

This is an add-on for debug drawing in 3D and for some 2D overlays, which is written in `C++` and can be used with `GDScript` or `C#`.

Based on my previous addon, which was developed only for C# https://github.com/DmitriySalnikov/godot_debug_draw_cs, and which was inspired by Zylann's GDScript addon https://github.com/Zylann/godot_debug_draw

## [Godot 3 version](https://github.com/DmitriySalnikov/godot_debug_draw_3d/tree/godot_3)

## Support me

Your support adds motivation to develop my public projects.

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/I2I53VZ2D)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://paypal.me/dmitriysalnikov)

[<img src="https://upload.wikimedia.org/wikipedia/commons/8/8f/QIWI_logo.svg" alt="qiwi" width=90px/>](https://qiwi.com/n/DMITRIYSALNIKOV)

## Features

3D:

* Arrow
* Billboard opaque square
* Box
* Camera Frustum
* Cylinder
* Gizmo
* Grid
* Line
* Line Path
* Line with Arrow
* Points
* Position 3D (3 crossing axes)
* Sphere

2D:

* **[Work in progress]**

Overlay:

* Text (with grouping and coloring)
* FPS Graph
* Custom Graphs

Precompiled for:

* Windows
* Linux
* macOS
* Android

## Download

To download, use the [Godot Asset Library](https://godotengine.org/asset-library/asset/1766) or download the archive by clicking the button at the top of the main repository page: `Code -> Download ZIP`, then unzip it to your project folder. Or use one of the stable versions from the [GitHub Releases](https://github.com/DmitriySalnikov/godot_debug_draw_3d/releases) page (just download one of the `Source Codes` in assets).

## Usage

* Close editor
* Copy `addons/debug_draw_3d` to your `addons` folder, create it if the folder doesn't exist
* Launch editor

### C\#

When you start the engine for the first time, bindings for `C#` will be generated automatically. If this does not happen, you can manually generate them through the `Project - Tools - Debug Draw` menu.

![project_tools_menu](/images/project_tools_menu.png)

## Examples

More examples can be found in the `examples_dd3d/` folder.

Simple test:

```gdscript
func _process(delta: float) -> void:
    var _time = Time.get_ticks_msec() / 1000.0
    var box_pos = Vector3(0, sin(_time * 4), 0)
    var line_begin = Vector3(-1, sin(_time * 4), 0)
    var line_end = Vector3(1, cos(_time * 4), 0)

    DebugDraw3D.draw_box(box_pos, Vector3(1, 2, 1), Color(0, 1, 0))
    DebugDraw3D.draw_line(line_begin, line_end, Color(1, 1, 0))
    DebugDraw2D.set_text("Time", _time)
    DebugDraw2D.set_text("Frames drawn", Engine.get_frames_drawn())
    DebugDraw2D.set_text("FPS", Engine.get_frames_per_second())
    DebugDraw2D.set_text("delta", delta)
```

```csharp
public override void _Process(float delta)
{
    var _time = Time.GetTicksMsec() / 1000.0f;
    var box_pos = new Vector3(0, Mathf.Sin(_time * 4f), 0);
    var line_begin = new Vector3(-1, Mathf.Sin(_time * 4f), 0);
    var line_end = new Vector3(1, Mathf.Cos(_time * 4f), 0);

    DebugDraw3D.DrawBox(box_pos, new Vector3(1, 2, 1), new Color(0, 1, 0));
    DebugDraw3D.DrawLine(line_begin, line_end, new Color(1, 1, 0));
    DebugDraw2D.SetText("Time", _time);
    DebugDraw2D.SetText("Frames drawn", Engine.GetFramesDrawn());
    DebugDraw2D.SetText("FPS", Engine.GetFramesPerSecond());
    DebugDraw2D.SetText("delta", delta);
}
```

![screenshot_1](/images/screenshot_1.png)

## API

A list of all functions is available in the documentation inside the editor.
![screenshot_4](/images/screenshot_4.png)

Besides `DebugDraw2D/3D`, you can also use `Dbg2/3`.

```gdscript
    DebugDraw3D.draw_box_xf(Transform3D(), Color.GREEN)
    Dbg3.draw_box_xf(Transform3D(), Color.GREEN)

    DebugDraw2D.set_text("delta", delta)
    Dbg2.set_text("delta", delta)
```

But unfortunately at the moment `GDExtension` does not support adding documentation.

## Exporting a project

Most likely, when exporting a release version of a game, you don't want to export the debug library along with it. But since there is still no `Conditional Compilation` in `GDScript`, so I decided to create a `dummy` library that has the same API as a regular library, but has minimal impact on performance, even if calls to its methods occur. The `dummy` library is used by default in the release version. However if you need to use debug rendering in the release version, then you can add the `forced_dd3d` feature when exporting. In this case, the release library with all the functionality will be used.

![export_features](/images/export_features.png)

In C#, these tags are not taken into account at compile time, so the Release build will use Runtime checks to disable draw calls. If you want to avoid this, you can manually specify the `FORCED_DD3D` symbol.

![csharp_compilation_symbols](/images/csharp_compilation_symbols.png)

## Known issues and limitations

Enabling occlusion culing can lower fps instead of increasing it. At the moment I do not know how to speed up the calculation of the visibility of objects.

The text in the keys and values of a text group cannot contain multi-line strings.

The entire text overlay can only be placed in one corner, unlike `DataGraphs`.

[Frustum of Camera3D does not take into account the window size from ProjectSettings](https://github.com/godotengine/godot/issues/70362).

**The version for Godot 4.0 requires explicitly specifying the exact data types, otherwise errors may occur.**

## More screenshots

`DebugDrawDemoScene.tscn` in editor
![screenshot_2](/images/screenshot_2.png)

`DebugDrawDemoScene.tscn` in play mode
![screenshot_3](/images/screenshot_3.png)

## Build

As well as for the engine itself, you will need to configure the [environment](https://docs.godotengine.org/en/4.1/contributing/development/compiling/index.html).
And also you need to apply several patches:

```bash
cd godot-cpp
git apply --ignore-space-change --ignore-whitespace ../patches/always_build_fix.patch
git apply --ignore-space-change --ignore-whitespace ../patches/1165.patch
# Optional
## Build only the necessary classes
git apply --ignore-space-change --ignore-whitespace ../patches/godot_cpp_exclude_unused_classes.patch
## Faster build
git apply --ignore-space-change --ignore-whitespace ../patches/unity_build.patch
```

Then you can just run scons as usual:

```bash
# build for the current system.
# target=editor is used for both the editor and the debug template.
scons target=editor dev_build=yes debug_symbols=yes
# build for the android. ANDROID_NDK_ROOT is required in your environment variables.
scons platform=android target=template_release arch=arm64v8
```
