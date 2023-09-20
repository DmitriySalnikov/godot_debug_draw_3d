# Web Build

Setup repo following guide from [Debug drawing utility for Godot](../addons/debug_draw_3d/README.md)

Ensure you have the [Godot Export Templates for v4.1.1-stable](https://github.com/godotengine/godot/releases/download/4.1.1-stable/Godot_v4.1.1-stable_export_templates.tpz).
This requires having installed the `web_dlink_debug.zip` template.
Or you can compile one yourself with godot, see [Compiling for Web: GDExtension](https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_web.html#gdextension)

```PowerShell
# Copy addons to project
cp -r addons/ dd3d_web_build/addons/

# Copy examples to project
cp -r examples_dd3d/ dd3d_web_build/examples_dd3d/

# Build the project for web platform with godot
mkdir -p dd3d_web_build/build
cp dd3d_web_build/coi-serviceworker.min.js dd3d_web_build/build/coi-serviceworker.min.js
godot -e --headless --path dd3d_web_build --quit
godot --headless --path dd3d_web_build --export-release web

# Host site locally via python
python -m http.server --directory dd3d_web_build/build
```
