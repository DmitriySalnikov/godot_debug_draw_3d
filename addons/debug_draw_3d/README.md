![icon](/images/icon_3d_128.png)

# Debug drawing utility for Godot

This is an add-on for debug drawing in 3D and for some 2D overlays, which is written in `C++` and can be used with `GDScript` or `C#`.

Based on my previous addon, which was developed [only for C#](https://github.com/DmitriySalnikov/godot_debug_draw_cs), and which was inspired by [Zylann's GDScript addon](https://github.com/Zylann/godot_debug_draw)

## [Documentation](https://dd3d.dmitriysalnikov.ru/docs/)

## [Godot 3 version](https://github.com/DmitriySalnikov/godot_debug_draw_3d/tree/godot_3)

## Support me

Your support adds motivation to develop my public projects.

<a href="https://boosty.to/dmitriysalnikov/donate"><img src="./docs/images/boosty.png" alt="Boosty" width=150px/></a>

<img src="./docs/images/USDT-TRC20.png" alt="USDT-TRC20" width=150px/>

<b>USDT-TRC20 TEw934PrsffHsAn5M63SoHYRuZo984EF6v</b>

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
* Plane
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
* Linux (built on Ubuntu 20.04)
* macOS (10.14+)
* Android (5.0+)
* iOS
* Web (Firefox not supported)

This addon supports working with several World3D and different Viewports.
There is also a no depth test mode and other settings that can be changed for each instance.

## [Interactive Web Demo](https://dd3d.dmitriysalnikov.ru/demo/)

[![screenshot_web](/images/screenshot_web.png)](https://dd3d.dmitriysalnikov.ru/demo/)

> [!WARNING]
>
> * Firefox most likely can't run this demo

## Download

To download, use the [Godot Asset Library](https://godotengine.org/asset-library/asset/1766) or download the archive by clicking the button at the top of the main repository page: `Code -> Download ZIP`, then unzip it to your project folder. Or use one of the stable versions from the [GitHub Releases](https://github.com/DmitriySalnikov/godot_debug_draw_3d/releases) page (just download one of the `Source Codes` in assets).

### Installation

* Close editor
* Copy `addons/debug_draw_3d` to your `addons` folder, create it if the folder doesn't exist
* Launch editor

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

![screenshot_1](/images/screenshot_1.png)

## API

This project has a separate [documentation](https://dd3d.dmitriysalnikov.ru/docs/) page.

Also, a list of all functions is available in the documentation inside the editor (see `DebugDraw3D` and `DebugDraw2D`).

![screenshot_4](/images/screenshot_4.png)

## Known issues and limitations

The text in the keys and values of a text group cannot contain multi-line strings.

The entire text overlay can only be placed in one corner, unlike `DataGraphs`.

[Frustum of Camera3D does not take into account the window size from ProjectSettings](https://github.com/godotengine/godot/issues/70362).

## More screenshots

`DebugDrawDemoScene.tscn` in editor
![screenshot_2](/images/screenshot_2.png)

`DebugDrawDemoScene.tscn` in play mode
![screenshot_3](/images/screenshot_3.png)
