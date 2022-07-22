cd godot-cpp
set cpu=%NUMBER_OF_PROCESSORS%
::set api=custom_api_file="../api.json"
set api= 

::git apply --ignore-space-change --ignore-whitespace ../patches/godot_cpp_trim_unused_classes.patch
git apply --ignore-space-change --ignore-whitespace ../patches/godot_cpp_debug_string_data.patch

title win x64 debug
scons platform=windows target=debug bits=64 -j%cpu% %api% generate_bindings=yes
if errorlevel 1 ( echo Failed to generate and compile debug godot-cpp source code. Code: %errorlevel% && exit /b %errorlevel% )

title win x64
scons platform=windows target=release bits=64 -j%cpu% %api%
if errorlevel 1 ( echo Failed to compile Windows godot-cpp for x64. Code: %errorlevel% && exit /b %errorlevel% )

title android arm64v8 debug
scons platform=android target=debug android_arch=arm64v8 -j%cpu% %api%
if errorlevel 1 ( echo Failed to compile debug Android godot-cpp for arm64v8. Code: %errorlevel% && exit /b %errorlevel% )

title android arm64v8
scons platform=android target=release android_arch=arm64v8 -j%cpu% %api%
if errorlevel 1 ( echo Failed to compile Android godot-cpp for arm64v8. Code: %errorlevel% && exit /b %errorlevel% )
