cd godot-cpp
::set api=custom_api_file="../api.json"
set api= 

git apply --ignore-space-change --ignore-whitespace ../patches/godot_cpp_exclude_unused_classes.patch
git apply --ignore-space-change --ignore-whitespace ../patches/typed_ptr.patch

title win x64 debug dev
scons platform=windows target=template_debug arch=x86_64 dev_build=yes %api% generate_bindings=yes
if errorlevel 1 ( echo Failed to generate and compile debug godot-cpp source code. Code: %errorlevel% && exit /b %errorlevel% )

title win x64 debug
scons platform=windows target=template_debug arch=x86_64 %api%
if errorlevel 1 ( echo Failed to generate and compile debug godot-cpp source code. Code: %errorlevel% && exit /b %errorlevel% )

title win x64
scons platform=windows target=template_release arch=x86_64 debug_symbols=no %api%
if errorlevel 1 ( echo Failed to compile Windows godot-cpp for x64. Code: %errorlevel% && exit /b %errorlevel% )

title android arm64v8 debug
scons platform=android target=template_debug android_arch=arm64v8 %api%
if errorlevel 1 ( echo Failed to compile debug Android godot-cpp for arm64v8. Code: %errorlevel% && exit /b %errorlevel% )

title android arm64v8
scons platform=android target=template_release android_arch=arm64v8 %api%
if errorlevel 1 ( echo Failed to compile Android godot-cpp for arm64v8. Code: %errorlevel% && exit /b %errorlevel% )
