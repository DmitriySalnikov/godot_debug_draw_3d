using Godot;
using Godot.Collections;

[Tool]
public class DebugDrawCS : Node
{
	static Color empty_color = new Color(0,0,0,0);
	static Node debug_draw_3d = null;


	/// Constants

	public static int BlockPosition_LeftTop { get; private set; } = 0;
	public static int BlockPosition_RightTop { get; private set; } = 0;
	public static int BlockPosition_LeftBottom { get; private set; } = 0;
	public static int BlockPosition_RightBottom { get; private set; } = 0;
	public static int GraphTextFlags_None { get; private set; } = 0;
	public static int GraphTextFlags_Current { get; private set; } = 0;
	public static int GraphTextFlags_Avarage { get; private set; } = 0;
	public static int GraphTextFlags_Max { get; private set; } = 0;
	public static int GraphTextFlags_Min { get; private set; } = 0;
	public static int GraphTextFlags_All { get; private set; } = 0;


	/// Parameters

	/// <summary>
	/// Recall all calls from DebugDraw3D instance to its singleton if needed
	/// </summary>
	public static bool RecallToSingleton { get => (bool)debug_draw_3d?.Get("recall_to_singleton"); set => debug_draw_3d?.Set("recall_to_singleton", value); }

	/// <summary>
	/// Enable or disable all debug draw.
	/// </summary>
	public static bool DebugEnabled { get => (bool)debug_draw_3d?.Get("debug_enabled"); set => debug_draw_3d?.Set("debug_enabled", value); }

	/// <summary>
	/// Debug for debug...
	/// </summary>
	public static bool Freeze3dRender { get => (bool)debug_draw_3d?.Get("freeze_3d_render"); set => debug_draw_3d?.Set("freeze_3d_render", value); }

	/// <summary>
	/// Debug for debug...
	/// </summary>
	public static bool DrawInstanceBounds { get => (bool)debug_draw_3d?.Get("draw_instance_bounds"); set => debug_draw_3d?.Set("draw_instance_bounds", value); }

	/// <summary>
	/// Geometry culling based on camera frustum.
	/// It is not recommended to use with the current implementation.
	/// </summary>
	public static bool UseFrustumCulling { get => (bool)debug_draw_3d?.Get("use_frustum_culling"); set => debug_draw_3d?.Set("use_frustum_culling", value); }

	/// <summary>
	/// Force use camera placed on edited scene. Usable for editor.
	/// </summary>
	public static bool ForceUseCameraFromScene { get => (bool)debug_draw_3d?.Get("force_use_camera_from_scene"); set => debug_draw_3d?.Set("force_use_camera_from_scene", value); }

	/// <summary>
	/// Base offset for all graphs
	/// </summary>
	public static Vector2 GraphsBaseOffset { get => (Vector2)debug_draw_3d?.Get("graphs_base_offset"); set => debug_draw_3d?.Set("graphs_base_offset", value); }

	/// <summary>
	/// Layers on which the geometry will be displayed
	/// </summary>
	public static int GeometryRenderLayers { get => (int)debug_draw_3d?.Get("geometry_render_layers"); set => debug_draw_3d?.Set("geometry_render_layers", value); }

	/// <summary>
	/// Position of text block
	/// </summary>
	public static int TextBlockPosition { get => (int)debug_draw_3d?.Get("text_block_position"); set => debug_draw_3d?.Set("text_block_position", value); }

	/// <summary>
	/// Offset from the corner selected in 'text_block_position'
	/// </summary>
	public static Vector2 TextBlockOffset { get => (Vector2)debug_draw_3d?.Get("text_block_offset"); set => debug_draw_3d?.Set("text_block_offset", value); }

	/// <summary>
	/// Text padding for each line
	/// </summary>
	public static Vector2 TextPadding { get => (Vector2)debug_draw_3d?.Get("text_padding"); set => debug_draw_3d?.Set("text_padding", value); }

	/// <summary>
	/// How long HUD text lines remain shown after being invoked.
	/// </summary>
	public static float TextDefaultDuration { get => (float)debug_draw_3d?.Get("text_default_duration"); set => debug_draw_3d?.Set("text_default_duration", value); }

	/// <summary>
	/// Color of the text drawn as HUD
	/// </summary>
	public static Color TextForegroundColor { get => (Color)debug_draw_3d?.Get("text_foreground_color"); set => debug_draw_3d?.Set("text_foreground_color", value); }

	/// <summary>
	/// Background color of the text drawn as HUD
	/// </summary>
	public static Color TextBackgroundColor { get => (Color)debug_draw_3d?.Get("text_background_color"); set => debug_draw_3d?.Set("text_background_color", value); }

	/// <summary>
	/// Custom Font
	/// </summary>
	public static Font TextCustomFont { get => (Font)debug_draw_3d?.Get("text_custom_font"); set => debug_draw_3d?.Set("text_custom_font", value); }

	/// <summary>
	/// Color of line with hit
	/// </summary>
	public static Color LineHitColor { get => (Color)debug_draw_3d?.Get("line_hit_color"); set => debug_draw_3d?.Set("line_hit_color", value); }

	/// <summary>
	/// Color of line after hit
	/// </summary>
	public static Color LineAfterHitColor { get => (Color)debug_draw_3d?.Get("line_after_hit_color"); set => debug_draw_3d?.Set("line_after_hit_color", value); }

	/// <summary>
	/// Custom 'Viewport' to use for frustum culling.
	/// </summary>
	public static Viewport CustomViewport { get => (Viewport)debug_draw_3d?.Get("custom_viewport"); set => debug_draw_3d?.Set("custom_viewport", value); }

	/// <summary>
	/// Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
	/// </summary>
	public static CanvasItem CustomCanvas { get => (CanvasItem)debug_draw_3d?.Get("custom_canvas"); set => debug_draw_3d?.Set("custom_canvas", value); }


	/// Init
	public DebugDrawCS(){
		var f = new File();
		if (f.FileExists("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns")){
			debug_draw_3d = ResourceLoader.Load<NativeScript>("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns").New() as Node;

			BlockPosition_LeftTop = (int)debug_draw_3d.Get("BlockPosition_LeftTop");
			BlockPosition_RightTop = (int)debug_draw_3d.Get("BlockPosition_RightTop");
			BlockPosition_LeftBottom = (int)debug_draw_3d.Get("BlockPosition_LeftBottom");
			BlockPosition_RightBottom = (int)debug_draw_3d.Get("BlockPosition_RightBottom");
			GraphTextFlags_None = (int)debug_draw_3d.Get("GraphTextFlags_None");
			GraphTextFlags_Current = (int)debug_draw_3d.Get("GraphTextFlags_Current");
			GraphTextFlags_Avarage = (int)debug_draw_3d.Get("GraphTextFlags_Avarage");
			GraphTextFlags_Max = (int)debug_draw_3d.Get("GraphTextFlags_Max");
			GraphTextFlags_Min = (int)debug_draw_3d.Get("GraphTextFlags_Min");
			GraphTextFlags_All = (int)debug_draw_3d.Get("GraphTextFlags_All");
		}
	}

	public override void _EnterTree(){
		if (!Engine.EditorHint){
			if(debug_draw_3d.Call("get_singleton") == null)
				AddChild(debug_draw_3d);
		}
	}

	protected override void Dispose(bool disposing){
		debug_draw_3d?.QueueFree();
		debug_draw_3d = null;
	}

	public override void _ExitTree(){
		debug_draw_3d?.QueueFree();
		debug_draw_3d = null;
	}

	public static Node GetSingleton(){
		return (Node)debug_draw_3d?.Call("get_singleton");
	}


	/// Draw Functions

	/// <summary>
	/// </summary>
	public static Dictionary GetRenderedPrimitivesCount(){
		return (Dictionary)debug_draw_3d?.Call("get_rendered_primitives_count");
	}

	/// <summary>
	/// Clear all 3D objects
	/// </summary>
	public static void Clear3dObjects(){
		debug_draw_3d?.Call("clear_3d_objects");
	}

	/// <summary>
	/// Clear all 2D objects
	/// </summary>
	public static void Clear2dObjects(){
		debug_draw_3d?.Call("clear_2d_objects");
	}

	/// <summary>
	/// Clear all debug objects
	/// </summary>
	public static void ClearAll(){
		debug_draw_3d?.Call("clear_all");
	}

	/// <summary>
	/// Draw sphere
	/// </summary>
	/// <param name="position">Position of the sphere center</param>
	/// <param name="radius">Sphere radius</param>
	/// <param name="color">Sphere color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawSphere(Vector3 position, float radius = 0.5f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_sphere", position, radius, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw sphere
	/// </summary>
	/// <param name="transform">Transform of the sphere</param>
	/// <param name="color">Sphere color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawSphereXf(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_sphere_xf", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw sphere with higher lines count
	/// </summary>
	/// <param name="position">Position of the sphere center</param>
	/// <param name="radius">Sphere radius</param>
	/// <param name="color">Sphere color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawSphereHd(Vector3 position, float radius = 0.5f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_sphere_hd", position, radius, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw sphere with higher lines count
	/// </summary>
	/// <param name="transform">Transform of the sphere</param>
	/// <param name="color">Sphere color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawSphereHdXf(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_sphere_hd_xf", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw vertical cylinder
	/// </summary>
	/// <param name="transform">Cylinder transform</param>
	/// <param name="color">Cylinder color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawCylinder(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_cylinder", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw rotated box
	/// </summary>
	/// <param name="transform">Box transform</param>
	/// <param name="color">Box color</param>
	/// <param name="isBoxCentered">Use 'position' as center of the box</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawBox(Transform transform, Color? color = null, bool is_box_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_box", transform, color == null? empty_color : color, is_box_centered, duration);
	}

	/// <summary>
	/// Draw AABB
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="color">Box color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawAabb(AABB aabb, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_aabb", aabb, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw AABB from 'a' to 'b'
	/// </summary>
	/// <param name="a">Firts corner</param>
	/// <param name="b">Second corner</param>
	/// <param name="color">Box color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawAabbAb(Vector3 a, Vector3 b, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_aabb_ab", a, b, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// </summary>
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="is_hit">Is hit</param>
	/// <param name="unitOffsetOfHit">Unit offset on the line where the hit occurs</param>
	/// <param name="hitColor">Color of the hit point and line before hit</param>
	/// <param name="afterHitColor">Color of line after hit position</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLineHit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_hit", start, end, hit, is_hit, hit_size, hit_color == null? empty_color : hit_color, after_hit_color == null? empty_color : after_hit_color, duration);
	}

	/// <summary>
	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// </summary>
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="is_hit">Is hit</param>
	/// <param name="unitOffsetOfHit">Unit offset on the line where the hit occurs</param>
	/// <param name="hitColor">Color of the hit point and line before hit</param>
	/// <param name="afterHitColor">Color of line after hit position</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLineHitOffset(Vector3 start, Vector3 end, bool is_hit, float unit_offset_of_hit = 0.5f, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_hit_offset", start, end, is_hit, unit_offset_of_hit, hit_size, hit_color == null? empty_color : hit_color, after_hit_color == null? empty_color : after_hit_color, duration);
	}

	/// <summary>
	/// Draw line
	/// </summary>
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="color">Line color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLine(Vector3 a, Vector3 b, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line", a, b, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw many line
	/// </summary>
	/// <param name="lines">Array of line points. 1 line = 2 Vector3. The size of the array must be even.</param>
	/// <param name="color">Line color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLines(Vector3[] lines, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_lines", lines, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw ray
	/// </summary>
	/// <param name="origin">Origin</param>
	/// <param name="direction">Direction</param>
	/// <param name="length">Length</param>
	/// <param name="color">Ray color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawRay(Vector3 origin, Vector3 direction, float length, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_ray", origin, direction, length, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw a sequence of points connected by lines
	/// </summary>
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLinePath(Vector3[] path, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_path", path, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw arrow
	/// </summary>
	/// <param name="Transform">Start point</param>
	/// <param name="color">Line color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawArrow(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw line with arrow
	/// </summary>
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="color">Line color</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the 'arrowSize' absolute or relative to the length of the line?</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawArrowLine(Vector3 a, Vector3 b, Color? color = null, float arrow_size = 0.5f, bool absolute_size = false, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_line", a, b, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// <summary>
	/// Draw ray with arrow
	/// </summary>
	/// <param name="origin">Origin</param>
	/// <param name="direction">Direction</param>
	/// <param name="length">Length</param>
	/// <param name="color">Ray color</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the 'arrowSize' absolute or relative to the length of the line?</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawArrowRay(Vector3 origin, Vector3 direction, float length, Color? color = null, float arrow_size = 0.5f, bool absolute_size = false, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_ray", origin, direction, length, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// <summary>
	/// Draw a sequence of points connected by lines with arrows
	/// </summary>
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the 'arrowSize' absolute or relative to the length of the line?</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawArrowPath(Vector3[] path, Color? color = null, float arrow_size = 0.75f, bool absolute_size = true, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_path", path, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// <summary>
	/// Draw a square that will always be turned towards the camera
	/// </summary>
	/// <param name="position">Center position of square</param>
	/// <param name="color">Color</param>
	/// <param name="size">Unit size</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawBillboardSquare(Vector3 position, float size = 0.2f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_billboard_square", position, size, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw 3 intersecting lines with the given transformations
	/// </summary>
	/// <param name="transform">Transform</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawPosition(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_position", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw 3 intersecting lines with the given transformations and arrows at the ends
	/// </summary>
	/// <param name="transform">Transform</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawGizmo(Transform transform, bool is_centered = false, float duration = 0f){
		debug_draw_3d?.Call("draw_gizmo", transform, is_centered, duration);
	}

	/// <summary>
	/// Draw simple grid with given size and subdivision
	/// </summary>
	/// <param name="origin"></param>
	/// <param name="x_size"></param>
	/// <param name="y_size"></param>
	/// <param name="subdivision"></param>
	/// <param name="color"></param>
	/// <param name="is_centered"></param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawGrid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2 subdivision, Color? color = null, bool is_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_grid", origin, x_size, y_size, subdivision, color == null? empty_color : color, is_centered, duration);
	}

	/// <summary>
	/// Draw simple grid with given size and subdivision
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="subdivision"></param>
	/// <param name="color"></param>
	/// <param name="is_centered"></param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawGridXf(Transform transform, Vector2 subdivision, Color? color = null, bool is_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_grid_xf", transform, subdivision, color == null? empty_color : color, is_centered, duration);
	}

	/// <summary>
	/// Draw camera frustum area
	/// </summary>
	/// <param name="camera">Camera node</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawCameraFrustum(Camera camera, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_camera_frustum", camera, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw camera frustum area
	/// </summary>
	/// <param name="cameraFrustum">Array of frustum planes</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawCameraFrustumPlanes(Array camera_frustum, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_camera_frustum_planes", camera_frustum, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Begin text group
	/// </summary>
	/// <param name="groupTitle">Group title and ID</param>
	/// <param name="groupPriority">Group priority</param>
	/// <param name="showTitle">Whether to show the title</param>
	public static void BeginTextGroup(string group_title, int group_priority = 0, Color? group_color = null, bool show_title = true){
		debug_draw_3d?.Call("begin_text_group", group_title, group_priority, group_color == null? empty_color : group_color, show_title);
	}

	/// <summary>
	/// End text group. Should be called after 'begin_text_group' if you don't need more than one group.
	/// If you need to create 2+ groups just call again 'begin_text_group' and this function in the end.
	/// </summary>
	/// <param name="groupTitle">Group title and ID</param>
	/// <param name="groupPriority">Group priority</param>
	/// <param name="showTitle">Whether to show the title</param>
	public static void EndTextGroup(){
		debug_draw_3d?.Call("end_text_group");
	}

	/// <summary>
	/// Add or update text in overlay
	/// </summary>
	/// <param name="key">Name of field if 'value' exists, otherwise whole line will equal 'key'.</param>
	/// <param name="value">Value of field</param>
	/// <param name="priority">Priority of this line. Lower value is higher position.</param>
	/// <param name="duration">Expiration time</param>
	public static void SetText(string key, object value = null, int priority = 0, Color? color_of_value = null, float duration = -1f){
		debug_draw_3d?.Call("set_text", key, value, priority, color_of_value == null? empty_color : color_of_value, duration);
	}

	/// <summary>
	/// Create new graph with custom data.
	/// To get more information about the returned class, please see the documentation.
	/// </summary>
	/// <param name="title">Title of the graph</param>
	public static Reference CreateGraph(string title){
		return (Reference)debug_draw_3d?.Call("create_graph", title);
	}

	/// <summary>
	/// Create new graph with custom data.
	/// To get more information about the returned class, please see the documentation.
	/// </summary>
	/// <param name="title">Title of the graph</param>
	public static Reference CreateFpsGraph(string title){
		return (Reference)debug_draw_3d?.Call("create_fps_graph", title);
	}

	/// <summary>
	/// Update custom graph data
	/// </summary>
	/// <param name="title">Title of the graph</param>
	/// <param name="data">New data</param>
	public static void GraphUpdateData(string title, float data){
		debug_draw_3d?.Call("graph_update_data", title, data);
	}

	/// <summary>
	/// Remove graph
	/// </summary>
	/// <param name="title">Title of the graph</param>
	public static void RemoveGraph(string title){
		debug_draw_3d?.Call("remove_graph", title);
	}

	/// <summary>
	/// Remove all graphs
	/// </summary>
	public static void ClearGraphs(){
		debug_draw_3d?.Call("clear_graphs");
	}

	/// <summary>
	/// Get config for graph.
	/// To get more information about the returned class, please see the documentation.
	/// </summary>
	/// <param name="title">Title of the graph</param>
	public static Reference GetGraphConfig(string title){
		return (Reference)debug_draw_3d?.Call("get_graph_config", title);
	}

	/// <summary>
	/// Get all graph names
	/// </summary>
	public static string[] GetGraphNames(){
		return (string[])debug_draw_3d?.Call("get_graph_names");
	}

}

