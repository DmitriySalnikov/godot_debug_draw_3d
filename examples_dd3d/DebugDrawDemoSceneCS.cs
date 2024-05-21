
using Godot;
using System;
using System.Collections.Generic;

[Tool]
public partial class DebugDrawDemoSceneCS : Node3D
{
    Random random = new Random();

    [Export] Font custom_font;
    [Export] bool zylann_example = false;
    [Export] bool update_in_physics = false;
    [Export] bool test_text = true;
    [Export] bool test_graphs = false;
    [Export] bool more_test_cases = true;
    [Export] bool draw_array_of_boxes = false;
    [Export] bool draw_1m_boxes = false;
    [Export(PropertyHint.Range, "0, 5, 0.001")] float debug_thickness = 0.1f;
    [Export(PropertyHint.Range, "0, 1")] float camera_frustum_scale = 0.9f;

    [ExportGroup("Text groups", "text_groups")]
    [Export] bool text_groups_show_hints = true;
    [Export] bool text_groups_show_stats = true;
    [Export] bool text_groups_show_stats_2d = true;
    [Export] DebugDraw2DConfig.BlockPosition text_groups_position = DebugDraw2DConfig.BlockPosition.LeftTop;
    [Export] Vector2I text_groups_offset = new Vector2I(8, 8);
    [Export] Vector2I text_groups_padding = new Vector2I(3, 1);
    [Export(PropertyHint.Range, "1, 100")] int text_groups_default_font_size = 15;
    [Export(PropertyHint.Range, "1, 100")] int text_groups_title_font_size = 20;
    [Export(PropertyHint.Range, "1, 100")] int text_groups_text_font_size = 17;

    [ExportGroup("Graphs", "graph")]
    [Export] Vector2I graph_offset = new Vector2I(8, 8);
    [Export] Vector2I graph_size = new Vector2I(200, 80);
    [Export(PropertyHint.Range, "1, 100")] int graph_title_font_size = 14;
    [Export(PropertyHint.Range, "1, 100")] int graph_text_font_size = 12;
    [Export(PropertyHint.Range, "0, 64")] int graph_text_precision = 1;
    [Export(PropertyHint.Range, "1, 32")] float graph_line_width = 1.0f;
    [Export(PropertyHint.Range, "1, 512")] int graph_buffer_size = 128;
    [Export] bool graph_frame_time_mode = true;
    [Export] bool graph_is_enabled = true;

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

    // TODO remove after moving to 4.2
    bool is_4_2_and_higher = ((int)Engine.GetVersionInfo()["major"]) >= 4 && ((int)Engine.GetVersionInfo()["minor"]) >= 2;

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

    Node3D dMisc_Grids_Grid;
    Node3D dMisc_Grids_Grid_Subdivision;
    Node3D dMisc_Grids_GridCentered_Subdivision;
    Node3D dMisc_Grids_GridCentered;

    MeshInstance3D dPostProcess;
    AnimationPlayer dLagTest_RESET;
    Node3D dHitTest_RayEmitter;


    public override async void _Ready()
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

        dOtherWorld = GetNode<MeshInstance3D>("OtherWorld");
        dOtherWorldViewport = GetNode<SubViewport>("OtherWorld/SubViewport");
        dOtherWorldBox = GetNode<Node3D>("OtherWorld/SubViewport/OtherWorldBox");

        dCustomCanvas = GetNode<Control>("CustomCanvas");
        dMisc_Arrow = GetNode<Node3D>("Misc/Arrow");
        dCamera = GetNode<Camera3D>("Camera");
        dMisc_Billboard = GetNode<Node3D>("Misc/Billboard");
        dMisc_Position = GetNode<Node3D>("Misc/Position");
        dMisc_GizmoTransform = GetNode<Node3D>("Misc/GizmoTransform");
        dMisc_GizmoNormal = GetNode<Node3D>("Misc/GizmoNormal");
        dMisc_GizmoOneColor = GetNode<Node3D>("Misc/GizmoOneColor");

        dMisc_Grids_Grid = GetNode<Node3D>("Grids/Grid");
        dMisc_Grids_Grid_Subdivision = GetNode<Node3D>("Grids/Grid/Subdivision");
        dMisc_Grids_GridCentered_Subdivision = GetNode<Node3D>("Grids/GridCentered/Subdivision");
        dMisc_Grids_GridCentered = GetNode<Node3D>("Grids/GridCentered");

        dPostProcess = GetNode<MeshInstance3D>("PostProcess");

        dLagTest_RESET = GetNode<AnimationPlayer>("LagTest/RESET");
        dHitTest_RayEmitter = GetNode<Node3D>("HitTest/RayEmitter");

        _update_keys_just_press();

        await new SignalAwaiter(GetTree(), "process_frame", this);

        // this check is required for inherited scenes, because an instance of this 
        // script is created first, and then overridden by another
        if (!IsInsideTree())
            return;

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
        DebugDraw3D.ScopedConfig().SetThickness(debug_thickness);
#pragma warning disable CS0162 // Unreachable code detected
        if (false) // #test
        {
            using var s11 = DebugDraw3D.NewScopedConfig().SetThickness(1);
            using var s13 = DebugDraw3D.NewScopedConfig();
            s13.SetThickness(3);
        }
#pragma warning restore CS0162 // Unreachable code detected

        _update_keys_just_press();

        if (_is_key_just_pressed(Key.F1))
            zylann_example = !zylann_example;

        // Zylann's example :D
        if (zylann_example)
        {
            DebugDraw2D.ClearGraphs();
            var _time = Time.GetTicksMsec() / 1000.0f;
            var box_pos = new Vector3(0, Mathf.Sin(_time * 4f), 0);
            var line_begin = new Vector3(-1, Mathf.Sin(_time * 4f), 0);
            var line_end = new Vector3(1, Mathf.Cos(_time * 4f), 0);
            DebugDraw3D.DrawBox(box_pos, Quaternion.Identity, new Vector3(1, 2, 1), new Color(0, 1, 0));
            DebugDraw3D.DrawLine(line_begin, line_end, new Color(1, 1, 0));
            DebugDraw2D.SetText("Time", _time);
            DebugDraw2D.SetText("Frames drawn", Engine.GetFramesDrawn());
            DebugDraw2D.SetText("FPS", Engine.GetFramesPerSecond());
            DebugDraw2D.SetText("delta", delta);
            dHitTest.Visible = false;
            dLagTest.Visible = false;
            return;
        }

        dHitTest.Visible = true;
        dLagTest.Visible = true;

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
            DebugDraw3D.Config.UseFrustumCulling = !DebugDraw3D.Config.UseFrustumCulling;

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
        DebugDraw3D.DrawSphereXf(dSphereTransform.GlobalTransform, Colors.Crimson);
        using (var _s1 = DebugDraw3D.NewScopedConfig().SetHdSphere(true))
            DebugDraw3D.DrawSphereXf(dSphereHDTransform.GlobalTransform, Colors.OrangeRed);

        // Delayed spheres
        if (timer_1 <= 0)
        {
            DebugDraw3D.DrawSphere(dSpherePosition.GlobalPosition, 2.0f, Colors.BlueViolet, 2.0f);
            using (var _s1 = DebugDraw3D.NewScopedConfig().SetHdSphere(true))
                DebugDraw3D.DrawSphere(dSpherePosition.GlobalPosition + Vector3.Forward * 4, 2.0f, Colors.CornflowerBlue, 2.0f);
            timer_1 = 2;
        }

        timer_1 -= delta;

        // Cylinders
        DebugDraw3D.DrawCylinder(dCylinder1.GlobalTransform, Colors.Crimson);
        DebugDraw3D.DrawCylinder(new Transform3D(Basis.Identity.Scaled(new Vector3(1, 2, 1)), dCylinder2.GlobalPosition), Colors.Red);
        DebugDraw3D.DrawCylinderAb(dCylinder3a.GlobalPosition, dCylinder3b.GlobalPosition, 0.7f);

        // Boxes
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
        DebugDraw3D.DrawSquare(dLines_Target.GlobalPosition, 0.5f, Colors.Red);

        DebugDraw3D.DrawLine(dLines_1.GlobalPosition, dLines_Target.GlobalPosition, Colors.Fuchsia);
        DebugDraw3D.DrawRay(dLines_3.GlobalPosition, (dLines_Target.GlobalPosition - dLines_3.GlobalPosition).Normalized(), 3.0f, Colors.Crimson);


        if (timer_3 <= 0)
        {
            DebugDraw3D.DrawLine(dLines_6.GlobalPosition, dLines_Target.GlobalPosition, Colors.Fuchsia, 2.0f);
            timer_3 = 2;
        }

        timer_3 -= delta;

        // Test UP vector
        DebugDraw3D.DrawLine(dLines_7.GlobalPosition, dLines_Target.GlobalPosition, Colors.Red);

        // Lines with Arrow
        DebugDraw3D.DrawArrow(dLines_2.GlobalPosition, dLines_Target.GlobalPosition, Colors.Blue, 0.5f, true);
        DebugDraw3D.DrawArrowRay(dLines_4.GlobalPosition, (dLines_Target.GlobalPosition - dLines_4.GlobalPosition).Normalized(), 8.0f, Colors.Lavender, 0.5f, true);

        DebugDraw3D.DrawLineHitOffset(dLines_5.GlobalPosition, dLines_Target.GlobalPosition, true, Mathf.Abs(Mathf.Sin(Time.GetTicksMsec() / 1000.0f)), 0.25f, Colors.Aqua);

        // Path

        // preparing data
        List<Vector3> points = new List<Vector3>();
        List<Vector3> points_below = new List<Vector3>();
        List<Vector3> points_below2 = new List<Vector3>();
        List<Vector3> points_below3 = new List<Vector3>();
        List<Vector3> points_below4 = new List<Vector3>();
        List<Vector3> lines_above = new List<Vector3>();

        foreach (var node in dLinePath.GetChildren())
        {
            if (node is Node3D c)
            {
                points.Add(c.GlobalPosition);
                points_below.Add(c.GlobalPosition + Vector3.Down);
                points_below2.Add(c.GlobalPosition + Vector3.Down * 2);
                points_below3.Add(c.GlobalPosition + Vector3.Down * 3);
                points_below4.Add(c.GlobalPosition + Vector3.Down * 4);
            }
        }

        for (int x = 0; x < points.Count - 1; x++)
        {
            lines_above.Add(points[x] + Vector3.Up);
            lines_above.Add(points[x + 1] + Vector3.Up);
        }

        // drawing lines
        DebugDraw3D.DrawLines(lines_above.ToArray());
        DebugDraw3D.DrawLinePath(points.ToArray(), Colors.Beige);
        DebugDraw3D.DrawPoints(points_below.ToArray(), DebugDraw3D.PointType.TypeSquare, 0.2f, Colors.DarkGreen);
        DebugDraw3D.DrawPointPath(points_below2.ToArray(), DebugDraw3D.PointType.TypeSquare, 0.25f, Colors.Blue, Colors.Tomato);
        DebugDraw3D.DrawArrowPath(points_below3.ToArray(), Colors.Gold, 0.5f);
        using (var _sl = DebugDraw3D.NewScopedConfig().SetThickness(0.05f))
            DebugDraw3D.DrawPointPath(points_below4.ToArray(), DebugDraw3D.PointType.TypeSphere, 0.25f, Colors.MediumSeaGreen, Colors.MediumVioletRed);

        // Other world

        using (var s = DebugDraw3D.NewScopedConfig().SetViewport(dOtherWorldBox.GetViewport()))
        {
            DebugDraw3D.DrawBoxXf(dOtherWorldBox.GlobalTransform.RotatedLocal(new Vector3(1, 1, -1).Normalized(), Mathf.Wrap(Time.GetTicksMsec() / 1000.0f, 0f, Mathf.Tau)), Colors.SandyBrown);
            DebugDraw3D.DrawBoxXf(dOtherWorldBox.GlobalTransform.RotatedLocal(new Vector3(-1, 1, -1).Normalized(), Mathf.Wrap(Time.GetTicksMsec() / 1000.0f, 0f, Mathf.Tau) - Mathf.Pi / 4), Colors.SandyBrown);
        }

        // Misc
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

        Transform3D tg = dMisc_Grids_Grid.GlobalTransform;
        Vector3 tn = dMisc_Grids_Grid_Subdivision.Transform.Origin;
        DebugDraw3D.DrawGrid(tg.Origin, tg.Basis.X, tg.Basis.Z, new Vector2I((int)tn.X * 10, (int)tn.Z * 10), Colors.LightCoral, false);

        var tn1 = dMisc_Grids_GridCentered_Subdivision.Transform.Origin;
        DebugDraw3D.DrawGridXf(dMisc_Grids_GridCentered.GlobalTransform, new Vector2I((int)(tn1.X * 10), (int)(tn1.Z * 10)));

        using (var s = DebugDraw3D.NewScopedConfig().SetThickness(0.05f))
        {
            DebugDraw3D.DrawBoxXf(dPostProcess.GlobalTransform, Colors.SeaGreen);
        }

        // 2D
        DebugDraw2D.Config.TextDefaultSize = text_groups_default_font_size;
        DebugDraw2D.Config.TextBlockOffset = text_groups_offset;
        DebugDraw2D.Config.TextBlockPosition = text_groups_position;
        DebugDraw2D.Config.TextPadding = text_groups_padding;

        DebugDraw2D.Config.TextCustomFont = custom_font;


        if (test_text)
        {
            timer_text -= delta;
            _text_tests();
        }

        // Graphs
        // Enable FPSGraph if not exists
        _create_graph("FPS", true, false, DebugDraw2DGraph.TextFlags.Current | DebugDraw2DGraph.TextFlags.Avg | DebugDraw2DGraph.TextFlags.Max | DebugDraw2DGraph.TextFlags.Min, "", DebugDraw2DGraph.GraphSide.Bottom, Engine.IsEditorHint() ? DebugDraw2DGraph.GraphPosition.LeftTop : DebugDraw2DGraph.GraphPosition.RightTop, new Vector2I(200, 80), custom_font);
        if (Engine.IsEditorHint())
        {
            if (DebugDraw2D.GetGraph("FPS") != null)
            {
                DebugDraw2D.GetGraph("FPS").Offset = new Vector2I(0, 64);
            }
        }

        // Adding more graphs
        if (test_graphs)
        {
            _graph_test();
        }
        else
        {
            _remove_graphs();
        }
        _upd_graph_params();

        // Lag Test
        dLagTest.Position = ((Vector3)dLagTest_RESET.GetAnimation("RESET").TrackGetKeyValue(0, 0)) + new Vector3(Mathf.Sin(Time.GetTicksMsec() / 100.0f) * 2.5f, 0, 0);
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

        if (draw_array_of_boxes)
        {
            _draw_array_of_boxes();
        }

    }

    void _text_tests()
    {

        if (timer_text < 0)
        {
            DebugDraw2D.SetText("Some delayed text", "for 2.5s", -1, Colors.Black, 2.5f); // it's supposed to show text for 2.5 seconds
            timer_text += 5;
        }

        DebugDraw2D.SetText("FPS", $"{Engine.GetFramesPerSecond():F2}", 0, Colors.Gold);
        DebugDraw2D.BeginTextGroup("-- First Group --", 2, Colors.LimeGreen, true, text_groups_title_font_size, text_groups_text_font_size);
        DebugDraw2D.SetText("Simple text");
        DebugDraw2D.SetText("Text", "Value", 0, Colors.Aquamarine);
        DebugDraw2D.SetText("Text out of order", null, -1, Colors.Silver);
        DebugDraw2D.BeginTextGroup("-- Second Group --", 1, Colors.Beige);
        DebugDraw2D.SetText("Rendered frames", Engine.GetFramesDrawn());
        DebugDraw2D.EndTextGroup();

        if (text_groups_show_stats)
        {
            DebugDraw2D.BeginTextGroup("-- Stats --", 3, Colors.Wheat);
            var render_stats = DebugDraw3D.GetRenderStats();

            if (render_stats != null && text_groups_show_stats)
            {
                DebugDraw2D.SetText("Total", render_stats.TotalGeometry);
                DebugDraw2D.SetText("Instances", render_stats.Instances, 1);
                DebugDraw2D.SetText("Lines", render_stats.Lines, 2);
                DebugDraw2D.SetText("Total Visible", render_stats.TotalVisible, 3);
                DebugDraw2D.SetText("Visible Instances", render_stats.VisibleInstances, 4);
                DebugDraw2D.SetText("Visible Lines", render_stats.VisibleLines, 5);

                DebugDraw2D.SetText("---", "", 6);

                DebugDraw2D.SetText("Culling time", $"{(render_stats.TotalTimeCullingUsec / 1000.0):F2} ms", 7);
                DebugDraw2D.SetText("Filling instances buffer", $"{(render_stats.TimeFillingBuffersInstancesUsec / 1000.0):F2} ms", 8);
                DebugDraw2D.SetText("Filling lines buffer", $"{(render_stats.TimeFillingBuffersLinesUsec / 1000.0):F2} ms", 9);
                DebugDraw2D.SetText("Filling time", $"{(render_stats.TotalTimeFillingBuffersUsec / 1000.0):F2} ms", 10);
                DebugDraw2D.SetText("Total time", $"{(render_stats.TotalTimeSpentUsec / 1000.0):F2} ms", 11);

                DebugDraw2D.SetText("---", null, 14);

                DebugDraw2D.SetText("Created scoped configs", $"{render_stats.CreatedScopedConfigs}", 15);
            }

            if (text_groups_show_stats && text_groups_show_stats_2d)
            {
                DebugDraw2D.SetText("----", null, 19);
            }

            var render_stats_2d = DebugDraw2D.GetRenderStats();
            if (render_stats_2d != null && text_groups_show_stats_2d)
            {
                DebugDraw2D.SetText("Text groups", render_stats_2d.OverlayTextGroups, 20);
                DebugDraw2D.SetText("Text lines", render_stats_2d.OverlayTextLines, 21);
                DebugDraw2D.SetText("Graphs total", render_stats_2d.OverlayGraphsTotal, 22);
                DebugDraw2D.SetText("Graphs enabled", render_stats_2d.OverlayGraphsEnabled, 23);
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
            DebugDraw2D.SetText("Alt: change render layers", DebugDraw3D.Config.GeometryRenderLayers, 1);
            if (!OS.HasFeature("web"))
            {
                DebugDraw2D.SetText("Ctrl: toggle anti-aliasing", GetViewport().Msaa3D == Viewport.Msaa.Msaa4X ? "MSAA 4x" : "Disabled", 2);
            }
            DebugDraw2D.SetText("Down: freeze render", DebugDraw3D.Config.Freeze3dRender, 3);
            if (Engine.IsEditorHint())
            {
                DebugDraw2D.SetText("Up: use scene camera", DebugDraw3D.Config.ForceUseCameraFromScene, 4);
            }
            DebugDraw2D.SetText("1,2,3: toggle debug", $"{DebugDraw3D.DebugEnabled}, {DebugDraw2D.DebugEnabled} 😐, {DebugDrawManager.DebugEnabled} 😏", 5);
            DebugDraw2D.SetText("Left: toggle frustum culling", DebugDraw3D.Config.UseFrustumCulling, 6);
            DebugDraw2D.SetText("Right: draw bounds for culling", DebugDraw3D.Config.VisibleInstanceBounds, 7);
        }
    }

    void _draw_rays_casts()
    {
        // Line hits render
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
            DebugDraw3D.DrawLine(dLagTest.GlobalPosition + Vector3.Up, dLagTest.GlobalPosition + new Vector3(0, 3, Mathf.Sin(Time.GetTicksMsec() / 50.0f)), null, 0.5f);
        }

        // Draw plane
        using (var _s11 = DebugDraw3D.NewScopedConfig().SetThickness(0.02f).SetPlaneSize(10))
        {
            var pl_node = GetNode<Node3D>("PlaneOrigin");
            var xf = pl_node.GlobalTransform;
            var normal = xf.Basis.Y.Normalized();
            var plane = new Plane(normal, xf.Origin.Dot(normal));

            var vp = GetViewport();
            if (is_4_2_and_higher)
            {
                if (Engine.IsEditorHint() && (Viewport)Engine.GetSingleton("EditorInterface").Call("get_editor_viewport_3d", 0) != null)
                {
                    vp = (Viewport)Engine.GetSingleton("EditorInterface").Call("get_editor_viewport_3d", 0);
                }
            }

            var cam = vp.GetCamera3D();
            if (cam != null)
            {
                var dir = vp.GetCamera3D().ProjectRayNormal(vp.GetMousePosition());
                Vector3? intersect = plane.IntersectsRay(cam.GlobalPosition, dir);

                DebugDraw3D.DrawPlane(plane, Colors.Coral * new Color(1, 1, 1, 0.4f), pl_node.GlobalPosition);
                if (is_4_2_and_higher)
                {
                    if (intersect.HasValue && intersect.Value.DistanceTo(pl_node.GlobalPosition) < _s11.GetPlaneSize() * 0.5f)
                    {
                        // Need to test different colors on both sides of the plane
                        var col = plane.IsPointOver(cam.GlobalPosition) ? Colors.Firebrick : Colors.Aquamarine;
                        DebugDraw3D.DrawSphere(intersect.Value, 0.3f, col);
                    }
                }
            }
        }
    }

    void _draw_array_of_boxes()
    {
        // Lots of boxes to check performance..
        var x_size = 50;
        var y_size = 50;
        var z_size = 3;
        var mul = 1.0f;
        var cubes_max_time = 1.25f;
        using var cfg = DebugDraw3D.NewScopedConfig();

        if (draw_1m_boxes)
        {
            x_size = 100;
            y_size = 100;
            z_size = 100;
            mul = 4.0f;
            cubes_max_time = 60f;
        }

        if (timer_cubes <= 0)
        {
            var start_time = Time.GetTicksUsec();
            for (int x = 0; x < x_size; x++)
            {
                for (int y = 0; y < y_size; y++)
                {
                    for (int z = 0; z < z_size; z++)
                    {
                        var size = Vector3.One;
                        cfg.SetThickness(Random.Shared.NextSingle() * 0.1f);
                        //size = new Vector3(Random.Shared.NextSingle() * 100 + 0.1f, Random.Shared.NextSingle() * 100 + 0.1f, Random.Shared.NextSingle() * 100 + 0.1f);
                        DebugDraw3D.DrawBox(new Vector3(x * mul, (-4 - z) * mul, y * mul), Quaternion.Identity, size, null, false, cubes_max_time);
                    }
                }
            }
            //GD.Print($"Draw Cubes: {((Time.GetTicksUsec() - start_time) / 1000.0):F2}ms");
            timer_cubes = cubes_max_time;
        }
    }

    void _graph_test()
    {
        _create_graph("fps", true, true, DebugDraw2DGraph.TextFlags.Current, "", DebugDraw2DGraph.GraphSide.Left, DebugDraw2DGraph.GraphPosition.RightTop);
        _create_graph("fps2", true, false, DebugDraw2DGraph.TextFlags.Current, "fps", DebugDraw2DGraph.GraphSide.Bottom, 0, new Vector2I(200, 100));

        _create_graph("Sin Wave!", false, true, DebugDraw2DGraph.TextFlags.Current, "fps2", DebugDraw2DGraph.GraphSide.Bottom);

        _create_graph("randf", false, true, DebugDraw2DGraph.TextFlags.Avg, "", DebugDraw2DGraph.GraphSide.Left, DebugDraw2DGraph.GraphPosition.RightBottom, new Vector2I(256, 60), custom_font);

        _create_graph("fps5", true, true, DebugDraw2DGraph.TextFlags.All, "randf", DebugDraw2DGraph.GraphSide.Top);
        _create_graph("fps6", true, true, DebugDraw2DGraph.TextFlags.All, "fps5", DebugDraw2DGraph.GraphSide.Top);
        _create_graph("fps12", true, true, DebugDraw2DGraph.TextFlags.All, "fps5", DebugDraw2DGraph.GraphSide.Left);

        _create_graph("fps7", true, false, DebugDraw2DGraph.TextFlags.All, "FPS", DebugDraw2DGraph.GraphSide.Bottom);
        _create_graph("fps8", true, true, DebugDraw2DGraph.TextFlags.All, "", DebugDraw2DGraph.GraphSide.Top, DebugDraw2DGraph.GraphPosition.LeftBottom);
        _create_graph("fps9", true, false, DebugDraw2DGraph.TextFlags.All, "fps8", DebugDraw2DGraph.GraphSide.Right);
        _create_graph("fps10", true, false, DebugDraw2DGraph.TextFlags.All, "fps8", DebugDraw2DGraph.GraphSide.Top);
        _create_graph("fps11", true, true, DebugDraw2DGraph.TextFlags.All, "fps9", DebugDraw2DGraph.GraphSide.Right);

        // If graphs exists, then more tests are done
        DebugDraw2D.GetGraph("Sin Wave!").DataGetter = new Callable(this, "_get_sin_wave_for_graph");
        DebugDraw2D.GetGraph("Sin Wave!").UpsideDown = false;

        DebugDraw2D.GetGraph("randf").TextSuffix = "utf8 ноль zéro";
        //DebugDraw2D.GetGraph("fps9").line_position = DebugDraw2DGraph.LINE_TOP
        DebugDraw2D.GetGraph("fps9").Offset = new Vector2I(0, 0);
        //DebugDraw2D.GetGraph("fps11").LlinePosition = DebugDraw2DGraph.LINE_BOTTOM
        DebugDraw2D.GetGraph("fps11").Offset = new Vector2I(16, 0);
        DebugDraw2D.GetGraph("fps6").Offset = new Vector2I(0, 32);
        DebugDraw2D.GetGraph("fps").Offset = new Vector2I(16, 72);
        DebugDraw2D.GetGraph("fps9").Enabled = graph_is_enabled;

        if (!Engine.IsEditorHint())
        {
            DebugDraw2D.GetGraph("fps").Corner = DebugDraw2DGraph.GraphPosition.LeftTop;
        }

        // Just sending random data to the graph
        DebugDraw2D.GraphUpdateData("randf", (float)random.NextDouble());
    }

    void _upd_graph_params()
    {
        DebugDraw2D.Config.GraphsBaseOffset = graph_offset;
        foreach (var g in new string[] { "FPS", "fps5", "fps8" })
        {
            var graph = DebugDraw2D.GetGraph(g) as DebugDraw2DFPSGraph;
            if (graph != null)
            {

                graph.Size = graph_size;
                graph.TitleSize = graph_title_font_size;
                graph.TextSize = graph_text_font_size;
                graph.LineWidth = graph_line_width;
                graph.TextPrecision = graph_text_precision;
                graph.BufferSize = graph_buffer_size;
                if (Engine.IsEditorHint() || g != "FPS")
                {
                    graph.FrameTimeMode = graph_frame_time_mode;
                }
            }
        }
    }

    float _get_sin_wave_for_graph()
    {
        var mul = Input.IsKeyPressed(Key.End) ? 4 : 2;
        return (float)Mathf.Sin(Engine.GetFramesDrawn() * 0.5) * mul;
    }

    void _remove_graphs()
    {
        DebugDraw2D.RemoveGraph("randf");
        DebugDraw2D.RemoveGraph("fps");
        DebugDraw2D.RemoveGraph("fps2");
        DebugDraw2D.RemoveGraph("Sin Wave!");
        DebugDraw2D.RemoveGraph("fps5");
        DebugDraw2D.RemoveGraph("fps6");
        DebugDraw2D.RemoveGraph("fps7");
        DebugDraw2D.RemoveGraph("fps8");
        DebugDraw2D.RemoveGraph("fps9");
        DebugDraw2D.RemoveGraph("fps10");
        DebugDraw2D.RemoveGraph("fps11");
        DebugDraw2D.RemoveGraph("fps12");
    }

    DebugDraw2DGraph _create_graph(string title, bool is_fps, bool show_title, DebugDraw2DGraph.TextFlags flags, string parent = "", DebugDraw2DGraph.GraphSide parent_side = DebugDraw2DGraph.GraphSide.Bottom, DebugDraw2DGraph.GraphPosition pos = DebugDraw2DGraph.GraphPosition.LeftBottom, Vector2I? size = null, Font font = null)
    {
        var graph = DebugDraw2D.GetGraph(title);
        if (graph == null)
        {
            if (is_fps)
            {
                graph = DebugDraw2D.CreateFpsGraph(title);
            }
            else
            {
                graph = DebugDraw2D.CreateGraph(title);
            }

            if (graph != null)
            {
                graph.Size = size ?? new Vector2I(256, 60);
                graph.BufferSize = 50;
                graph.Corner = pos;
                graph.ShowTitle = show_title;
                graph.ShowTextFlags = flags;
                graph.CustomFont = font;
                graph.SetParent(parent, parent_side);
            }
        }
        return graph;
    }
}
