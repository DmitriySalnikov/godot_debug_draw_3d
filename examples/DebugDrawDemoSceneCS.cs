using Godot;
using System;
using System.Collections.Generic;

[Tool]
public class DebugDrawDemoSceneCS : Spatial
{
    // Godot 3.x cannot properly release resources
    [Export] Font custom_font = null;
    [Export] bool zylann_example = false;
    [Export] bool show_hints = true;
    [Export] bool test_graphs = false;
    [Export] bool more_test_cases = false;
    [Export] bool draw_array_of_boxes = false;

    Random random = new Random();

    float time = 0;
    float time2 = 0;
    float time3 = 0;

    public override async void _Ready()
    {
        await new SignalAwaiter(GetTree(), "idle_frame", this);

        if (!IsInsideTree())
            return;
    }

    public override void _Input(InputEvent @event)
    {
        if (@event is InputEventKey key)
        {
            if (key.IsPressed())
            {
                if (key.Scancode == (int)KeyList.F1)
                    zylann_example = !zylann_example;
                if (key.Scancode == (int)KeyList.Left)
                    DebugDrawCS.UseFrustumCulling = !DebugDrawCS.UseFrustumCulling;
            }
        }
    }

    public override void _Process(float delta)
    {
        //Zylann's example :D
        if (zylann_example)
        {
            DebugDrawCS.ClearGraphs();
            var _time = OS.GetTicksMsec() / 1000f;
            var box_pos = new Vector3(0, Mathf.Sin(_time * 4), 0);
            var line_begin = new Vector3(-1, Mathf.Sin(_time * 4), 0);
            var line_end = new Vector3(1, Mathf.Cos(_time * 4), 0);

            DebugDrawCS.DrawBox(box_pos, new Vector3(1, 2, 1), new Color(0, 1, 0));
            DebugDrawCS.DrawLine(line_begin, line_end, new Color(1, 1, 0));
            DebugDrawCS.SetText("Time", _time);
            DebugDrawCS.SetText("Frames drawn", Engine.GetFramesDrawn());
            DebugDrawCS.SetText("FPS", Engine.GetFramesPerSecond());
            DebugDrawCS.SetText("delta", delta);
            return;
        }

        // Testing the rendering layers by showing the image from the second camera inside the 2D panel
        DebugDrawCS.GeometryRenderLayers = !Input.IsKeyPressed((int)KeyList.Shift) ? 1 : 0b10010;
        GetNode<Control>("Panel").Visible = Input.IsKeyPressed((int)KeyList.Shift);

        // More property toggles
        DebugDrawCS.Freeze3dRender = Input.IsKeyPressed((int)KeyList.Enter);
        DebugDrawCS.ForceUseCameraFromScene = Input.IsKeyPressed((int)KeyList.Up);
        DebugDrawCS.DebugEnabled = !Input.IsKeyPressed((int)KeyList.Down);
        DebugDrawCS.VisibleInstanceBounds = Input.IsKeyPressed((int)KeyList.Right);

        // Enable FPSGraph if not exists
        var g = _create_graph("FPS", true, false, Engine.EditorHint ? DebugDrawCS.BlockPosition_LeftTop : DebugDrawCS.BlockPosition_RightTop, DebugDrawCS.GraphTextFlags_Current | DebugDrawCS.GraphTextFlags_Avarage | DebugDrawCS.GraphTextFlags_Max | DebugDrawCS.GraphTextFlags_Min, new Vector2(200, 80), custom_font);
        if (g != null)
            g.BufferSize = 300;

        // Adding more graphs
        if (test_graphs)
            _graph_test();

        // Zones with black borders
        var col = Colors.Black;
        foreach (var z in GetNode<Spatial>("Zones").GetChildren())
            DebugDrawCS.DrawBoxXf(((Spatial)z).GlobalTransform, col);

        // Spheres
        DebugDrawCS.DrawSphereXf(GetNode<Spatial>("SphereTransform").GlobalTransform, Colors.Crimson);
        DebugDrawCS.DrawSphereHdXf(GetNode<Spatial>("SphereHDTransform").GlobalTransform, Colors.OrangeRed);

        // Delayed spheres
        if (time <= 0)
        {
            DebugDrawCS.DrawSphere(GetNode<Spatial>("SpherePosition").GlobalTransform.origin, 2, Colors.BlueViolet, 2);
            DebugDrawCS.DrawSphereHd(GetNode<Spatial>("SpherePosition").GlobalTransform.origin + Vector3.Forward * 4, 2, Colors.Cornflower, 2);
            time = 2;
        }
        time -= delta;

        // Cylinders
        DebugDrawCS.DrawCylinder(GetNode<Spatial>("Cylinder1").GlobalTransform, Colors.Crimson);
        DebugDrawCS.DrawCylinder(new Transform(Basis.Identity, GetNode<Spatial>("Cylinder2").GlobalTransform.origin).Scaled(new Vector3(1, 2, 1)), Colors.Red);

        // Boxes
        DebugDrawCS.DrawBoxXf(GetNode<Spatial>("Box1").GlobalTransform, Colors.Purple);
        DebugDrawCS.DrawBox(GetNode<Spatial>("Box2").GlobalTransform.origin, Vector3.One, Colors.RebeccaPurple);
        DebugDrawCS.DrawBoxXf(new Transform(new Basis(Vector3.Up, Mathf.Pi * 0.25f).Scaled(Vector3.One * 2), GetNode<Spatial>("Box3").GlobalTransform.origin), Colors.RosyBrown);

        var r = GetNode<Spatial>("AABB");
        DebugDrawCS.DrawAabbAb(r.GetChild<Spatial>(0).GlobalTransform.origin, r.GetChild<Spatial>(1).GlobalTransform.origin, Colors.DeepPink);
        DebugDrawCS.DrawAabb(new AABB(GetNode<Spatial>("AABB_fixed").GlobalTransform.origin, new Vector3(2, 1, 2)), Colors.Aqua);

        // Lines
        var target = GetNode<Spatial>("Lines/Target");
        DebugDrawCS.DrawSquare(target.GlobalTransform.origin, 0.5f, Colors.Red);

        DebugDrawCS.DrawLine(GetNode<Spatial>("Lines/1").GlobalTransform.origin, target.GlobalTransform.origin, Colors.Fuchsia);
        DebugDrawCS.DrawRay(GetNode<Spatial>("Lines/3").GlobalTransform.origin, (target.GlobalTransform.origin - GetNode<Spatial>("Lines/3").GlobalTransform.origin).Normalized(), 3, Colors.Crimson);

        if (time3 <= 0)
        {
            DebugDrawCS.DrawLine(GetNode<Spatial>("Lines/6").GlobalTransform.origin, target.GlobalTransform.origin, Colors.Fuchsia, 2);
            time3 = 2;
        }
        time3 -= delta;

        // Lines with Arrow
        DebugDrawCS.DrawArrowLine(GetNode<Spatial>("Lines/2").GlobalTransform.origin, target.GlobalTransform.origin, Colors.Blue, 0.5f, true);
        DebugDrawCS.DrawArrowRay(GetNode<Spatial>("Lines/4").GlobalTransform.origin, (target.GlobalTransform.origin - GetNode<Spatial>("Lines/4").GlobalTransform.origin).Normalized(), 8, Colors.Lavender, 0.5f, true);
        
        DebugDrawCS.DrawLineHitOffset(GetNode<Spatial>("Lines/5").GlobalTransform.origin, target.GlobalTransform.origin, true, Mathf.Abs(Mathf.Sin(OS.GetTicksMsec() / 1000.0f)), 0.25f, Colors.Aqua);

        // Path

        /// preparing data
        List<Vector3> points = new List<Vector3>();
        List<Vector3> points_below = new List<Vector3>();
        List<Vector3> points_below2 = new List<Vector3>();
        List<Vector3> points_below3 = new List<Vector3>();
        List<Vector3> lines_above = new List<Vector3>();
        foreach (var ch in GetNode<Spatial>("LinePath").GetChildren())
        {
            Spatial c = (Spatial)ch;

            points.Add(c.GlobalTransform.origin);
            points_below.Add(c.GlobalTransform.origin + Vector3.Down);
            points_below2.Add(c.GlobalTransform.origin + Vector3.Down * 2);
            points_below3.Add(c.GlobalTransform.origin + Vector3.Down * 3);
        }

        for (int x = 0; x<points.Count-1; x++)
        {
            lines_above.Add(points[x] + Vector3.Up);
            lines_above.Add(points[x+1] + Vector3.Up);
        }

        /// drawing lines
        DebugDrawCS.DrawLines(lines_above.ToArray());
        DebugDrawCS.DrawLinePath(points.ToArray(), Colors.Beige);
        DebugDrawCS.DrawArrowPath(points_below.ToArray(), Colors.Gold, 0.5f);
        DebugDrawCS.DrawPoints(points_below2.ToArray(), 0.2f, Colors.DarkGreen);
        DebugDrawCS.DrawPointPath(points_below3.ToArray(), 0.25f, Colors.Blue, Colors.Tomato);

        // Misc
        DebugDrawCS.DrawCameraFrustum(GetNode<Camera>("Camera"), Colors.DarkOrange);

        DebugDrawCS.DrawArrow(GetNode<Spatial>("Misc/Arrow").GlobalTransform, Colors.YellowGreen);

        DebugDrawCS.DrawSquare(GetNode<Spatial>("Misc/Billboard").GlobalTransform.origin, 0.5f, Colors.Green);

        DebugDrawCS.DrawPosition(GetNode<Spatial>("Misc/Position").GlobalTransform, Colors.Brown);

        DebugDrawCS.DrawGizmo(GetNode<Spatial>("Misc/GizmoTransform").GlobalTransform, null, true);
        DebugDrawCS.DrawGizmo(GetNode<Spatial>("Misc/GizmoNormal").GlobalTransform.Orthonormalized(), null, false);
        DebugDrawCS.DrawGizmo(GetNode<Spatial>("Misc/GizmoOneColor").GlobalTransform.Orthonormalized(), Colors.Brown, true);

        var tg = GetNode<Spatial>("Misc/Grids/Grid").GlobalTransform;
        var tn = GetNode<Spatial>("Misc/Grids/Grid/Subdivision").Transform.origin;
        DebugDrawCS.DrawGrid(tg.origin, tg.basis.x, tg.basis.z, new Vector2(tn.x*10, tn.z*10), Colors.LightCoral, false);

        var tn1 = GetNode<Spatial>("Misc/Grids/GridCentered/Subdivision").Transform.origin;
        DebugDrawCS.DrawGridXf(GetNode<Spatial>("Misc/Grids/GridCentered").GlobalTransform, new Vector2(tn1.x*10, tn1.z*10));

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

        var RenderCount = DebugDrawCS.GetRenderStats();
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

        if (show_hints)
        {
            DebugDrawCS.BeginTextGroup("controls", 1024, Colors.White, false);
            DebugDrawCS.SetText("Shift: change render layers", DebugDrawCS.GeometryRenderLayers, 1);
            DebugDrawCS.SetText("Enter: freeze render", DebugDrawCS.Freeze3dRender, 2);
            DebugDrawCS.SetText("Up: use scene camera", DebugDrawCS.ForceUseCameraFromScene, 3);
            DebugDrawCS.SetText("Down: toggle debug", DebugDrawCS.DebugEnabled, 4);
            DebugDrawCS.SetText("Left: toggle frustum culling", DebugDrawCS.UseFrustumCulling, 5);
            DebugDrawCS.SetText("Right: draw bounds for culling", DebugDrawCS.VisibleInstanceBounds, 6);
        }

        // Lag Test
        GetNode<Spatial>("LagTest").Translation = ((Transform)GetNode<AnimationPlayer>("LagTest/RESET").GetAnimation("RESET").TrackGetKeyValue(0, 0)).origin + new Vector3(Mathf.Sin(OS.GetTicksMsec() / 100.0f) * 2.5f, 0, 0);
        DebugDrawCS.DrawBox(GetNode<Spatial>("LagTest").GlobalTransform.origin, Vector3.One * 2.01f, null, true);

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

        if (draw_array_of_boxes)
        {
            _draw_array_of_boxes();
        }
    }

    void _more_tests()
    {
        // Line hits render
        foreach (var _ray in GetNode("HitTest/RayEmitter").GetChildren())
        {
            if (_ray is RayCast ray)
            {
                DebugDrawCS.DrawLineHit(ray.GlobalTransform.origin, ray.GlobalTransform.Translated(ray.CastTo).origin, ray.GetCollisionPoint(), ray.IsColliding(), 0.15f);
            }
        }

        // Delayed line render
        DebugDrawCS.DrawLine(GetNode<Spatial>("LagTest").GlobalTransform.origin + Vector3.Up, GetNode<Spatial>("LagTest").GlobalTransform.origin +new Vector3(0, 3, Mathf.Sin(OS.GetTicksMsec() / 50.0f)), null, 0.5f);
    }

    void _draw_array_of_boxes()
    {
        // Lots of boxes to check performance.. I guess..
        if (time2 <= 0)
        {
            for (int x = 0; x< 50; x++)
                for (int y = 0; y< 50; y++)
                    for (int z = 0; z< 3; z++)
                        DebugDrawCS.DrawBox(new Vector3(x, -4-z, y), Vector3.One, null, false, 1.25f);
            time2 = 1.25f;
        }
        time2 -= GetProcessDeltaTime();
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

        // If graphs exists, then more tests are done
        if (DebugDrawCS.GetGraphConfig("fps5") != null)
        {
            DebugDrawCS.GetGraphConfig("randf").TextSuffix = "utf8 ноль zéro";
            DebugDrawCS.GetGraphConfig("fps9").CenteredGraphLine = false;

            if (Engine.EditorHint)
            {
                DebugDrawCS.GetGraphConfig("fps5").Offset = new Vector2(0, -30);
                DebugDrawCS.GetGraphConfig("fps8").Offset = new Vector2(280, -60);
                DebugDrawCS.GetGraphConfig("fps9").Offset =new Vector2(0, -75);
            }
            else
            {
                DebugDrawCS.GetGraphConfig("fps5").Offset =new Vector2(0, 0);
                DebugDrawCS.GetGraphConfig("fps8").Offset = new Vector2(280, 0);
                DebugDrawCS.GetGraphConfig("fps9").Offset = new Vector2(0, -75);
            }
        }

        // Just sending random data to the graph
        DebugDrawCS.GraphUpdateData("randf", (float)random.NextDouble());
    }

    DebugDrawCS.GraphParameters _create_graph(string title, bool is_fps, bool show_title, int pos, int flags, Vector2? size = null, Font font = null)
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
                graph.Size = size.HasValue ? size.Value : new Vector2(256, 60);
                graph.BufferSize =  50;
                graph.Position= pos;
                graph.ShowTitle= show_title;
                graph.ShowTextFlags= flags;
                graph.CustomFont =  font;
            }
        }

        return graph;
    }

}
