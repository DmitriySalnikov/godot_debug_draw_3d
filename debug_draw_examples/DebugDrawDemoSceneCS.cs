using Godot;
using System;
using System.Collections.Generic;

[Tool]
public class DebugDrawDemoSceneCS : Spatial
{
    [Export] Font custom_font = null;
    [Export] bool zylann_example = false;
    [Export] bool test_graphs = false;
    [Export] bool more_test_cases = false;

    Random random = new Random();

    float time = 0;
    float time2 = 0;
    float time3 = 0;
    bool toogle_frustum_key = false;

    public override async void _Ready()
    {
        await new SignalAwaiter(GetTree(), "idle_frame", this);

        DebugDrawCS.DrawLine3d(Vector3.Zero, Vector3.One, null, 10);
        DebugDrawCS.DrawLine3d(Vector3.Zero, Vector3.One * 1, null, 1);
        DebugDrawCS.DrawLine3d(Vector3.Zero, Vector3.One * 2, null, 1);
        DebugDrawCS.DrawLine3d(Vector3.Zero, Vector3.One * 3, null, 1);
        DebugDrawCS.DrawLine3d(Vector3.Zero, Vector3.One * 4, null, 1);
    }

    protected override void Dispose(bool disposing)
    {
        custom_font = null;
        base.Dispose(disposing);
    }

    public override void _Input(InputEvent @event)
    {
        if (@event is InputEventKey key)
        {
            if (key.IsPressed())
            {
                if (key.Scancode == (int)KeyList.F1)
                {
                    zylann_example = !zylann_example;
                }
            }
        }
    }

    public override void _Process(float delta)
    {
        //Zylann's example :D
        if (zylann_example)
        {
            DebugDrawCS.ClearGraphs();
            var _time = OS.GetTicksMsec() / 1000.0;
            var box_pos = new Vector3(0, Mathf.Sin((float)_time * 4), 0);
            var line_begin = new Vector3(-1, Mathf.Sin((float)_time * 4), 0);
            var line_end = new Vector3(1, Mathf.Cos((float)_time * 4), 0);

            DebugDrawCS.DrawAabbAb(box_pos, new Vector3(1, 2, 1), new Color(0, 1, 0), 0);// Box need to be NOT centered
            DebugDrawCS.DrawLine3d(line_begin, line_end, new Color(1, 1, 0));
            DebugDrawCS.SetText("Time", _time);
            DebugDrawCS.SetText("Frames drawn", Engine.GetFramesDrawn());
            DebugDrawCS.SetText("FPS", Engine.GetFramesPerSecond());
            DebugDrawCS.SetText("delta", delta);
            return;
        }

        DebugDrawCS.Freeze3dRender = Input.IsKeyPressed((int)KeyList.Enter);
        DebugDrawCS.ForceUseCameraFromScene = Input.IsKeyPressed((int)KeyList.Up);
        DebugDrawCS.DebugEnabled = !Input.IsKeyPressed((int)KeyList.Down);
        DebugDrawCS.DrawInstanceBounds = Input.IsKeyPressed((int)KeyList.Right);
        if (toogle_frustum_key && !Input.IsKeyPressed((int)KeyList.Left))
            DebugDrawCS.UseFrustumCulling = !DebugDrawCS.UseFrustumCulling;
        toogle_frustum_key = Input.IsKeyPressed((int)KeyList.Left);

        // Enable FPSGraph
        _create_graph("FPS", true, false, Engine.EditorHint ? DebugDrawCS.BlockPosition_LeftTop : DebugDrawCS.BlockPosition_RightTop, DebugDrawCS.GraphTextFlags_Current | DebugDrawCS.GraphTextFlags_Avarage | DebugDrawCS.GraphTextFlags_Max | DebugDrawCS.GraphTextFlags_Min, new Vector2(200, 80), custom_font);

        if (test_graphs)
            _graph_test();

        // Zones
        var col = Colors.Black;
        foreach (var z in GetNode<Spatial>("Zones").GetChildren())
            DebugDrawCS.DrawBoxXf(((Spatial)z).GlobalTransform, col);

        // Spheres
        DebugDrawCS.DrawSphereXf(GetNode<Spatial>("SphereTransform").GlobalTransform, Colors.Crimson);
        DebugDrawCS.DrawSphereHdXf(GetNode<Spatial>("SphereHDTransform").GlobalTransform, Colors.OrangeRed);

        if (time <= 0)
        {
            DebugDrawCS.DrawSphere(GetNode<Spatial>("SpherePosition").GlobalTransform.origin, 2, Colors.BlueViolet, 2);
            DebugDrawCS.DrawSphereHd(GetNode<Spatial>("SpherePosition").GlobalTransform.origin + Vector3.Forward * 4, 2, Colors.Cornflower, 2);
            time = 2;
        }
        time -= delta;

        // Cylinders
        DebugDrawCS.DrawCylinderXf(GetNode<Spatial>("Cylinder1").GlobalTransform, Colors.Crimson);
        DebugDrawCS.DrawCylinder(GetNode<Spatial>("Cylinder2").GlobalTransform.origin, Quat.Identity, 1, 1, Colors.Red);

        // Boxes
        DebugDrawCS.DrawBoxXf(GetNode<Spatial>("Box1").GlobalTransform, Colors.Purple);
        DebugDrawCS.DrawBox(GetNode<Spatial>("Box2").GlobalTransform.origin, Quat.Identity, Vector3.One, Colors.RebeccaPurple);
        DebugDrawCS.DrawBox(GetNode<Spatial>("Box3").GlobalTransform.origin, new Quat(Vector3.Up, Mathf.Pi * 0.25f), Vector3.One * 2, Colors.RosyBrown);

        var r = GetNode<Spatial>("AABB");
        DebugDrawCS.DrawAabbAb(r.GetChild<Spatial>(0).GlobalTransform.origin, r.GetChild<Spatial>(1).GlobalTransform.origin, Colors.DeepPink);
        DebugDrawCS.DrawAabb(new AABB(GetNode<Spatial>("AABB_fixed").GlobalTransform.origin, new Vector3(2, 1, 2)), Colors.Aqua);

        // Lines
        var target = GetNode<Spatial>("Lines/Target");
        DebugDrawCS.DrawBillboardSquare(target.GlobalTransform.origin, 0.5f, Colors.Red);

        // Normal
        DebugDrawCS.DrawLine3d(GetNode<Spatial>("Lines/1").GlobalTransform.origin, target.GlobalTransform.origin, Colors.Fuchsia);
        DebugDrawCS.DrawRay3d(GetNode<Spatial>("Lines/3").GlobalTransform.origin, (target.GlobalTransform.origin - GetNode<Spatial>("Lines/3").GlobalTransform.origin).Normalized(), 3, Colors.Crimson);


        if (time3 <= 0)
        {
            DebugDrawCS.DrawLine3d(GetNode<Spatial>("Lines/6").GlobalTransform.origin, target.GlobalTransform.origin, Colors.Fuchsia, 2);
            time3 = 2;
        }
        time3 -= delta;

        // Arrow
        DebugDrawCS.DrawArrowLine3d(GetNode<Spatial>("Lines/2").GlobalTransform.origin, target.GlobalTransform.origin, Colors.Blue);
        DebugDrawCS.DrawArrowRay3d(GetNode<Spatial>("Lines/4").GlobalTransform.origin, (target.GlobalTransform.origin - GetNode<Spatial>("Lines/4").GlobalTransform.origin).Normalized(), 8, Colors.Lavender);

        // Path
        List<Vector3> points = new List<Vector3>();
        List<Vector3> points_below = new List<Vector3>();
        List<Vector3> lines_above = new List<Vector3>();
        foreach (var ch in GetNode<Spatial>("LinePath").GetChildren())
        {
            Spatial c = (Spatial)ch;

            points.Add(c.GlobalTransform.origin);
            points_below.Add(c.GlobalTransform.origin + Vector3.Down);
        }

        for (int x = 0; x<points.Count-1; x++)
        {
            lines_above.Add(points[x] + Vector3.Up);
            lines_above.Add(points[x+1] + Vector3.Up);
        }

        DebugDrawCS.DrawLines3d(lines_above.ToArray());
        DebugDrawCS.DrawLinePath3d(points.ToArray(), Colors.Beige);
        DebugDrawCS.DrawArrowPath3d(points_below.ToArray(), Colors.Gold, 0.75f);
        DebugDrawCS.DrawLine3dHitOffset(GetNode<Spatial>("Lines/5").GlobalTransform.origin, target.GlobalTransform.origin, true, Mathf.Abs(Mathf.Sin(OS.GetTicksMsec() / 1000.0f)), 0.25f, Colors.Aqua);

        // Misc
        DebugDrawCS.DrawCameraFrustum(GetNode<Camera>("Camera"), Colors.DarkOrange);

        DebugDrawCS.DrawBillboardSquare(GetNode<Spatial>("Misc/Billboard").GlobalTransform.origin, 0.5f, Colors.Green);

        DebugDrawCS.DrawPosition3dXf(GetNode<Spatial>("Misc/Position").GlobalTransform, Colors.Brown);
        
        DebugDrawCS.DrawGizmo3d(GetNode<Spatial>("Misc/Gizmo").GlobalTransform.origin, GetNode<Spatial>("Misc/Gizmo").GlobalTransform.basis.Quat(), Vector3.One);
        DebugDrawCS.DrawGizmo3dXf(GetNode<Spatial>("Misc/GizmoTransform").GlobalTransform, true);

        // Text
        DebugDrawCS.TextCustomFont = custom_font;
        DebugDrawCS.SetText("FPS", FormattableString.Invariant($"{Engine.GetFramesPerSecond():F2}"), 0, Colors.Gold);

        DebugDrawCS.BeginTextGroup("-- First Group --", 2, Colors.ForestGreen);
        DebugDrawCS.SetText("Simple text");
        DebugDrawCS.SetText("Text", "Value", 0, Colors.Aquamarine);
        DebugDrawCS.SetText("Text out of order", null, -1, Colors.Silver);
        DebugDrawCS.BeginTextGroup("-- Second Group --", 1, Colors.Beige);
        DebugDrawCS.SetText("Rendered frames", Engine.GetFramesDrawn());
        DebugDrawCS.EndTextGroup();

        DebugDrawCS.BeginTextGroup("-- Stats --", 3, Colors.Wheat);

        var RenderCount = DebugDrawCS.GetRenderedPrimitivesCount();
        if (RenderCount.Count > 0)
        {
            DebugDrawCS.SetText("Total", RenderCount["total"], 0);
            DebugDrawCS.SetText("Instances", RenderCount["instances"], 1);
            DebugDrawCS.SetText("Wireframes", RenderCount["wireframes"], 2);
            DebugDrawCS.SetText("Total Visible", RenderCount["total_visible"], 3);
            DebugDrawCS.SetText("Visible Instances", RenderCount["visible_instances"], 4);
            DebugDrawCS.SetText("Visible Wireframes", RenderCount["visible_wireframes"], 5);
        }
        DebugDrawCS.EndTextGroup();

        // Lag Test
        GetNode<Spatial>("LagTest").Translation = ((Transform)GetNode<AnimationPlayer>("LagTest/RESET").GetAnimation("RESET").TrackGetKeyValue(0, 0)).origin + new Vector3(Mathf.Sin(OS.GetTicksMsec() / 100.0f) * 2.5f, 0, 0);
        DebugDrawCS.DrawBox(GetNode<Spatial>("LagTest").GlobalTransform.origin, Quat.Identity, Vector3.One * 2.01f);

        if (more_test_cases)
        {
            foreach (var _ray in GetNode("HitTest/RayEmitter").GetChildren())
                if (_ray is RayCast ray)
                    ray.SetPhysicsProcessInternal(true);
            _more_tests();
        }

        else
        {
            foreach (var _ray in GetNode("HitTest/RayEmitter").GetChildren())
                if (_ray is RayCast ray)
                    ray.SetPhysicsProcessInternal(false);
        }

    }

    void _more_tests()
    {
        foreach (var _ray in GetNode("HitTest/RayEmitter").GetChildren())
        {
            if (_ray is RayCast ray)
            {
                DebugDrawCS.DrawLine3dHit(ray.GlobalTransform.origin, ray.GlobalTransform.Translated(ray.CastTo).origin, ray.GetCollisionPoint(), ray.IsColliding(), 0.15f);
            }
        }

        if (time2 <= 0)
        {
            for (int x = 0; x< 50; x++)
                for (int y = 0; y< 50; y++)
                    for (int z = 0; z< 3; z++)
                        DebugDrawCS.DrawBox(new Vector3(x, z, y), Quat.Identity, Vector3.One, null, false, 1.25f);
            time2 = 1.25f;
        }
        time2 -= GetProcessDeltaTime();

        // Delayed line render
        DebugDrawCS.DrawLine3d(GetNode<Spatial>("LagTest").GlobalTransform.origin + Vector3.Up, GetNode<Spatial>("LagTest").GlobalTransform.origin +new Vector3(0, 3, Mathf.Sin(OS.GetTicksMsec() / 50.0f)), null, 0.5f);
    }

    void _graph_test()
    {
        _create_graph("fps", true, true, DebugDrawCS.BlockPosition_RightTop, DebugDrawCS.GraphTextFlags_Current);
        _create_graph("fps2", true, false, DebugDrawCS.BlockPosition_RightTop, DebugDrawCS.GraphTextFlags_Current);
        _create_graph("fps3", true, true, DebugDrawCS.BlockPosition_RightTop, DebugDrawCS.GraphTextFlags_Current);

        _create_graph("randf", false, true, DebugDrawCS.BlockPosition_RightBottom, DebugDrawCS.GraphTextFlags_Avarage, new Vector2(256, 60), custom_font);
        _create_graph("fps5", true, false, DebugDrawCS.BlockPosition_RightBottom, DebugDrawCS.GraphTextFlags_All);
        _create_graph("fps6", true, true, DebugDrawCS.BlockPosition_RightBottom, DebugDrawCS.GraphTextFlags_All);

        _create_graph("fps7", true, false, DebugDrawCS.BlockPosition_LeftTop, DebugDrawCS.GraphTextFlags_All);
        _create_graph("fps8", true, true, DebugDrawCS.BlockPosition_LeftBottom, DebugDrawCS.GraphTextFlags_All);
        _create_graph("fps9", true, true, DebugDrawCS.BlockPosition_LeftBottom, DebugDrawCS.GraphTextFlags_All);
        _create_graph("fps10", true, false, DebugDrawCS.BlockPosition_LeftBottom, DebugDrawCS.GraphTextFlags_All);

        if (DebugDrawCS.GetGraphConfig("fps5") != null)
        {
            DebugDrawCS.GetGraphConfig("randf").Set("text_suffix", "utf8 ноль zéro");
            if (Engine.EditorHint)
            {
                DebugDrawCS.GetGraphConfig("fps5").Set("offset", new Vector2(0, -30));
                DebugDrawCS.GetGraphConfig("fps8").Set("offset", new Vector2(280, -60));
                DebugDrawCS.GetGraphConfig("fps9").Set("offset", new Vector2(0, -75));
            }
            else
            {
                DebugDrawCS.GetGraphConfig("fps5").Set("offset", new Vector2(0, 0));
                DebugDrawCS.GetGraphConfig("fps8").Set("offset", new Vector2(280, 0));
                DebugDrawCS.GetGraphConfig("fps9").Set("offset", new Vector2(0, -75));
            }
        }

        DebugDrawCS.GraphUpdateData("randf", (float)random.NextDouble());
    }

    void _create_graph(string title, bool is_fps, bool show_title, int pos, int flags, Vector2? size = null, Font font = null)
    {
        var graph = DebugDrawCS.GetGraphConfig(title);
        if (graph == null)
        {
            if (is_fps)
                graph = DebugDrawCS.CreateFpsGraph(title);
            else
                graph = DebugDrawCS.CreateGraph(title);

            if (graph != null)
            {
                graph.Set("size", size.HasValue ? size : new Vector2(256, 60));
                graph.Set("buffer_size", 50);
                graph.Set("position", pos);
                graph.Set("show_title", show_title);
                graph.Set("show_text_flags", flags);
                graph.Set("custom_font", font);
            }
        }
    }

}
