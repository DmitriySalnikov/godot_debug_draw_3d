
using Godot;
using System;
using System.Collections.Generic;
using System.Threading;

[Tool]
public partial class DD3DDemoCS : Node3D
{
    [Export] Font custom_font;
    [Export] Font custom_3d_font;
    [Export] bool zylann_example = false;
    [Export] bool update_in_physics = false;
    [Export] bool test_text = true;
    [Export] bool more_test_cases = true;
    [Export] bool draw_3d_text = true;
    [Export] bool draw_array_of_boxes = false;
    [Export] bool draw_text_with_boxes = false;
    [Export] bool draw_1m_boxes = false;
    [Export(PropertyHint.Range, "0, 5, 0.001")] float debug_thickness = 0.1f;
    [Export(PropertyHint.Range, "0, 1, 0.001")] float debug_center_brightness = 0.8f;
    [Export(PropertyHint.Range, "0, 1")] float camera_frustum_scale = 0.9f;

    [ExportGroup("Text groups", "text_groups")]
    [Export] bool text_groups_show_examples = true;
    [Export] bool text_groups_show_hints = true;
    [Export] bool text_groups_show_stats = true;
    [Export] bool text_groups_show_stats_2d = true;
    [Export] DebugDraw2DConfig.BlockPosition text_groups_position = DebugDraw2DConfig.BlockPosition.LeftTop;
    [Export] Vector2I text_groups_offset = new Vector2I(8, 8);
    [Export] Vector2I text_groups_padding = new Vector2I(3, 1);
    [Export(PropertyHint.Range, "1, 100")] int text_groups_default_font_size = 15;
    [Export(PropertyHint.Range, "1, 100")] int text_groups_title_font_size = 20;
    [Export(PropertyHint.Range, "1, 100")] int text_groups_text_font_size = 17;

    [ExportGroup("Tests", "tests")]
    [Export]
    bool tests_use_threads = false;
    Thread[] test_thread = null;
    bool test_thread_closing = false;


    Dictionary<Key, int> button_presses = new Dictionary<Key, int>() {
        { Key.Left, 0 },
        { Key.Up, 0 },
        { Key.Ctrl, 0 },
        { Key.F1, 0 },
        { Key.Key1, 0 },
        { Key.Key2, 0 },
        { Key.Key3, 0 },
    };

    double timer_1 = 0.0;
    double timer_cubes = 0.0;
    double timer_3 = 0.0;
    double timer_text = 0.0;

    public DD3DDemoCS()
    {
        // https://github.com/godotengine/godot/issues/78513
        // register cleanup code to prevent unloading issues
        System.Runtime.Loader.AssemblyLoadContext.GetLoadContext(System.Reflection.Assembly.GetExecutingAssembly()).Unloading += alc =>
        {
            _thread_stop();
        };
    }

    public override async void _Ready()
    {
        _get_nodes();
        _update_keys_just_press();

        await new SignalAwaiter(GetTree(), "process_frame", this);

        // this check is required for inherited scenes, because an instance of this 
        // script is created first, and then overridden by another
        if (!IsInsideTree())
            return;

        // DD3D GDExtension not loaded
        if (DebugDraw3D.Config == null)
            return;
        DebugDraw2D.Config.TextBackgroundColor = new Color(0.3f, 0.3f, 0.3f, 0.8f);
    }

    bool _is_key_just_pressed(Key key)
    {
        if (button_presses[key] == 1)
        {
            button_presses[key] = 2;
            return true;
        }
        return false;
    }

    void _update_timers(double delta)
    {
        timer_1 -= delta;
        timer_cubes -= delta;
        timer_3 -= delta;
        timer_text -= delta;
    }

    void _update_keys_just_press()
    {
        var set = (Key k) => Input.IsKeyPressed(k) ? (button_presses[k] == 0 ? 1 : button_presses[k]) : 0;
        button_presses[Key.Left] = set(Key.Left);
        button_presses[Key.Up] = set(Key.Up);
        button_presses[Key.Ctrl] = set(Key.Ctrl);
        button_presses[Key.F1] = set(Key.F1);
        button_presses[Key.Key1] = set(Key.Key1);
        button_presses[Key.Key2] = set(Key.Key2);
        button_presses[Key.Key3] = set(Key.Key3);
    }

    bool phys_frame_called = false;
    public override void _Process(double delta)
    {
        ((ShaderMaterial)((PrimitiveMesh)dOtherWorld.Mesh).Material).SetShaderParameter("albedo_texture", dOtherWorldViewport.GetTexture());

        phys_frame_called = false;
        if (!update_in_physics)
        {
            MainUpdate(delta);
            _update_timers(delta);
        }

        _call_from_thread();
    }

    public override void _PhysicsProcess(double delta)
    {
        if (!phys_frame_called)
        {
            phys_frame_called = true;
            if (update_in_physics)
            {
                MainUpdate(delta);
                _update_timers(delta);
            }
        }

        // Physics specific:
        if (!zylann_example)
        {
            // DD3D GDExtension not loaded
            if (DebugDraw3D.ScopedConfig() == null)
                return;

            DebugDraw3D.DrawLine(dLines_8.GlobalPosition, dLines_Target.GlobalPosition, Colors.Yellow);
            if (more_test_cases)
            {
                _draw_rays_casts();
            }

            // Additional drawing in the Viewport
            using (var _w1 = DebugDraw3D.NewScopedConfig().SetViewport(dOtherWorldBox.GetViewport()).SetThickness(0.01f).SetCenterBrightness(1).SetNoDepthTest(true))
            {
                DebugDraw3D.DrawBoxXf(new Transform3D(Basis.Identity
                    .Scaled(Vector3.One * 0.3f)
                    .Rotated(new Vector3(0, 0, 1), Mathf.Pi / 4)
                    .Rotated(new Vector3(0, 1, 0), Mathf.Wrap(Time.GetTicksMsec() / -1500.0f, 0, Mathf.Tau) - Mathf.Pi / 4), dOtherWorldBox.GlobalPosition),
                    Colors.Brown, true, 0.4f);
            }
        }
    }

    void MainUpdate(double delta)
    {
        // DD3D GDExtension not loaded
        if (DebugDraw3D.ScopedConfig() == null)
            return;

        DebugDraw3D.ScopedConfig().SetThickness(debug_thickness).SetCenterBrightness(debug_center_brightness);

        _update_keys_just_press();

        if (_is_key_just_pressed(Key.F1))
            zylann_example = !zylann_example;

        // Zylann's example :D
        if (zylann_example)
        {
            var _time = Time.GetTicksMsec() / 1000.0f;
            var box_pos = new Vector3(0, Mathf.Sin(_time * 4f), 0);
            var line_begin = new Vector3(-1, Mathf.Sin(_time * 4f), 0);
            var line_end = new Vector3(1, Mathf.Cos(_time * 4f), 0);
            DebugDraw3D.DrawBox(box_pos, Quaternion.Identity, new Vector3(1, 2, 1), new Color(0, 1, 0));
            DebugDraw3D.DrawLine(line_begin, line_end, new Color(1, 1, 0));
            DebugDraw2D.SetText("Time", _time.ToString());
            DebugDraw2D.SetText("Frames drawn", Engine.GetFramesDrawn().ToString());
            DebugDraw2D.SetText("FPS", Engine.GetFramesPerSecond().ToString());
            DebugDraw2D.SetText("delta", delta.ToString());

            dHitTest.Visible = false;
            dLagTest.Visible = false;
            dPlaneOrigin.Visible = false;
            pZDepthTestCube.Visible = false;
            dOtherWorld.Visible = false;
            return;
        }

        dHitTest.Visible = true;
        dLagTest.Visible = true;
        dPlaneOrigin.Visible = true;
        pZDepthTestCube.Visible = true;
        dOtherWorld.Visible = true;

        // Testing the rendering layers by showing the image from the second camera inside the 2D panel
        DebugDraw3D.Config.GeometryRenderLayers = !Input.IsKeyPressed(Key.Alt) ? 1 : 0b10010;
        dPanel.Visible = Input.IsKeyPressed(Key.Alt);
        DebugDraw2D.CustomCanvas = Input.IsKeyPressed(Key.Alt) ? dCustomCanvas : null;

        // More property toggles
        DebugDraw3D.Config.Freeze3dRender = Input.IsKeyPressed(Key.Down);
        DebugDraw3D.Config.VisibleInstanceBounds = Input.IsKeyPressed(Key.Right);

        // Regenerate meshes
        if (Input.IsActionJustPressed("ui_end"))
            DebugDraw3D.RegenerateGeometryMeshes();

        // Some property toggles
        if (_is_key_just_pressed(Key.Left))
            DebugDraw3D.Config.FrustumCullingMode = (DebugDraw3DConfig.CullingMode)Mathf.Wrap((int)DebugDraw3D.Config.FrustumCullingMode + 1, 0, 3);

        if (_is_key_just_pressed(Key.Up))
            DebugDraw3D.Config.ForceUseCameraFromScene = !DebugDraw3D.Config.ForceUseCameraFromScene;

        if (_is_key_just_pressed(Key.Ctrl))
            if (!Engine.IsEditorHint())
                GetViewport().Msaa3D = GetViewport().Msaa3D == Viewport.Msaa.Msaa4X ? Viewport.Msaa.Disabled : Viewport.Msaa.Msaa4X;

        if (!Engine.IsEditorHint())
        {
            if (_is_key_just_pressed(Key.Key1))
                DebugDraw3D.DebugEnabled = !DebugDraw3D.DebugEnabled;
            if (_is_key_just_pressed(Key.Key2))
                DebugDraw2D.DebugEnabled = !DebugDraw2D.DebugEnabled;
            if (_is_key_just_pressed(Key.Key3))
                DebugDrawManager.DebugEnabled = !DebugDrawManager.DebugEnabled;
        }


        DebugDraw3D.Config.FrustumLengthScale = camera_frustum_scale;

        // Zones with black borders
        foreach (var node in dZones.GetChildren())
        {
            if (node is Node3D z)
            {
                DebugDraw3D.DrawBoxXf(z.GlobalTransform, Colors.Black);
            }
        }

        // Spheres
        _draw_zone_title(pSpheresBox, "Spheres");

        DebugDraw3D.DrawSphereXf(dSphereTransform.GlobalTransform, Colors.Crimson);
        using (var _s1 = DebugDraw3D.NewScopedConfig().SetHdSphere(true))
            DebugDraw3D.DrawSphereXf(dSphereHDTransform.GlobalTransform, Colors.OrangeRed);

        /// Delayed spheres
        if (timer_1 <= 0)
        {
            var s_xf = dSpherePosition.GlobalTransform;
            DebugDraw3D.DrawSphere(s_xf.Origin, 2.0f, Colors.BlueViolet, 2.0f);
            using (var _s1 = DebugDraw3D.NewScopedConfig().SetHdSphere(true))
                DebugDraw3D.DrawSphere(s_xf.Origin - s_xf.Basis.Z * 4.2f, 2.0f, Colors.CornflowerBlue, 2.0f);
            timer_1 = 2;
        }

        // Capsules
        {
            _draw_zone_title(pCapsulesBox, "Capsules");

            var capsule1_xf = dCapsule1.GlobalTransform;
            var capsule1_scale = capsule1_xf.Basis.Scale;
            DebugDraw3D.DrawCapsule(capsule1_xf.Origin, capsule1_xf.Basis.GetRotationQuaternion(), capsule1_scale.X, capsule1_scale.Y, Colors.Crimson);
            DebugDraw3D.DrawCapsuleAb(dCapsule2a.GlobalPosition, dCapsule2b.GlobalPosition, 0.6f);
        }

        // Cylinders
        {
            _draw_zone_title(pCylindersBox, "Cylinders");

            DebugDraw3D.DrawCylinder(dCylinder1.GlobalTransform, Colors.Crimson);
            DebugDraw3D.DrawCylinder(new Transform3D(Basis.Identity.Scaled(new Vector3(1, 2, 1)), dCylinder2.GlobalPosition), Colors.Red);
            DebugDraw3D.DrawCylinderAb(dCylinder3a.GlobalPosition, dCylinder3b.GlobalPosition, 0.7f);
        }

        // Boxes
        _draw_zone_title(pBoxesBox, "Boxes");

        DebugDraw3D.DrawBoxXf(dBox1.GlobalTransform, Colors.MediumPurple);
        DebugDraw3D.DrawBox(dBox2.GlobalPosition, Quaternion.FromEuler(new Vector3(0, Mathf.DegToRad(45), Mathf.DegToRad(45))), Vector3.One, Colors.RebeccaPurple);
        DebugDraw3D.DrawBoxXf(new Transform3D(new Basis(Vector3.Up, Mathf.Pi * 0.25f).Scaled(Vector3.One * 2), dBox3.GlobalPosition), Colors.RosyBrown);

        DebugDraw3D.DrawAabb(new Aabb(dAABB_fixed.GlobalPosition, new Vector3(2, 1, 2)), Colors.Aqua);
        DebugDraw3D.DrawAabbAb(dAABB.GetChild<Node3D>(0).GlobalPosition, dAABB.GetChild<Node3D>(1).GlobalPosition, Colors.DeepPink);

        // Boxes AB

        DebugDraw3D.DrawArrow(dBoxAB.GlobalPosition, dBoxABup.GlobalPosition, Colors.Gold, 0.1f, true);
        DebugDraw3D.DrawBoxAb(dBoxABa.GlobalPosition, dBoxABb.GlobalPosition, dBoxABup.GlobalPosition - dBoxAB.GlobalPosition, Colors.Peru);

        DebugDraw3D.DrawArrow(dBoxABEdge.GlobalPosition, dBoxABEdgeup.GlobalPosition, Colors.DarkRed, 0.1f, true);
        DebugDraw3D.DrawBoxAb(dBoxABEdgea.GlobalPosition, dBoxABEdgeb.GlobalPosition, dBoxABEdgeup.GlobalPosition - dBoxABEdge.GlobalPosition, Colors.DarkOliveGreen, false);

        // Lines
        _draw_zone_title(pLinesBox, "Lines");

        DebugDraw3D.DrawSquare(dLines_Target.GlobalPosition, 0.5f, Colors.Red);

        DebugDraw3D.DrawLine(dLines_1.GlobalPosition, dLines_Target.GlobalPosition, Colors.Fuchsia);
        DebugDraw3D.DrawRay(dLines_3.GlobalPosition, (dLines_Target.GlobalPosition - dLines_3.GlobalPosition).Normalized(), 3.0f, Colors.Crimson);


        if (timer_3 <= 0)
        {
            DebugDraw3D.DrawLine(dLines_6.GlobalPosition, dLines_Target.GlobalPosition, Colors.Fuchsia, 2.0f);
            timer_3 = 2;
        }

        // Test UP vector
        DebugDraw3D.DrawLine(dLines_7.GlobalPosition, dLines_Target.GlobalPosition, Colors.Red);

        // Lines with Arrow
        DebugDraw3D.DrawArrow(dLines_2.GlobalPosition, dLines_Target.GlobalPosition, Colors.Blue, 0.5f, true);
        DebugDraw3D.DrawArrowRay(dLines_4.GlobalPosition, (dLines_Target.GlobalPosition - dLines_4.GlobalPosition).Normalized(), 8.0f, Colors.Lavender, 0.5f, true);

        DebugDraw3D.DrawLineHitOffset(dLines_5.GlobalPosition, dLines_Target.GlobalPosition, true, Mathf.Abs(Mathf.Sin(Time.GetTicksMsec() / 1000.0f)), 0.25f, Colors.Aqua);

        // Paths
        _draw_zone_title(pPathsBox, "Paths");

        _draw_paths(Vector3.Zero);
        _draw_paths(new Vector3(-2, 0, -1), 0);
        _draw_paths(new Vector3(2, 0, -1), 1);

        // Misc
        _draw_zone_title(pMiscBox, "Misc");

        if (Engine.IsEditorHint())
        {
            using var s = DebugDraw3D.NewScopedConfig().SetThickness(0);
            DebugDraw3D.DrawCameraFrustum(dCamera, Colors.DarkOrange);
        }

        using (var s = DebugDraw3D.NewScopedConfig().SetCenterBrightness(0.1f))
        {
            DebugDraw3D.DrawArrowhead(dMisc_Arrow.GlobalTransform, Colors.YellowGreen);
        }

        DebugDraw3D.DrawSquare(dMisc_Billboard.GlobalPosition, 0.5f, Colors.Green);

        DebugDraw3D.DrawPosition(dMisc_Position.GlobalTransform, Colors.Brown);

        DebugDraw3D.DrawGizmo(dMisc_GizmoTransform.GlobalTransform, null, true);
        DebugDraw3D.DrawGizmo(dMisc_GizmoOneColor.GlobalTransform, Colors.Brown, true);
        using (var s = DebugDraw3D.NewScopedConfig().SetCenterBrightness(0.5f).SetNoDepthTest(true))
        {
            DebugDraw3D.DrawGizmo(dMisc_GizmoNormal.GlobalTransform.Orthonormalized(), null, false);
        }

        // Grids
        _draw_zone_title_pos(dGrids_GridCentered.GlobalPosition + new Vector3(0, 1.5f, 0), "Grids", 96, 36);

        Transform3D tg = dGrids_Grid.GlobalTransform;
        Vector3 tn = dGrids_Grid_Subdivision.Transform.Origin;
        DebugDraw3D.DrawGrid(tg.Origin, tg.Basis.X, tg.Basis.Z, new Vector2I((int)tn.X * 10, (int)tn.Z * 10), Colors.LightCoral, false);

        var tn1 = dGrids_GridCentered_Subdivision.Transform.Origin;
        DebugDraw3D.DrawGridXf(dGrids_GridCentered.GlobalTransform, new Vector2I((int)(tn1.X * 10), (int)(tn1.Z * 10)));

        using (var s = DebugDraw3D.NewScopedConfig().SetThickness(0.05f))
        {
            DebugDraw3D.DrawBoxXf(dPostProcess.GlobalTransform, Colors.SeaGreen);
        }

        // Local transform
        _draw_local_xf_box(pLocalTransformRecursiveOrigin.GlobalTransform, 0.05f, 10);

        // 2D
        DebugDraw2D.Config.TextDefaultSize = text_groups_default_font_size;
        DebugDraw2D.Config.TextBlockOffset = text_groups_offset;
        DebugDraw2D.Config.TextBlockPosition = text_groups_position;
        DebugDraw2D.Config.TextPadding = text_groups_padding;

        DebugDraw2D.Config.TextCustomFont = custom_font;


        if (test_text)
        {
            _text_tests();
        }

        // Lag Test
        var lag_test_pos = ToGlobal((Vector3)dLagTest_RESET.GetAnimation("RESET").TrackGetKeyValue(0, 0));
        _draw_zone_title_pos(lag_test_pos, "Lag test");

        dLagTest.Position = lag_test_pos + new Vector3(Mathf.Sin(Time.GetTicksMsec() / 100.0f) * 2.5f, 0, 0);
        DebugDraw3D.DrawBox(dLagTest.GlobalPosition, Quaternion.Identity, Vector3.One * 2.01f, Colors.Chocolate, true);

        if (more_test_cases)
        {
            foreach (var node in dHitTest_RayEmitter.GetChildren())
            {
                if (node is RayCast3D ray)
                    ray.SetPhysicsProcessInternal(true);
            }

            _more_tests();
        }
        else
        {
            foreach (var node in dHitTest_RayEmitter.GetChildren())
            {
                if (node is RayCast3D ray)
                    ray.SetPhysicsProcessInternal(false);
            }
        }

        _draw_other_world();

        if (draw_array_of_boxes)
        {
            _draw_array_of_boxes();
        }

    }

    void _text_tests()
    {
        DebugDraw2D.SetText("FPS", $"{Engine.GetFramesPerSecond():F2}", 0, Colors.Gold);

        if (text_groups_show_examples)
        {
            if (timer_text < 0)
            {
                DebugDraw2D.SetText("Some delayed text", "for 2.5s", -1, Colors.Black, 2.5f); // it's supposed to show text for 2.5 seconds
                timer_text += 5;
            }

            DebugDraw2D.BeginTextGroup("-- First Group --", 2, Colors.LimeGreen, true, text_groups_title_font_size, text_groups_text_font_size);
            DebugDraw2D.SetText("Simple text");
            DebugDraw2D.SetText("Text", "Value", 0, Colors.Aquamarine);
            DebugDraw2D.SetText("Text out of order", null, -1, Colors.Silver);
            DebugDraw2D.BeginTextGroup("-- Second Group --", 1, Colors.Beige);
            DebugDraw2D.SetText("Rendered frames", Engine.GetFramesDrawn().ToString());
            DebugDraw2D.EndTextGroup();
        }

        if (text_groups_show_stats)
        {
            DebugDraw2D.BeginTextGroup("-- Stats --", 3, Colors.Wheat);
            var render_stats = DebugDraw3D.GetRenderStats();

            if (render_stats != null && text_groups_show_stats)
            {
                DebugDraw2D.SetText("Total", render_stats.TotalGeometry.ToString());
                DebugDraw2D.SetText("Instances", render_stats.Instances.ToString(), 1);
                DebugDraw2D.SetText("Lines", render_stats.Lines.ToString(), 2);
                DebugDraw2D.SetText("Total Visible", render_stats.TotalVisible.ToString(), 3);
                DebugDraw2D.SetText("Visible Instances", render_stats.VisibleInstances.ToString(), 4);
                DebugDraw2D.SetText("Visible Lines", render_stats.VisibleLines.ToString(), 5);

                DebugDraw2D.SetText("---", "", 12);

                DebugDraw2D.SetText("Culling time", $"{(render_stats.TotalTimeCullingUsec / 1000.0):F2} ms", 13);
                DebugDraw2D.SetText("Filling instances buffer", $"{(render_stats.TimeFillingBuffersInstancesUsec / 1000.0):F2} ms", 14);
                DebugDraw2D.SetText("Filling lines buffer", $"{(render_stats.TimeFillingBuffersLinesUsec / 1000.0):F2} ms", 15);
                DebugDraw2D.SetText("Filling time", $"{(render_stats.TotalTimeFillingBuffersUsec / 1000.0):F2} ms", 16);
                DebugDraw2D.SetText("Total time", $"{(render_stats.TotalTimeSpentUsec / 1000.0):F2} ms", 17);

                DebugDraw2D.SetText("----", null, 32);

                DebugDraw2D.SetText("Total Label3D", render_stats.NodesLabel3dExistsTotal.ToString(), 33);
                DebugDraw2D.SetText("Visible Label3D", (render_stats.NodesLabel3dVisible + render_stats.NodesLabel3dVisiblePhysics).ToString(), 34);

                DebugDraw2D.SetText("-----", null, 48);

                DebugDraw2D.SetText("Created scoped configs", $"{render_stats.CreatedScopedConfigs}", 49);
            }

            if (text_groups_show_stats && text_groups_show_stats_2d)
            {
                DebugDraw2D.SetText("------", null, 64);
            }

            var render_stats_2d = DebugDraw2D.GetRenderStats();
            if (render_stats_2d != null && text_groups_show_stats_2d)
            {
                DebugDraw2D.SetText("Text groups", render_stats_2d.OverlayTextGroups.ToString(), 96);
                DebugDraw2D.SetText("Text lines", render_stats_2d.OverlayTextLines.ToString(), 97);
            }
            DebugDraw2D.EndTextGroup();
        }

        if (text_groups_show_hints)
        {
            DebugDraw2D.BeginTextGroup("controls", 1024, Colors.White, false);
            if (!Engine.IsEditorHint())
            {
                DebugDraw2D.SetText("WASD QE, LMB", "To move", 0);
            }
            DebugDraw2D.SetText("Alt: change render layers", DebugDraw3D.Config.GeometryRenderLayers.ToString(), 1);
            if (!OS.HasFeature("web"))
            {
                DebugDraw2D.SetText("Ctrl: toggle anti-aliasing", GetViewport().Msaa3D == Viewport.Msaa.Msaa4X ? "MSAA 4x" : "Disabled", 2);
            }
            DebugDraw2D.SetText("Down: freeze render", DebugDraw3D.Config.Freeze3dRender.ToString(), 3);
            if (Engine.IsEditorHint())
            {
                DebugDraw2D.SetText("Up: use scene camera", DebugDraw3D.Config.ForceUseCameraFromScene.ToString(), 4);
            }
            DebugDraw2D.SetText("1,2,3: toggle debug", $"{DebugDraw3D.DebugEnabled}, {DebugDraw2D.DebugEnabled} 😐, {DebugDrawManager.DebugEnabled} 😏", 5);
            DebugDraw2D.SetText("Left: frustum culling mode", frustum_culling_mode_names[(int)DebugDraw3D.Config.FrustumCullingMode], 6);
            DebugDraw2D.SetText("Right: draw bounds for culling", DebugDraw3D.Config.VisibleInstanceBounds.ToString(), 7);
        }
    }

    readonly string[] frustum_culling_mode_names = ["Disabled", "Rough", "Precise"];

    void _draw_zone_title(Node3D node, string title)
    {
        if (draw_3d_text)
        {
            using var _s1 = DebugDraw3D.NewScopedConfig().SetTextOutlineSize(72);
            DebugDraw3D.DrawText(node.GlobalPosition + node.GlobalBasis.Y * 0.85f, title, 128);
        }
    }

    void _draw_zone_title_pos(Vector3 pos, string title, int font_size = 128, int outline = 72)
    {
        if (draw_3d_text)
        {
            using var _s1 = DebugDraw3D.NewScopedConfig().SetTextOutlineSize(outline);
            DebugDraw3D.DrawText(pos, title, font_size);
        }
    }

    const float _local_mul = 0.45f;
    static readonly Vector3 _local_mul_vec = new(_local_mul, _local_mul, _local_mul);
    Vector3[] __local_lines_cross_recursive = [new Vector3(-0.5f, -0.5f, -0.5f), new Vector3(0.5f, -0.5f, 0.5f), new Vector3(-0.5f, -0.5f, 0.5f), new Vector3(0.5f, -0.5f, -0.5f)];
    Transform3D __local_box_recursive = Transform3D.Identity.RotatedLocal(Vector3.Up, Mathf.DegToRad(30)).Translated(new Vector3(-0.25f, -0.55f, 0.25f)).Scaled(_local_mul_vec);
    Transform3D __local_sphere_recursive = Transform3D.Identity.Translated(new Vector3(0.5f, 0.55f, -0.5f)).Scaled(_local_mul_vec);

    void _draw_local_xf_box(Transform3D xf, float thickness, int max_depth, int depth = 0)
    {
        if (depth >= max_depth)
            return;

        using var _s1 = DebugDraw3D.NewScopedConfig().SetThickness(thickness).SetTransform(xf);

        // a box with a small offset
        DebugDraw3D.DrawBoxXf(new Transform3D(Basis.Identity, new Vector3(0, 0.001f, 0)), Colors.Brown);
        // a box and a stand for the next depth
        DebugDraw3D.DrawBoxXf(__local_box_recursive, Colors.Chartreuse);
        // just a sphere and lines
        DebugDraw3D.DrawSphereXf(__local_sphere_recursive, Colors.DarkOrange);

        _s1.SetThickness(0);

        DebugDraw3D.DrawLines(__local_lines_cross_recursive, Colors.Crimson);

        // A simple animation generator with descent into the depth of the scene
#if false
        {
            Animation anim = pRecursiveTransformTest.GetAnimation("recursive");
            // clear keys
            if (depth == 0)
                for (var i = 0; i < anim.TrackGetKeyCount(0); i++)
                {
                    anim.TrackRemoveKey(0, 0);
                    anim.TrackRemoveKey(1, 0);
                }

            var time = depth * 2;
            var s_xf = xf * __local_sphere_recursive;
            var next_s_xf = (xf * __local_box_recursive.Translated(__local_box_recursive.Basis.Y)) * __local_sphere_recursive;
            var get_sphere_pos = (Transform3D l_xf) => l_xf.Origin + (l_xf).Basis.Y;

            anim.PositionTrackInsertKey(0, time, get_sphere_pos(s_xf));
            anim.RotationTrackInsertKey(1, time, new Transform3D(Basis.Identity, get_sphere_pos(s_xf)).LookingAt(get_sphere_pos(next_s_xf), xf.Basis.Y).Basis.GetRotationQuaternion());
        }
#endif

        _draw_local_xf_box(xf * __local_box_recursive.Translated(__local_box_recursive.Basis.Y), thickness * _local_mul, max_depth, depth + 1);
    }


    void _draw_other_world()
    {
        using var s = DebugDraw3D.NewScopedConfig().SetViewport(dOtherWorldBox.GetViewport());
        DebugDraw3D.DrawBoxXf(dOtherWorldBox.GlobalTransform.RotatedLocal(new Vector3(1, 1, -1).Normalized(), Mathf.Wrap(Time.GetTicksMsec() / 1000.0f, 0f, Mathf.Tau)), Colors.SandyBrown);
        DebugDraw3D.DrawBoxXf(dOtherWorldBox.GlobalTransform.RotatedLocal(new Vector3(-1, 1, -1).Normalized(), Mathf.Wrap(Time.GetTicksMsec() / 1000.0f, 0f, Mathf.Tau) - Mathf.Pi / 4), Colors.SandyBrown);

        if (draw_3d_text)
        {
            var angle = Mathf.Wrap(Time.GetTicksMsec() / 1000.0f, 0, Mathf.Tau);
            using (var _w2 = DebugDraw3D.NewScopedConfig().SetTextFont(custom_3d_font).SetTextFixedSize(true))
            {
                DebugDraw3D.DrawText(dOtherWorldBox.GlobalPosition + new Vector3(Mathf.Cos(angle), -0.25f, Mathf.Sin(angle)), "Hello world!", 40, Colors.Crimson, 0);
            }

            using (var _w3 = DebugDraw3D.NewScopedConfig().SetNoDepthTest(true).SetTextOutlineColor(Colors.IndianRed).SetTextOutlineSize(6))
            {
                DebugDraw3D.DrawText(dOtherWorldBox.GlobalPosition + new Vector3(Mathf.Cos(angle), +0.25f, Mathf.Sin(-angle)), "World without depth", 20, Colors.Pink, 0);
            }
        }
    }

    void _draw_rays_casts()
    {
        // Line hits render
        _draw_zone_title_pos(pHitTestSphere.GlobalPosition, "Line hits", 96, 36);

        foreach (var node in dHitTest_RayEmitter.GetChildren())
        {
            if (node is RayCast3D ray)
            {
                ray.ForceRaycastUpdate();
                DebugDraw3D.DrawLineHit(ray.GlobalPosition, ray.ToGlobal(ray.TargetPosition), ray.GetCollisionPoint(), ray.IsColliding(), 0.3f);
            }
        }
    }

    void _more_tests()
    {
        // Delayed line render
        using (var s = DebugDraw3D.NewScopedConfig().SetThickness(0.035f))
        {
            DebugDraw3D.DrawLine(dLagTest.GlobalPosition + Vector3.Up, dLagTest.GlobalPosition + new Vector3(0, 3, Mathf.Sin(Time.GetTicksMsec() / 50.0f)), null, 0.35f);

            if (draw_3d_text)
            {
                DebugDraw3D.DrawText(dLagTest.GlobalPosition + new Vector3(0, 3, Mathf.Sin(Time.GetTicksMsec() / 50.0f)), $"{Mathf.Sin(Time.GetTicksMsec() / 50.0f):F1}", 16, null, 0.35f);
            }
        }

        // Draw plane
        using (var _s11 = DebugDraw3D.NewScopedConfig().SetThickness(0.02f).SetPlaneSize(10))
        {
            var pl_node = GetNode<Node3D>("PlaneOrigin");
            var xf = pl_node.GlobalTransform;
            var normal = xf.Basis.Y.Normalized();
            var plane = new Plane(normal, xf.Origin.Dot(normal));

            var vp = GetViewport();
            if (Engine.IsEditorHint() && (Viewport)Engine.GetSingleton("EditorInterface").Call("get_editor_viewport_3d", 0) != null)
            {
                vp = (Viewport)Engine.GetSingleton("EditorInterface").Call("get_editor_viewport_3d", 0);
            }

            var cam = vp.GetCamera3D();
            if (cam != null)
            {
                var dir = vp.GetCamera3D().ProjectRayNormal(vp.GetMousePosition());
                Vector3? intersect = plane.IntersectsRay(cam.GlobalPosition, dir);

                DebugDraw3D.DrawPlane(plane, Colors.Coral * new Color(1, 1, 1, 0.4f), pl_node.GlobalPosition);
                if (intersect.HasValue && intersect.Value.DistanceTo(pl_node.GlobalPosition) < _s11.GetPlaneSize() * 0.5f)
                {
                    // Need to test different colors on both sides of the plane
                    var col = plane.IsPointOver(cam.GlobalPosition) ? Colors.Firebrick : Colors.Aquamarine;
                    DebugDraw3D.DrawSphere(intersect.Value, 0.3f, col);
                }
            }
        }
    }

    void _draw_paths(Vector3 offset, int max_points = -1)
    {
        /// preparing data
        List<Vector3> points = [];
        List<Vector3> points_below = [];
        List<Vector3> points_below2 = [];
        List<Vector3> points_below3 = [];
        List<Vector3> points_below4 = [];
        List<Vector3> lines_above = [];

        var down_vec = -GlobalTransform.Basis.Y;
        for (int node_idx = 0; node_idx < (max_points < 0 ? dLinePath.GetChildCount() : Mathf.Min(dLinePath.GetChildCount(), max_points)); node_idx++)
        {
            var node = dLinePath.GetChild(node_idx);
            if (node is Node3D c)
            {
                points.Add(c.GlobalPosition + offset);
                points_below.Add(c.GlobalPosition + down_vec + offset);
                points_below2.Add(c.GlobalPosition + down_vec * 2 + offset);
                points_below3.Add(c.GlobalPosition + down_vec * 3 + offset);
                points_below4.Add(c.GlobalPosition + down_vec * 4 + offset);
            }
        }

        for (int x = 0; x < points.Count - 1; x++)
        {
            lines_above.Add(points[x] - down_vec);
            lines_above.Add(points[x + 1] - down_vec);
        }

        /// drawing lines
        DebugDraw3D.DrawLines(lines_above.ToArray());
        DebugDraw3D.DrawLinePath(points.ToArray(), Colors.Beige);
        DebugDraw3D.DrawPoints(points_below.ToArray(), DebugDraw3D.PointType.TypeSquare, 0.2f, Colors.DarkGreen);
        DebugDraw3D.DrawPointPath(points_below2.ToArray(), DebugDraw3D.PointType.TypeSquare, 0.25f, Colors.Blue, Colors.Tomato);
        DebugDraw3D.DrawArrowPath(points_below3.ToArray(), Colors.Gold, 0.5f);
        using (var _sl = DebugDraw3D.NewScopedConfig().SetThickness(0.05f))
            DebugDraw3D.DrawPointPath(points_below4.ToArray(), DebugDraw3D.PointType.TypeSphere, 0.25f, Colors.MediumSeaGreen, Colors.MediumVioletRed);

    }
    void _draw_array_of_boxes()
    {
        // Lots of boxes to check performance..
        var x_size = 50;
        var y_size = 50;
        var z_size = 3;
        var mul = 1.0f;
        var cubes_max_time = 1.25f;
        var show_text = draw_text_with_boxes;
        using var cfg = DebugDraw3D.NewScopedConfig();

        if (draw_1m_boxes)
        {
            x_size = 100;
            y_size = 100;
            z_size = 100;
            mul = 4.0f;
            cubes_max_time = 60f;
            draw_text_with_boxes = false;
        }

        var size = Vector3.One;
        var half_size = size * 0.5f;

        if (timer_cubes <= 0)
        {
            var start_time = Time.GetTicksUsec();
            for (int x = 0; x < x_size; x++)
            {
                for (int y = 0; y < y_size; y++)
                {
                    for (int z = 0; z < z_size; z++)
                    {
                        cfg.SetThickness(Random.Shared.NextSingle() * 0.1f);
                        var pos = new Vector3(x * mul, (-4 - z) * mul, y * mul) + GlobalPosition;
                        DebugDraw3D.DrawBox(pos, Quaternion.Identity, size, null, false, cubes_max_time);

                        if (show_text && z == 0)
                        {
                            DebugDraw3D.DrawText(pos + half_size, pos.ToString(), 32, null, cubes_max_time);
                        }
                    }
                }
            }
            GD.Print($"Draw Cubes C#: {((Time.GetTicksUsec() - start_time) / 1000.0):F3}ms");
            timer_cubes = cubes_max_time;
        }

        if (timer_cubes > cubes_max_time)
        {
            DebugDraw3D.ClearAll();
            timer_cubes = 0;
        }
    }

    public override void _Notification(int what)
    {
        if (what == NotificationEditorPreSave || what == NotificationExitTree)
            _thread_stop();
    }

    void _call_from_thread()
    {
        if (tests_use_threads && (test_thread == null || !test_thread[0].IsAlive))
        {
            test_thread_closing = false;
            test_thread = new Thread[4];
            for (int i = 0; i < test_thread.Length; i++)
            {
                test_thread[i] = new Thread(_thread_body);
                test_thread[i].Start();
            }
        }
        else if (!tests_use_threads && (test_thread != null && test_thread[0].IsAlive))
        {
            _thread_stop();
        }
    }

    void _thread_stop()
    {
        if (test_thread != null && test_thread[0].IsAlive)
        {
            tests_use_threads = false;
            test_thread_closing = true;
            for (int i = 0; i < test_thread.Length; i++)
            {
                test_thread[i].Join();
            }
            test_thread = null;
        }
    }

    void _thread_body()
    {
        GD.Print("C# Thread started!");
        while (!test_thread_closing)
        {
            DebugDraw3D.DrawBox(new Vector3(0, -1, 0), Quaternion.Identity, Vector3.One, Colors.Brown, true, 0.016f);

            var boxes = 10;
            for (int y = 0; y < boxes; y++)
            {
                var offset = (float)Mathf.Sin(Mathf.Tau / boxes * y + Mathf.Wrap(Time.GetTicksMsec() / 100.0, 0, Mathf.Tau));
                var pos = new Vector3(offset, y, 0);
                DebugDraw3D.DrawBox(pos, Quaternion.Identity, Vector3.One, Colors.GreenYellow, true, 0.016f);
                DebugDraw3D.DrawText(pos, y.ToString(), 64, Colors.White, 0.016f);

                if (y == 0)
                {
                    DebugDraw2D.SetText("thread. sin", offset.ToString());
                }
            }

            {
                var _mt_test_object_creation = new DebugDraw3DScopeConfig();
            }
            OS.DelayMsec(16);
        }
        GD.Print("C# Thread finished!");
    }

    Node3D dHitTest;
    CsgBox3D dLagTest;
    PanelContainer dPanel;
    Node3D dZones;
    Node3D dSpherePosition;
    Node3D dSphereTransform;
    Node3D dSphereHDTransform;
    Node3D dAABB;
    Node3D dAABB_fixed;
    Node3D dBox1;
    Node3D dBox2;
    Node3D dBox3;
    Node3D dBoxAB;
    Node3D dBoxABa;
    Node3D dBoxABb;
    Node3D dBoxABup;
    Node3D dBoxABEdge;
    Node3D dBoxABEdgea;
    Node3D dBoxABEdgeb;
    Node3D dBoxABEdgeup;
    Node3D dLines_1;
    Node3D dLines_2;
    Node3D dLines_3;
    Node3D dLines_4;
    Node3D dLines_5;
    Node3D dLines_6;
    Node3D dLines_7;
    Node3D dLines_8;
    Node3D dLines_Target;
    Node3D dLinePath;
    Node3D dCylinder1;
    Node3D dCylinder2;
    Node3D dCylinder3a;
    Node3D dCylinder3b;
    Node3D dCapsule1;
    Node3D dCapsule2a;
    Node3D dCapsule2b;

    Node3D pSpheresBox;
    Node3D pCylindersBox;
    Node3D pCapsulesBox;
    Node3D pBoxesBox;
    Node3D pLinesBox;
    Node3D pPathsBox;
    Node3D pMiscBox;

    MeshInstance3D dPlaneOrigin;
    MeshInstance3D pZDepthTestCube;

    MeshInstance3D dOtherWorld;
    SubViewport dOtherWorldViewport;
    Node3D dOtherWorldBox;

    Control dCustomCanvas;
    Node3D dMisc_Arrow;
    Camera3D dCamera;
    Node3D dMisc_Billboard;
    Node3D dMisc_Position;
    Node3D dMisc_GizmoTransform;
    Node3D dMisc_GizmoNormal;
    Node3D dMisc_GizmoOneColor;
    Node3D pLocalTransformRecursiveOrigin;
    AnimationPlayer pRecursiveTransformTest;

    Node3D dGrids_Grid;
    Node3D dGrids_Grid_Subdivision;
    Node3D dGrids_GridCentered_Subdivision;
    Node3D dGrids_GridCentered;

    MeshInstance3D dPostProcess;
    AnimationPlayer dLagTest_RESET;
    Node3D dHitTest_RayEmitter;
    Node3D pHitTestSphere;

    void _get_nodes()
    {
        dHitTest = GetNode<Node3D>("HitTest");
        dLagTest = GetNode<CsgBox3D>("LagTest");
        dPanel = GetNode<PanelContainer>("Panel");
        dZones = GetNode<Node3D>("Zones");
        dSpherePosition = GetNode<Node3D>("Spheres/SpherePosition");
        dSphereTransform = GetNode<Node3D>("Spheres/SphereTransform");
        dSphereHDTransform = GetNode<Node3D>("Spheres/SphereHDTransform");
        dAABB = GetNode<Node3D>("Boxes/AABB");
        dAABB_fixed = GetNode<Node3D>("Boxes/AABB_fixed");
        dBox1 = GetNode<Node3D>("Boxes/Box1");
        dBox2 = GetNode<Node3D>("Boxes/Box2");
        dBox3 = GetNode<Node3D>("Boxes/Box3");
        dBoxAB = GetNode<Node3D>("Boxes/BoxAB");
        dBoxABa = GetNode<Node3D>("Boxes/BoxAB/a");
        dBoxABb = GetNode<Node3D>("Boxes/BoxAB/b");
        dBoxABup = GetNode<Node3D>("Boxes/BoxAB/o/up");
        dBoxABEdge = GetNode<Node3D>("Boxes/BoxABEdge");
        dBoxABEdgea = GetNode<Node3D>("Boxes/BoxABEdge/a");
        dBoxABEdgeb = GetNode<Node3D>("Boxes/BoxABEdge/b");
        dBoxABEdgeup = GetNode<Node3D>("Boxes/BoxABEdge/o/up");
        dLines_1 = GetNode<Node3D>("Lines/1");
        dLines_2 = GetNode<Node3D>("Lines/2");
        dLines_3 = GetNode<Node3D>("Lines/3");
        dLines_4 = GetNode<Node3D>("Lines/4");
        dLines_5 = GetNode<Node3D>("Lines/5");
        dLines_6 = GetNode<Node3D>("Lines/6");
        dLines_7 = GetNode<Node3D>("Lines/7");
        dLines_8 = GetNode<Node3D>("Lines/8");
        dLines_Target = GetNode<Node3D>("Lines/Target");
        dLinePath = GetNode<Node3D>("LinePath");
        dCylinder1 = GetNode<Node3D>("Cylinders/Cylinder1");
        dCylinder2 = GetNode<Node3D>("Cylinders/Cylinder2");
        dCylinder3a = GetNode<Node3D>("Cylinders/Cylinder3/1");
        dCylinder3b = GetNode<Node3D>("Cylinders/Cylinder3/2");
        dCapsule1 = GetNode<Node3D>("Capsules/Capsule1");
        dCapsule2a = GetNode<Node3D>("Capsules/Capsule2/1");
        dCapsule2b = GetNode<Node3D>("Capsules/Capsule2/2");

        pSpheresBox = GetNode<Node3D>("%SpheresBox");
        pCylindersBox = GetNode<Node3D>("%CylindersBox");
        pCapsulesBox = GetNode<Node3D>("%CapsulesBox");
        pBoxesBox = GetNode<Node3D>("%BoxesBox");
        pLinesBox = GetNode<Node3D>("%LinesBox");
        pPathsBox = GetNode<Node3D>("%PathsBox");
        pMiscBox = GetNode<Node3D>("%MiscBox");

        dPlaneOrigin = GetNode<MeshInstance3D>("PlaneOrigin");
        pZDepthTestCube = GetNode<MeshInstance3D>("%ZDepthTestCube");

        dOtherWorld = GetNode<MeshInstance3D>("OtherWorld");
        dOtherWorldViewport = GetNode<SubViewport>("OtherWorld/SubViewport");
        dOtherWorldBox = GetNode<Node3D>("OtherWorld/SubViewport/SubViewportContainer/SubViewport/OtherWorldBox");

        dCustomCanvas = GetNode<Control>("CustomCanvas");
        dMisc_Arrow = GetNode<Node3D>("Misc/Arrow");
        dCamera = GetNode<Camera3D>("Camera");
        dMisc_Billboard = GetNode<Node3D>("Misc/Billboard");
        dMisc_Position = GetNode<Node3D>("Misc/Position");
        dMisc_GizmoTransform = GetNode<Node3D>("Misc/GizmoTransform");
        dMisc_GizmoNormal = GetNode<Node3D>("Misc/GizmoNormal");
        dMisc_GizmoOneColor = GetNode<Node3D>("Misc/GizmoOneColor");
        pLocalTransformRecursiveOrigin = GetNode<Node3D>("%LocalTransformRecursiveOrigin");
        pRecursiveTransformTest = GetNode<AnimationPlayer>("%RecursiveTransformTest");

        dGrids_Grid = GetNode<Node3D>("Grids/Grid");
        dGrids_Grid_Subdivision = GetNode<Node3D>("Grids/Grid/Subdivision");
        dGrids_GridCentered_Subdivision = GetNode<Node3D>("Grids/GridCentered/Subdivision");
        dGrids_GridCentered = GetNode<Node3D>("Grids/GridCentered");

        dPostProcess = GetNode<MeshInstance3D>("PostProcess");

        dLagTest_RESET = GetNode<AnimationPlayer>("LagTest/RESET");
        dHitTest_RayEmitter = GetNode<Node3D>("HitTest/RayEmitter");
        pHitTestSphere = GetNode<Node3D>("%HitTestSphere");
    }
}
