![icon](/images/icon.png)

# Debug drawing utility for Godot

This is an add-on for debug drawing in 3D and for some 2D overlays, which is written in `C++` and can be used with `GDScript`<!-- or `C#`-->.

Based on my previous addon, which was developed only for C# https://github.com/DmitriySalnikov/godot_debug_draw_cs, and which was inspired by Zylann's GDScript addon https://github.com/Zylann/godot_debug_draw

## [Godot 3 version](https://github.com/DmitriySalnikov/godot_debug_draw_3d/tree/godot_3)

## Support me

Your support adds motivation to develop my public projects.

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/I2I53VZ2D)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://paypal.me/dmitriysalnikov)

[<img src="https://upload.wikimedia.org/wikipedia/commons/8/8f/QIWI_logo.svg" alt="qiwi" width=90px/>](https://qiwi.com/n/DMITRIYSALNIKOV)

## Features

3D Primitives:

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

To download, use the [Godot Asset Library](https://godotengine.org/asset-library/asset/1766) or download the archive by clicking the button at the top of the main repository page: `Code -> Download ZIP`, then unzip it to your project folder. Or use one of the stable versions from the [GitHub Releases](https://github.com/DmitriySalnikov/godot_debug_draw_3d/releases) page (just download one of the "Source Codes" in assets).

## Usage

1. Copy `addons/debug_draw_3d` to your `addons` folder, create it if the folder doesn't exist
1. Restart the editor
<!--
1. Rebuild the project if you use C#
1. Add `addons/debug_draw_3d/debug_draw.gd` or/and `addons/debug_draw_3d/DebugDrawCS.cs` to your project as autoload singleton
2. (Optionally) Enable the `Debug Draw 3D for Editor` plugin to enable debug drawing support inside the editor
-->

## Examples

More examples can be found in the `examples_dd3d/` folder.

Simple test:

```gdscript
func _process(delta: float) -> void:
    var _time = Time.get_ticks_msec() / 1000.0
    var box_pos = Vector3(0, sin(_time * 4), 0)
    var line_begin = Vector3(-1, sin(_time * 4), 0)
    var line_end = Vector3(1, cos(_time * 4), 0)

    DebugDraw.draw_box(box_pos, Vector3(1, 2, 1), Color(0, 1, 0))
    DebugDraw.draw_line(line_begin, line_end, Color(1, 1, 0))
    DebugDraw.set_text("Time", _time)
    DebugDraw.set_text("Frames drawn", Engine.get_frames_drawn())
    DebugDraw.set_text("FPS", Engine.get_frames_per_second())
    DebugDraw.set_text("delta", delta)
```

<!-- ```csharp
public override void _Process(float delta)
{
    var _time = OS.GetTicksMsec() / 1000f;
    var box_pos = new Vector3(0, Mathf.Sin(_time * 4), 0);
    var line_begin = new Vector3(-1, Mathf.Sin(_time * 4), 0);
    var line_end = new Vector3(1, Mathf.Cos(_time * 4), 0);

    DebugDrawCS.DrawBox(box_pos, new Vector3(1, 2, 1), new Color(0, 1, 0));
    DebugDrawCS.DrawLine(line_begin, line_end, new Color(1, 1, 0));
    DebugDrawCS.SetText("Time", _time);
    DebugDrawCS.SetText("Frames drawn", Engine.GetFramesDrawn());
    DebugDrawCS.SetText("FPS", Engine.GetFramesPerSecond());
    DebugDrawCS.SetText("delta", delta);
}
``` -->

![screenshot_1](/images/screenshot_1.png)

## API

A list of all functions is available in the documentation inside the editor.
![screenshot_4](/images/screenshot_4.png)

Besides `DebugDraw`, you can also use `Dbg3`.

```gdscript
    DebugDraw.draw_box_xf(Transform3D(), Color.GREEN)
    Dbg3.draw_box_xf(Transform3D(), Color.GREEN)
```

But unfortunately at the moment `GDExtension` does not support adding documentation.

## Exporting a project

Most likely, when exporting a release version of a game, you don't want to export the debug library along with it. But since there is still no `Conditional Compilation` in `GDScript`, so I decided to create a `dummy` library that has the same API as a regular library, but has minimal impact on performance, even if calls to its methods occur. The `dummy` library is used by default in the release version. However if you need to use debug rendering in the release version, then you can add the `forced_dd3d` feature when exporting. In this case, the release library with all the functionality will be used.

![export_features](/images/export_features.png)

<!-- ### For GDScript

I made a dummy wrapper to remove unnecessary checks and calls in GDScript after exporting the release version of the game. It contains only definitions of functions and parameters, but does not execute any code.

To use it, you need to replace the original autoload with the dummy version (`res://addons/debug_draw_3d/debug_draw_dummy.gd`) before exporting.

*note:* I previously suggested overriding autoloads via project settings, but this approach did not work correctly and was removed in godot 3.5. -->

<!-- ### For C\#

Just switch to the `release` build and all calls to this library will be removed. -->

<!-- ### For Native Libraries

In order to not export native libraries in the release build, you need to specify an exclusion filter.

To do this, select the profile in the `Export` menu, go to the `Resources` tab and add this line `addons/debug_draw_3d/libs/*` to the `Filters to exclude...`. If necessary, separate the previous values with a comma.

![export filter](/images/export_filter.png)

There are also additional parameters in the project settings to disable debug rendering in certain conditions.

![proj settings](/images/additional_proj_settings.png) -->

<!-- ### Remark

It will not be possible to completely get rid of this library in the release build in this way. Since empty functions can still be called, which can slow down code execution very slightly. To avoid this, you need to get rid of the calls of these functions in your code.

In `GDScript`, for example, you can use `if`'s before calling debugging functions so that they are not called in the release build. And in `C#`, conditional compilation (`#if DEBUG`) can be used so that calls to debugging functions occur only in the debug assembly. -->

## Known issues and limitations

Enabling occlusion culing can lower fps instead of increasing it. At the moment I do not know how to speed up the calculation of the visibility of objects.

The text in the keys and values of a text group cannot contain multi-line strings.

The entire text overlay can only be placed in one corner, unlike DataGraphs.

[Frustum of Camera3D does not take into account the window size from ProjectSettings](https://github.com/godotengine/godot/issues/70362).

**The version for Godot 4.0 requires explicitly specifying the exact data types, otherwise errors may occur.**

**The C# binding is not ready yet.**

## More screenshots

`DebugDrawDemoScene.tscn` in editor
![screenshot_2](/images/screenshot_2.png)

`DebugDrawDemoScene.tscn` in play mode
![screenshot_3](/images/screenshot_3.png)
