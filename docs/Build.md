# Build

As well as for the engine itself, you will need to configure the [environment](https://docs.godotengine.org/en/4.1/contributing/development/compiling/index.html).
And also you may need to apply several patches:

```python
cd godot-cpp
# Optional
# #Build only the necessary classes
git apply --ignore-space-change --ignore-whitespace ../patches/godot_cpp_exclude_unused_classes.patch
# #Faster builds and larger binaries
git apply --ignore-space-change --ignore-whitespace ../patches/unity_build.patch
# #Hide useless function exports. Can greatly reduce the size of libraries
git apply --ignore-space-change --ignore-whitespace ../patches/visibilty_hidden.patch
```

Then you can just run scons as usual:

```python
# build for the current system.
# target=editor is used for both the editor and the debug template.
scons target=editor dev_build=yes debug_symbols=yes
# Android build. ANDROID_NDK_ROOT is required in your environment variables.
# Or ANDROID_HOME with 'ndk/23.2.8568313' installed.
scons platform=android target=template_release arch=arm64v8
# Web build. GDExtension for Web currently works best with Emscripten '3.1.39'
scons platform=web target=template_debug
```

## JavaScript/Web build

If you have problems running the Web version of your project, you can try using the scripts and tips from [this page](https://gist.github.com/DmitriySalnikov/ce12ff100df4e3352176768f5232abfa).

If you too want to add an [Interactive Demo](https://dd3d.dmitriysalnikov.ru/demo/) to your GitHub repository, then you can see how Nick Maltbie ([nicholas-maltbie](https://github.com/nicholas-maltbie)) added this feature to this repository in PR [#24](https://github.com/DmitriySalnikov/godot_debug_draw_3d/pull/24)!

In short, you need to activate `Extension Support` when exporting and add the [gzuidhof/coi-serviceworker](https://github.com/gzuidhof/coi-serviceworker) to the \<head\> and to the export folder. Then you will need to somehow publish a demo on the GitHub pages, as for example done in [#24](https://github.com/DmitriySalnikov/godot_debug_draw_3d/pull/24/files#diff-46a620e221376649fe75b0aaf2f607fee47f0d47db1d37bc08bb4a5f11b1af98).

![](images/export_web_for_github.png)
