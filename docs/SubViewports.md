# SubViewports

If you use `DebugDraw3D` in `SubViewport`s, then you need to use DebugDraw3DScopeConfig.set_viewport to explicitly specify the viewport in which debug information will be drawn.

This will allow the add-on to find the correct camera that will be used for frustum culling.

```python
func _process(delta) -> void:
    # Create a scoped config with explicit viewport specification.
    var _s = DebugDraw3D.new_scoped_config().set_viewport(%MyViewport)
    # Also, if the script is attached to a Node inside this Viewport,
    # you can use the get_viewport() function of that Node.
    var _s2 = DebugDraw3D.new_scoped_config().set_viewport(get_viewport())
    DebugDraw3D.draw_box(Vector3.ZERO, Quaternion.IDENTITY, Vector3.ONE, Color.RED)
```

If you are using `SubViewport` to render the entire scene, you can use DebugDraw3D.scoped_config so that you don't have to specify it every time.

Just call this function once right after loading your viewport:

```python
func _enter_tree() -> void:
    DebugDraw3D.scoped_config().set_viewport(%MyViewport)
```

DebugDraw3DScopeConfig.set_viewport also takes into account the `own_world_3d` parameter of the `SubViewport`, so that debug information will be drawn in the selected world.

## Split screen

If you are using split screen or somehow viewing the same location through multiple Viewports, then you should disable frustum culling to avoid issues with disappearing objects.

To do this, use DebugDraw3DConfig.set_frustum_culling_mode:

```python
func _enter_tree() -> void:
    DebugDraw3D.config.frustum_culling_mode = DebugDraw3DConfig.FRUSTUM_DISABLED
```

This is how `DD3DDemoSubViewport.tscn` looks with `frustum_culling_mode` enabled (`FRUSTUM_PRECISE`) and disabled (`FRUSTUM_DISABLED`).

![](images/subviewport_demo.jpg)
