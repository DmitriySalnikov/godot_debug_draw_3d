@mainpage
# Introduction

This is an add-on for debug drawing in 3D and for some 2D overlays, which is written in `C++` and can be used with `GDScript` or `C#`.

## Screenshots

`DebugDrawDemoScene.tscn` in editor
![screenshot_2](/images/screenshot_2.png)

`DebugDrawDemoScene.tscn` in play mode
![screenshot_3](/images/screenshot_3.png)

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
* Web (WebAssembly)

This addon supports working with several World3D and different Viewports.
There is also a no depth test mode and other settings that can be changed for each instance.

This library supports double-precision builds, for more information, see [Double Precision](DoublePrecision.md).

## Download

To download, use the [Godot Asset Library](https://godotengine.org/asset-library/asset/1766) or use one of the stable versions from the [GitHub Releases](https://github.com/DmitriySalnikov/godot_debug_draw_3d/releases) page.

For versions prior to `1.4.5`, just download one of the `source codes` in the assets. For newer versions, download `debug-draw-3d_[version].zip`.

### Installation

* Close editor
* Copy `addons/debug_draw_3d` to your `addons` folder, create it if the folder doesn't exist
* Launch editor

## Support me

Your support adds motivation to develop my public projects.

[![](images/boosty.png){html: width=192px}](https://boosty.to/dmitriysalnikov/donate)

![USDT-TRC20 TEw934PrsffHsAn5M63SoHYRuZo984EF6v](images/USDT-TRC20.png){html: width=192px}

<h2><a href="https://dd3d.dmitriysalnikov.ru/demo/">Interactive Web Demo</a></h2>

[![](/images/screenshot_web.png)](https://dd3d.dmitriysalnikov.ru/demo/)

## Next steps

[Examples](Examples.md)

[Classes](annotated.html)
