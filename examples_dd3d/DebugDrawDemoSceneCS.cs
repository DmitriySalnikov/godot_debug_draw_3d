
using Godot;
using System;
using System.Collections.Generic;

[Tool]
public partial class DebugDrawDemoSceneCS : Node3D
{
    Random random = new Random();

    [Export] Font custom_font;
    [Export] bool zylann_example = false;
    [Export] bool test_text = true;
    [Export] bool test_graphs = false;
    [Export] bool more_test_cases = true;
    [Export] bool draw_array_of_boxes = false;
    [Export(PropertyHint.Range, "0, 1024")] float start_culling_distance = 0.0f;

    [ExportGroup("Text groups", "text_groups")]
    [Export] bool text_groups_show_hints = true;
    [Export] bool text_groups_show_stats = true;
    [Export] bool text_groups_show_stats_2d = true;
    [Export] DebugDrawConfig2D.BlockPosition text_groups_position = DebugDrawConfig2D.BlockPosition.LeftTop;
    [Export] Vector2I text_groups_offset = new Vector2I(8, 8);
    [Export] Vector2I text_groups_padding = new Vector2I(3, 1);
    [Export(PropertyHint.Range, "1, 100")] int text_groups_default_font_size = 12;
    [Export(PropertyHint.Range, "1, 100")] int text_groups_title_font_size = 14;
    [Export(PropertyHint.Range, "1, 100")] int text_groups_text_font_size = 12;

    [ExportGroup("Graphs", "graph")]
    [Export] Vector2I graph_offset = new Vector2I(8, 8);
    [Export] Vector2I graph_size = new Vector2I(200, 80);
    [Export(PropertyHint.Range, "1, 100")] int graph_title_font_size = 14;
    [Export(PropertyHint.Range, "1, 100")] int graph_text_font_size = 12;
    [Export(PropertyHint.Range, "0, 64")] int graph_text_precision = 2;
    [Export(PropertyHint.Range, "1, 32")] float graph_line_width = 1.0f;
    [Export(PropertyHint.Range, "1, 512")] int graph_buffer_size = 128;
    [Export] bool graph_frame_time_mode = true;
    [Export] bool graph_is_enabled = true;

    Dictionary<Key, int> button_presses = new Dictionary<Key, int>() {
        { Key.Left, 0 },
        { Key.Up, 0 },
        { Key.F1, 0 },
        { Key.Key1, 0 },
        { Key.Key2, 0 },
        { Key.Key3, 0 },
    };

    double time = 0.0;
    double time2 = 0.0;
    double time3 = 0.0;
    double time_text = 0.0;

    Node3D dHitTest;
    CsgBox3D dLagTest;
    PanelContainer dPanel;
    Node3D dZones;
    Node3D dAABB;
    Node3D dAABB_fixed;
    Node3D dSpherePosition;
    Node3D dSphereTransform;
    Node3D dSphereHDTransform;
    Node3D dBox1;
    Node3D dBox2;
    Node3D dBox3;
    Node3D dBoxOutOfDistanceCulling;
    Node3D dLines_1;
    Node3D dLines_3;
    Node3D dLines_6;
    Node3D dLines_2;
    Node3D dLines_4;
    Node3D dLines_5;
    Node3D dLines_Target;
    Node3D dLinePath;
    Node3D dCylinder1;
    Node3D dCylinder2;

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
    AnimationPlayer dLagTest_RESET;
    Node3D dHitTest_RayEmitter;


    public override async void _Ready()
    {
        dHitTest = GetNode<Node3D>("HitTest");
        dLagTest = GetNode<CsgBox3D>("LagTest");
        dPanel = GetNode<PanelContainer>("Panel");
        dZones = GetNode<Node3D>("Zones");
        dAABB = GetNode<Node3D>("AABB");
        dAABB_fixed = GetNode<Node3D>("AABB_fixed");
        dSpherePosition = GetNode<Node3D>("SpherePosition");
        dSphereTransform = GetNode<Node3D>("SphereTransform");
        dSphereHDTransform = GetNode<Node3D>("SphereHDTransform");
        dBox1 = GetNode<Node3D>("Box1");
        dBox2 = GetNode<Node3D>("Box2");
        dBox3 = GetNode<Node3D>("Box3");
        dBoxOutOfDistanceCulling = GetNode<Node3D>("BoxOutOfDistanceCulling");
        dLines_1 = GetNode<Node3D>("Lines/1");
        dLines_3 = GetNode<Node3D>("Lines/3");
        dLines_6 = GetNode<Node3D>("Lines/6");
        dLines_2 = GetNode<Node3D>("Lines/2");
        dLines_4 = GetNode<Node3D>("Lines/4");
        dLines_5 = GetNode<Node3D>("Lines/5");
        dLines_Target = GetNode<Node3D>("Lines/Target");
        dLinePath = GetNode<Node3D>("LinePath");
        dCylinder1 = GetNode<Node3D>("Cylinder1");
        dCylinder2 = GetNode<Node3D>("Cylinder2");

        dCustomCanvas = GetNode<Control>("CustomCanvas");
        dMisc_Arrow = GetNode<Node3D>("Misc/Arrow");
        dCamera = GetNode<Camera3D>("Camera");
        dMisc_Billboard = GetNode<Node3D>("Misc/Billboard");
        dMisc_Position = GetNode<Node3D>("Misc/Position");
        dMisc_GizmoTransform = GetNode<Node3D>("Misc/GizmoTransform");
        dMisc_GizmoNormal = GetNode<Node3D>("Misc/GizmoNormal");
        dMisc_GizmoOneColor = GetNode<Node3D>("Misc/GizmoOneColor");
        dMisc_Grids_Grid = GetNode<Node3D>("Misc/Grids/Grid");
        dMisc_Grids_Grid_Subdivision = GetNode<Node3D>("Misc/Grids/Grid/Subdivision");
        dMisc_Grids_GridCentered_Subdivision = GetNode<Node3D>("Misc/Grids/GridCentered/Subdivision");
        dMisc_Grids_GridCentered = GetNode<Node3D>("Misc/Grids/GridCentered");
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

    void _update_keys_just_press()
    {
        var set = (Key k) => Input.IsKeyPressed(k) ? (button_presses[k] == 0 ? 1 : button_presses[k]) : 0;
        button_presses[Key.Left] = set(Key.Left);
        button_presses[Key.Up] = set(Key.Up);
        button_presses[Key.F1] = set(Key.F1);
        button_presses[Key.Key1] = set(Key.Key1);
        button_presses[Key.Key2] = set(Key.Key2);
        button_presses[Key.Key3] = set(Key.Key3);
    }

    public override void _Process(double delta)
    {
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
            DebugDraw3D.DrawBox(box_pos, new Vector3(1, 2, 1), new Color(0, 1, 0));
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
        DebugDraw3D.Config.GeometryRenderLayers = !Input.IsKeyPressed(Key.Shift) ? 1 : 0b10010;
        dPanel.Visible = Input.IsKeyPressed(Key.Shift);
        DebugDraw2D.CustomCanvas = Input.IsKeyPressed(Key.Shift) ? dCustomCanvas : null;

        // More property toggles
        DebugDraw3D.Config.Freeze3dRender = Input.IsKeyPressed(Key.Down);
        DebugDraw3D.Config.VisibleInstanceBounds = Input.IsKeyPressed(Key.Right);

        // Some property toggles
        if (_is_key_just_pressed(Key.Left))
            DebugDraw3D.Config.UseFrustumCulling = !DebugDraw3D.Config.UseFrustumCulling;

        if (_is_key_just_pressed(Key.Up))
            DebugDraw3D.Config.ForceUseCameraFromScene = !DebugDraw3D.Config.ForceUseCameraFromScene;

        if (_is_key_just_pressed(Key.Key1))
            DebugDraw3D.DebugEnabled = !DebugDraw3D.DebugEnabled;
        if (_is_key_just_pressed(Key.Key2))
            DebugDraw2D.DebugEnabled = !DebugDraw2D.DebugEnabled;
        if (_is_key_just_pressed(Key.Key3))
            DebugDrawManager.DebugEnabled = !DebugDrawManager.DebugEnabled;


        if (Engine.IsEditorHint())
        {
            DebugDraw3D.Config.CullByDistance = DebugDraw3D.Config.ForceUseCameraFromScene ? start_culling_distance : 0.0f;
        }
        else
        {
            DebugDraw3D.Config.CullByDistance = start_culling_distance;
        }

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
        DebugDraw3D.DrawSphereHdXf(dSphereHDTransform.GlobalTransform, Colors.OrangeRed);

        // Delayed spheres
        if (time <= 0)
        {
            DebugDraw3D.DrawSphere(dSpherePosition.GlobalTransform.Origin, 2.0f, Colors.BlueViolet, 2.0f);
            DebugDraw3D.DrawSphereHd(dSpherePosition.GlobalTransform.Origin + Vector3.Forward * 4, 2.0f, Colors.CornflowerBlue, 2.0f);
            time = 2;
        }

        time -= delta;

        // Cylinders
        DebugDraw3D.DrawCylinder(dCylinder1.GlobalTransform, Colors.Crimson);
        DebugDraw3D.DrawCylinder(new Transform3D(Basis.Identity, dCylinder2.GlobalTransform.Origin).Scaled(new Vector3(1, 2, 1)), Colors.Red);

        // Boxes
        DebugDraw3D.DrawBoxXf(dBox1.GlobalTransform, Colors.MediumPurple);
        DebugDraw3D.DrawBox(dBox2.GlobalTransform.Origin, Vector3.One, Colors.RebeccaPurple);
        DebugDraw3D.DrawBoxXf(new Transform3D(new Basis(Vector3.Up, Mathf.Pi * 0.25f).Scaled(Vector3.One * 2), dBox3.GlobalTransform.Origin), Colors.RosyBrown);

        DebugDraw3D.DrawBoxXf(dBoxOutOfDistanceCulling.GlobalTransform, Colors.Red);

        DebugDraw3D.DrawAabb(new Aabb(dAABB_fixed.GlobalTransform.Origin, new Vector3(2, 1, 2)), Colors.Aqua);
        DebugDraw3D.DrawAabbAb(dAABB.GetChild<Node3D>(0).GlobalTransform.Origin, dAABB.GetChild<Node3D>(1).GlobalTransform.Origin, Colors.DeepPink);

        // Lines
        var target = dLines_Target;
        DebugDraw3D.DrawSquare(target.GlobalTransform.Origin, 0.5f, Colors.Red);

        DebugDraw3D.DrawLine(dLines_1.GlobalTransform.Origin, target.GlobalTransform.Origin, Colors.Fuchsia);
        DebugDraw3D.DrawRay(dLines_3.GlobalTransform.Origin, (target.GlobalTransform.Origin - dLines_3.GlobalTransform.Origin).Normalized(), 3.0f, Colors.Crimson);


        if (time3 <= 0)
        {
            DebugDraw3D.DrawLine(dLines_6.GlobalTransform.Origin, target.GlobalTransform.Origin, Colors.Fuchsia, 2.0f);
            time3 = 2;
        }

        time3 -= delta;

        // Lines with Arrow
        DebugDraw3D.DrawArrowLine(dLines_2.GlobalTransform.Origin, target.GlobalTransform.Origin, Colors.Blue, 0.5f, true);
        DebugDraw3D.DrawArrowRay(dLines_4.GlobalTransform.Origin, (target.GlobalTransform.Origin - dLines_4.GlobalTransform.Origin).Normalized(), 8.0f, Colors.Lavender, 0.5f, true);

        DebugDraw3D.DrawLineHitOffset(dLines_5.GlobalTransform.Origin, target.GlobalTransform.Origin, true, Mathf.Abs(Mathf.Sin(Time.GetTicksMsec() / 1000.0f)), 0.25f, Colors.Aqua);

        // Path

        // preparing data
        List<Vector3> points = new List<Vector3>();
        List<Vector3> points_below = new List<Vector3>();
        List<Vector3> points_below2 = new List<Vector3>();
        List<Vector3> points_below3 = new List<Vector3>();
        List<Vector3> lines_above = new List<Vector3>();

        foreach (var node in dLinePath.GetChildren())
        {
            if (node is Node3D c)
            {
                points.Add(c.GlobalTransform.Origin);
                points_below.Add(c.GlobalTransform.Origin + Vector3.Down);
                points_below2.Add(c.GlobalTransform.Origin + Vector3.Down * 2);
                points_below3.Add(c.GlobalTransform.Origin + Vector3.Down * 3);
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
        DebugDraw3D.DrawArrowPath(points_below.ToArray(), Colors.Gold, 0.5f);
        DebugDraw3D.DrawPoints(points_below2.ToArray(), 0.2f, Colors.DarkGreen);
        DebugDraw3D.DrawPointPath(points_below3.ToArray(), 0.25f, Colors.Blue, Colors.Tomato);

        // Misc
        DebugDraw3D.DrawCameraFrustum(dCamera, Colors.DarkOrange);

        DebugDraw3D.DrawArrow(dMisc_Arrow.GlobalTransform, Colors.YellowGreen);

        DebugDraw3D.DrawSquare(dMisc_Billboard.GlobalTransform.Origin, 0.5f, Colors.Green);

        DebugDraw3D.DrawPosition(dMisc_Position.GlobalTransform, Colors.Brown);

        DebugDraw3D.DrawGizmo(dMisc_GizmoTransform.GlobalTransform, null, true);
        DebugDraw3D.DrawGizmo(dMisc_GizmoNormal.GlobalTransform.Orthonormalized(), null, false);
        DebugDraw3D.DrawGizmo(dMisc_GizmoOneColor.GlobalTransform, Colors.Brown, true);

        Transform3D tg = dMisc_Grids_Grid.GlobalTransform;
        Vector3 tn = dMisc_Grids_Grid_Subdivision.Transform.Origin;
        DebugDraw3D.DrawGrid(tg.Origin, tg.Basis.X, tg.Basis.Z, new Vector2I((int)tn.X * 10, (int)tn.Z * 10), Colors.LightCoral, false);

        var tn1 = dMisc_Grids_GridCentered_Subdivision.Transform.Origin;
        DebugDraw3D.DrawGridXf(dMisc_Grids_GridCentered.GlobalTransform, new Vector2I((int)tn1.X * 10, (int)tn1.Z * 10));

        // 2D
        DebugDraw2D.Config.TextDefaultSize = text_groups_default_font_size;
        DebugDraw2D.Config.TextBlockOffset = text_groups_offset;
        DebugDraw2D.Config.TextBlockPosition = text_groups_position;
        DebugDraw2D.Config.TextPadding = text_groups_padding;

        DebugDraw2D.Config.TextCustomFont = custom_font;


        if (test_text)
        {
            time_text -= delta;
            _text_tests();
        }

        // Graphs
        // Enable FPSGraph if not exists
        _create_graph("FPS", true, false, DebugDrawGraph.TextFlags.Current | DebugDrawGraph.TextFlags.Avg | DebugDrawGraph.TextFlags.Max | DebugDrawGraph.TextFlags.Min, "", DebugDrawGraph.GraphSide.Bottom, Engine.IsEditorHint() ? DebugDrawGraph.GraphPosition.LeftTop : DebugDrawGraph.GraphPosition.RightTop, new Vector2I(200, 80), custom_font);
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
        DebugDraw3D.DrawBox(dLagTest.GlobalTransform.Origin, Vector3.One * 2.01f, null, true);

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

        if (time_text < 0)
        {
            DebugDraw2D.SetText("Some delayed text", "for 2.5s", -1, Colors.Black, 2.5f); // it's supposed to show text for 2.5 seconds
            time_text += 5;
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
            DebugDraw2D.SetText("Shift: change render layers", DebugDraw3D.Config.GeometryRenderLayers, 1);
            DebugDraw2D.SetText("Down: freeze render", DebugDraw3D.Config.Freeze3dRender, 2);
            DebugDraw2D.SetText("Up: use scene camera", DebugDraw3D.Config.ForceUseCameraFromScene, 3);
            DebugDraw2D.SetText("1,2,3: toggle debug", $"{DebugDraw3D.DebugEnabled}, {DebugDraw2D.DebugEnabled} 😐, {DebugDrawManager.DebugEnabled} 😏", 4);
            DebugDraw2D.SetText("Left: toggle frustum culling", DebugDraw3D.Config.UseFrustumCulling, 5);
            DebugDraw2D.SetText("Right: draw bounds for culling", DebugDraw3D.Config.VisibleInstanceBounds, 6);
        }
    }

    void _more_tests()
    {
        // Line hits render
        foreach (var node in dHitTest_RayEmitter.GetChildren())
        {
            if (node is RayCast3D ray)
            {
                DebugDraw3D.DrawLineHit(ray.GlobalTransform.Origin, ray.ToGlobal(ray.TargetPosition), ray.GetCollisionPoint(), ray.IsColliding(), 0.15f);
            }
        }

        // Delayed line render
        DebugDraw3D.DrawLine(dLagTest.GlobalTransform.Origin + Vector3.Up, dLagTest.GlobalTransform.Origin + new Vector3(0, 3, Mathf.Sin(Time.GetTicksMsec() / 50.0f)), null, 0.5f);
    }

    void _draw_array_of_boxes()
    {
        // Lots of boxes to check performance.. I guess..
        if (time2 <= 0)
        {
            for (int x = 0; x < 50; x++)
            {
                for (int y = 0; y < 50; y++)
                {
                    for (int z = 0; z < 3; z++)
                    {
                        DebugDraw3D.DrawBox(new Vector3(x, -4 - z, y), Vector3.One, null, false, 1.25f);
                    }
                }
            }
            time2 = 1.25;
        }
        time2 -= GetProcessDeltaTime();
    }


    void _graph_test()
    {
        _create_graph("fps", true, true, DebugDrawGraph.TextFlags.Current, "", DebugDrawGraph.GraphSide.Left, DebugDrawGraph.GraphPosition.RightTop);
        _create_graph("fps2", true, false, DebugDrawGraph.TextFlags.Current, "fps", DebugDrawGraph.GraphSide.Bottom, 0, new Vector2I(200, 100));

        _create_graph("Sin Wave!", false, true, DebugDrawGraph.TextFlags.Current, "fps2", DebugDrawGraph.GraphSide.Bottom);

        _create_graph("randf", false, true, DebugDrawGraph.TextFlags.Avg, "", DebugDrawGraph.GraphSide.Left, DebugDrawGraph.GraphPosition.RightBottom, new Vector2I(256, 60), custom_font);

        _create_graph("fps5", true, true, DebugDrawGraph.TextFlags.All, "randf", DebugDrawGraph.GraphSide.Top);
        _create_graph("fps6", true, true, DebugDrawGraph.TextFlags.All, "fps5", DebugDrawGraph.GraphSide.Top);
        _create_graph("fps12", true, true, DebugDrawGraph.TextFlags.All, "fps5", DebugDrawGraph.GraphSide.Left);

        _create_graph("fps7", true, false, DebugDrawGraph.TextFlags.All, "FPS", DebugDrawGraph.GraphSide.Bottom);
        _create_graph("fps8", true, true, DebugDrawGraph.TextFlags.All, "", DebugDrawGraph.GraphSide.Top, DebugDrawGraph.GraphPosition.LeftBottom);
        _create_graph("fps9", true, false, DebugDrawGraph.TextFlags.All, "fps8", DebugDrawGraph.GraphSide.Right);
        _create_graph("fps10", true, false, DebugDrawGraph.TextFlags.All, "fps8", DebugDrawGraph.GraphSide.Top);
        _create_graph("fps11", true, true, DebugDrawGraph.TextFlags.All, "fps9", DebugDrawGraph.GraphSide.Right);

        // If graphs exists, then more tests are done
        DebugDraw2D.GetGraph("Sin Wave!").DataGetter = new Callable(this, "_get_sin_wave_for_graph");
        DebugDraw2D.GetGraph("Sin Wave!").UpsideDown = false;

        DebugDraw2D.GetGraph("randf").TextSuffix = "utf8 ноль zéro";
        //DebugDraw2D.GetGraph("fps9").line_position = DebugDrawGraph.LINE_TOP
        DebugDraw2D.GetGraph("fps9").Offset = new Vector2I(0, 0);
        //DebugDraw2D.GetGraph("fps11").LlinePosition = DebugDrawGraph.LINE_BOTTOM
        DebugDraw2D.GetGraph("fps11").Offset = new Vector2I(16, 0);
        DebugDraw2D.GetGraph("fps6").Offset = new Vector2I(0, 32);
        DebugDraw2D.GetGraph("fps").Offset = new Vector2I(16, 72);
        DebugDraw2D.GetGraph("fps9").Enabled = graph_is_enabled;

        if (!Engine.IsEditorHint())
        {
            DebugDraw2D.GetGraph("fps").Corner = DebugDrawGraph.GraphPosition.LeftTop;
        }

        // Just sending random data to the graph
        DebugDraw2D.GraphUpdateData("randf", (float)random.NextDouble());
    }

    void _upd_graph_params()
    {
        DebugDraw2D.Config.GraphsBaseOffset = graph_offset;
        foreach (var g in new string[] { "FPS", "fps5", "fps8" })
        {
            var graph = DebugDraw2D.GetGraph(g) as DebugDrawFPSGraph;
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

    DebugDrawGraph _create_graph(string title, bool is_fps, bool show_title, DebugDrawGraph.TextFlags flags, string parent = "", DebugDrawGraph.GraphSide parent_side = DebugDrawGraph.GraphSide.Bottom, DebugDrawGraph.GraphPosition pos = DebugDrawGraph.GraphPosition.LeftBottom, Vector2I? size = null, Font font = null)
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
