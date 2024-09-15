scons apply_patches

cd godot-cpp

title win x64 debug dev
scons platform=windows target=editor arch=x86_64 dev_build=yes %api% generate_bindings=yes
if errorlevel 1 ( echo Failed to generate and compile debug godot-cpp source code. Code: %errorlevel% && exit /b %errorlevel% )

title win x64 debug
scons platform=windows target=editor arch=x86_64 debug_symbols=yes %api%
if errorlevel 1 ( echo Failed to generate and compile debug godot-cpp source code. Code: %errorlevel% && exit /b %errorlevel% )

title win x64
scons platform=windows target=template_release arch=x86_64 debug_symbols=no %api%
if errorlevel 1 ( echo Failed to compile Windows godot-cpp for x64. Code: %errorlevel% && exit /b %errorlevel% )
