using Godot;
using Godot.Collections;

[Tool]
public class DebugDrawCS : Node
{
	static Color empty_color = new Color(0,0,0,0);
	static Node debug_draw_3d = null;


	/// Constants

	public static int BlockPosition_LeftTop { get; private set; }
	public static int BlockPosition_RightTop { get; private set; }
	public static int BlockPosition_LeftBottom { get; private set; }
	public static int BlockPosition_RightBottom { get; private set; }
	public static int FPSGraphTextFlags_None { get; private set; }
	public static int FPSGraphTextFlags_Current { get; private set; }
	public static int FPSGraphTextFlags_Avarage { get; private set; }
	public static int FPSGraphTextFlags_Max { get; private set; }
	public static int FPSGraphTextFlags_Min { get; private set; }
	public static int FPSGraphTextFlags_All { get; private set; }


	/// Parameters

	/// Recall all calls from DebugDraw3D instance to its singleton if needed
	public static bool RecallToSingleton { get => (bool)debug_draw_3d?.Get("recall_to_singleton"); set => debug_draw_3d?.Set("recall_to_singleton", value); }

	/// Enable or disable all debug draw.
	public static bool DebugEnabled { get => (bool)debug_draw_3d?.Get("debug_enabled"); set => debug_draw_3d?.Set("debug_enabled", value); }

	/// Debug for debug...
	public static bool Freeze3dRender { get => (bool)debug_draw_3d?.Get("freeze_3d_render"); set => debug_draw_3d?.Set("freeze_3d_render", value); }

	/// Debug for debug...
	public static bool DrawInstanceBounds { get => (bool)debug_draw_3d?.Get("draw_instance_bounds"); set => debug_draw_3d?.Set("draw_instance_bounds", value); }

	/// Geometry culling based on camera frustum. Change to false to disable it
	public static bool UseFrustumCulling { get => (bool)debug_draw_3d?.Get("use_frustum_culling"); set => debug_draw_3d?.Set("use_frustum_culling", value); }

	/// Force use camera placed on edited scene. Usable for editor.
	public static bool ForceUseCameraFromScene { get => (bool)debug_draw_3d?.Get("force_use_camera_from_scene"); set => debug_draw_3d?.Set("force_use_camera_from_scene", value); }

	/// Position of text block
	public static int TextBlockPosition { get => (int)debug_draw_3d?.Get("text_block_position"); set => debug_draw_3d?.Set("text_block_position", value); }

	/// Offset from the corner selected in 'text_block_position'
	public static Vector2 TextBlockOffset { get => (Vector2)debug_draw_3d?.Get("text_block_offset"); set => debug_draw_3d?.Set("text_block_offset", value); }

	/// Text padding for each line
	public static Vector2 TextPadding { get => (Vector2)debug_draw_3d?.Get("text_padding"); set => debug_draw_3d?.Set("text_padding", value); }

	/// How long HUD text lines remain shown after being invoked.
	public static float TextDefaultDuration { get => (float)debug_draw_3d?.Get("text_default_duration"); set => debug_draw_3d?.Set("text_default_duration", value); }

	/// Color of the text drawn as HUD
	public static Color TextForegroundColor { get => (Color)debug_draw_3d?.Get("text_foreground_color"); set => debug_draw_3d?.Set("text_foreground_color", value); }

	/// Background color of the text drawn as HUD
	public static Color TextBackgroundColor { get => (Color)debug_draw_3d?.Get("text_background_color"); set => debug_draw_3d?.Set("text_background_color", value); }

	/// Custom Font
	public static Font TextCustomFont { get => (Font)debug_draw_3d?.Get("text_custom_font"); set => debug_draw_3d?.Set("text_custom_font", value); }

	/// Color of line with hit
	public static Color LineHitColor { get => (Color)debug_draw_3d?.Get("line_hit_color"); set => debug_draw_3d?.Set("line_hit_color", value); }

	/// Color of line after hit
	public static Color LineAfterHitColor { get => (Color)debug_draw_3d?.Get("line_after_hit_color"); set => debug_draw_3d?.Set("line_after_hit_color", value); }

	/// Custom 'Viewport' to use for frustum culling. Usually used in editor.
	public static Viewport CustomViewport { get => (Viewport)debug_draw_3d?.Get("custom_viewport"); set => debug_draw_3d?.Set("custom_viewport", value); }

	/// Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
	public static CanvasItem CustomCanvas { get => (CanvasItem)debug_draw_3d?.Get("custom_canvas"); set => debug_draw_3d?.Set("custom_canvas", value); }


	/// Init
	public DebugDrawCS(){
		var f = new File();
		if (f.FileExists("res://addons/debug_draw_3d/debug_draw_3d.gdns")){
			debug_draw_3d = ResourceLoader.Load<NativeScript>("res://addons/debug_draw_3d/debug_draw_3d.gdns").New() as Node;

			BlockPosition_LeftTop = (int)debug_draw_3d.Get("BlockPosition_LeftTop");
			BlockPosition_RightTop = (int)debug_draw_3d.Get("BlockPosition_RightTop");
			BlockPosition_LeftBottom = (int)debug_draw_3d.Get("BlockPosition_LeftBottom");
			BlockPosition_RightBottom = (int)debug_draw_3d.Get("BlockPosition_RightBottom");
			FPSGraphTextFlags_None = (int)debug_draw_3d.Get("FPSGraphTextFlags_None");
			FPSGraphTextFlags_Current = (int)debug_draw_3d.Get("FPSGraphTextFlags_Current");
			FPSGraphTextFlags_Avarage = (int)debug_draw_3d.Get("FPSGraphTextFlags_Avarage");
			FPSGraphTextFlags_Max = (int)debug_draw_3d.Get("FPSGraphTextFlags_Max");
			FPSGraphTextFlags_Min = (int)debug_draw_3d.Get("FPSGraphTextFlags_Min");
			FPSGraphTextFlags_All = (int)debug_draw_3d.Get("FPSGraphTextFlags_All");
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

	public static Dictionary GetRenderedPrimitivesCount(){
		return (Dictionary)debug_draw_3d?.Call("get_rendered_primitives_count");
	}

	/// Clear all 3D objects
	public static void Clear3dObjects(){
		debug_draw_3d?.Call("clear_3d_objects");
	}

	/// Clear all 2D objects
	public static void Clear2dObjects(){
		debug_draw_3d?.Call("clear_2d_objects");
	}

	/// Clear all debug objects
	public static void ClearAll(){
		debug_draw_3d?.Call("clear_all");
	}

	/// Draw sphere
	/// position: Position of the sphere center
	/// radius: Sphere radius
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	public static void DrawSphere(Vector3 position, float radius = 0.5f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_sphere", position, radius, color == null? empty_color : color, duration);
	}

	/// Draw sphere
	/// transform: Transform of the sphere
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	public static void DrawSphereXf(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_sphere_xf", transform, color == null? empty_color : color, duration);
	}

	/// Draw vertical cylinder
	/// position: Center position
	/// radius: Cylinder radius
	/// height: Cylinder height
	/// color: Cylinder color
	/// duration: Duration of existence in seconds
	public static void DrawCylinder(Vector3 position, Quat rotation, float radius = 1f, float height = 1f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_cylinder", position, rotation, radius, height, color == null? empty_color : color, duration);
	}

	/// Draw vertical cylinder
	/// transform: Cylinder transform
	/// color: Cylinder color
	/// duration: Duration of existence in seconds
	public static void DrawCylinderXf(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_cylinder_xf", transform, color == null? empty_color : color, duration);
	}

	/// Draw rotated box
	/// position: Position of the box
	/// rotation: Box rotation
	/// size: Box size
	/// color: Box color
	/// duration: Duration of existence in seconds
	/// isBoxCentered: Use 'position' as center of the box
	public static void DrawBox(Vector3 position, Quat rotation, Vector3 size, Color? color = null, bool is_box_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_box", position, rotation, size, color == null? empty_color : color, is_box_centered, duration);
	}

	/// Draw rotated box
	/// transform: Box transform
	/// color: Box color
	/// duration: Duration of existence in seconds
	/// isBoxCentered: Use 'position' as center of the box
	public static void DrawBoxXf(Transform transform, Color? color = null, bool is_box_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_box_xf", transform, color == null? empty_color : color, is_box_centered, duration);
	}

	/// Draw AABB
	/// aabb: AABB
	/// color: Box color
	/// duration: Duration of existence in seconds
	public static void DrawAabb(AABB aabb, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_aabb", aabb, color == null? empty_color : color, duration);
	}

	/// Draw AABB from 'a' to 'b'
	/// a: Firts corner
	/// b: Second corner
	/// color: Box color
	/// duration: Duration of existence in seconds
	public static void DrawAabbAb(Vector3 a, Vector3 b, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_aabb_ab", a, b, color == null? empty_color : color, duration);
	}

	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// a: Start point
	/// b: End point
	/// is_hit: Is hit
	/// unitOffsetOfHit: Unit offset on the line where the hit occurs
	/// duration: Duration of existence in seconds
	/// hitColor: Color of the hit point and line before hit
	/// afterHitColor: Color of line after hit position
	public static void DrawLine3dHit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_3d_hit", start, end, hit, is_hit, hit_size, hit_color == null? empty_color : hit_color, after_hit_color == null? empty_color : after_hit_color, duration);
	}

	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// a: Start point
	/// b: End point
	/// is_hit: Is hit
	/// unitOffsetOfHit: Unit offset on the line where the hit occurs
	/// duration: Duration of existence in seconds
	/// hitColor: Color of the hit point and line before hit
	/// afterHitColor: Color of line after hit position
	public static void DrawLine3dHitOffset(Vector3 start, Vector3 end, bool is_hit, float unit_offset_of_hit = 0.5f, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_3d_hit_offset", start, end, is_hit, unit_offset_of_hit, hit_size, hit_color == null? empty_color : hit_color, after_hit_color == null? empty_color : after_hit_color, duration);
	}

	/// Draw line
	/// a: Start point
	/// b: End point
	/// color: Line color
	/// duration: Duration of existence in seconds
	public static void DrawLine3d(Vector3 a, Vector3 b, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_3d", a, b, color == null? empty_color : color, duration);
	}

	/// Draw ray
	/// origin: Origin
	/// direction: Direction
	/// length: Length
	/// color: Ray color
	/// duration: Duration of existence in seconds
	public static void DrawRay3d(Vector3 origin, Vector3 direction, float length, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_ray_3d", origin, direction, length, color == null? empty_color : color, duration);
	}

	/// Draw a sequence of points connected by lines
	/// path: Sequence of points
	/// color: Color
	/// duration: Duration of existence in seconds
	public static void DrawLinePath3d(Vector3[] path, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_path_3d", path, color == null? empty_color : color, duration);
	}

	/// Draw line with arrow
	/// a: Start point
	/// b: End point
	/// color: Line color
	/// duration: Duration of existence in seconds
	/// arrowSize: Size of the arrow
	/// absoluteSize: Is the 'arrowSize' absolute or relative to the length of the line?
	public static void DrawArrowLine3d(Vector3 a, Vector3 b, Color? color = null, float arrow_size = 0.5f, bool absolute_size = true, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_line_3d", a, b, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// Draw ray with arrow
	/// origin: Origin
	/// direction: Direction
	/// length: Length
	/// color: Ray color
	/// duration: Duration of existence in seconds
	/// arrowSize: Size of the arrow
	/// absoluteSize: Is the 'arrowSize' absolute or relative to the length of the line?
	public static void DrawArrowRay3d(Vector3 origin, Vector3 direction, float length, Color? color = null, float arrow_size = 0.5f, bool absolute_size = true, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_ray_3d", origin, direction, length, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// Draw a sequence of points connected by lines with arrows
	/// path: Sequence of points
	/// color: Color
	/// duration: Duration of existence in seconds
	/// arrowSize: Size of the arrow
	/// absoluteSize: Is the 'arrowSize' absolute or relative to the length of the line?
	public static void DrawArrowPath3d(Vector3[] path, Color? color = null, float arrow_size = 0.75f, bool absolute_size = true, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_path_3d", path, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// Draw a square that will always be turned towards the camera
	/// position: Center position of square
	/// color: Color
	/// size: Unit size
	/// duration: Duration of existence in seconds
	public static void DrawBillboardSquare(Vector3 position, float size = 0.2f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_billboard_square", position, size, color == null? empty_color : color, duration);
	}

	/// Draw camera frustum area
	/// camera: Camera node
	/// color: Color
	/// duration: Duration of existence in seconds
	public static void DrawCameraFrustum(Camera camera, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_camera_frustum", camera, color == null? empty_color : color, duration);
	}

	/// Draw camera frustum area
	/// cameraFrustum: Array of frustum planes
	/// color: Color
	/// duration: Duration of existence in seconds
	public static void DrawCameraFrustumPlanes(Array camera_frustum, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_camera_frustum_planes", camera_frustum, color == null? empty_color : color, duration);
	}

	/// Draw 3 intersecting lines with the given transformations
	/// position: Center position
	/// rotation: Rotation
	/// scale: Scale
	/// color: Color
	/// duration: Duration of existence in seconds
	public static void DrawPosition3d(Vector3 position, Quat rotation, Vector3 scale, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_position_3d", position, rotation, scale, color == null? empty_color : color, duration);
	}

	/// Draw 3 intersecting lines with the given transformations
	/// transform: Transform
	/// color: Color
	/// duration: Duration of existence in seconds
	public static void DrawPosition3dXf(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_position_3d_xf", transform, color == null? empty_color : color, duration);
	}

	/// Begin text group
	/// groupTitle: Group title and ID
	/// groupPriority: Group priority
	/// showTitle: Whether to show the title
	public static void BeginTextGroup(string group_title, int group_priority = 0, Color? group_color = null, bool show_title = true){
		debug_draw_3d?.Call("begin_text_group", group_title, group_priority, group_color == null? empty_color : group_color, show_title);
	}

	/// End text group. Should be called after 'begin_text_group' if you don't need more than one group.
	/// If you need to create 2+ groups just call again 'begin_text_group' and this function in the end.
	/// groupTitle: Group title and ID
	/// groupPriority: Group priority
	/// showTitle: Whether to show the title
	public static void EndTextGroup(){
		debug_draw_3d?.Call("end_text_group");
	}

	/// Add or update text in overlay
	/// key: Name of field if 'value' exists, otherwise whole line will equal 'key'.
	/// value: Value of field
	/// priority: Priority of this line. Lower value is higher position.
	/// duration: Expiration time
	public static void SetText(string key, object value = null, int priority = 0, Color? color_of_value = null, float duration = -1f){
		debug_draw_3d?.Call("set_text", key, value, priority, color_of_value == null? empty_color : color_of_value, duration);
	}

	/// Create new graph with custom data
	/// title: Title of the graph
	public static Reference CreateGraph(string title){
		return (Reference)debug_draw_3d?.Call("create_graph", title);
	}

	/// Create new graph with custom data
	/// title: Title of the graph
	public static Reference CreateFpsGraph(string title){
		return (Reference)debug_draw_3d?.Call("create_fps_graph", title);
	}

	/// Update custom graph data
	/// title: Title of the graph
	/// data: New data
	public static void GraphUpdateData(string title, float data){
		debug_draw_3d?.Call("graph_update_data", title, data);
	}

	/// Remove graph
	/// title: Title of the graph
	public static void RemoveGraph(string title){
		debug_draw_3d?.Call("remove_graph", title);
	}

	/// Remove all graphs
	public static void ClearGraphs(){
		debug_draw_3d?.Call("clear_graphs");
	}

	/// Get config for graph
	/// title: Title of the graph
	public static Reference GetGraphConfig(string title){
		return (Reference)debug_draw_3d?.Call("get_graph_config", title);
	}

	/// Get all graph names
	public static string[] GetGraphNames(){
		return (string[])debug_draw_3d?.Call("get_graph_names");
	}

}

