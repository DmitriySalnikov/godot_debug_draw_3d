
// This condition will indicate when the API implementation needs to be compiled.
// While it is not recommended to change the conditions here, you can add
// DEBUG_DRAW_3D_IMPLEMENTATION to the "Conditional compilation symbols" in your project properties.
#if DEBUG // examples: DEBUG || WITH_3D_DEBUG, (GODOT_WINDOWS || GODOT_LINUX) && !TOOLS, true
#define DEBUG_DRAW_3D_IMPLEMENTATION
#endif

/// ////////////////////////////////////////////////
/// THIS FILE HAS BEEN GENERATED.
/// CHANGES IN THIS FILE WILL BE OVERWRITTEN AFTER THE UPDATE!
/// SEE debug_draw_3d/generate_debug_draw_3d_api.gd
/// ////////////////////////////////////////////////

using Godot;
using Godot.Collections;

[Tool]
public class DebugDrawCS : Node
{
#if DEBUG_DRAW_3D_IMPLEMENTATION
	/// Indicates the need for library initialization.
	/// Can be overridden in the project settings.
	static string is_need_to_initialize_lib_setting = "debug/settings/debug_draw_3d/is_need_to_initialize_lib";
	
	static Color empty_color = new Color(0,0,0,0);
	static Node debug_draw_3d = null;
#endif

#region Constants
#if DEBUG_DRAW_3D_IMPLEMENTATION
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
#else
	public const int BlockPosition_LeftTop = 0;
	public const int BlockPosition_RightTop = 0;
	public const int BlockPosition_LeftBottom = 0;
	public const int BlockPosition_RightBottom = 0;
	public const int GraphTextFlags_None = 0;
	public const int GraphTextFlags_Current = 0;
	public const int GraphTextFlags_Avarage = 0;
	public const int GraphTextFlags_Max = 0;
	public const int GraphTextFlags_Min = 0;
	public const int GraphTextFlags_All = 0;
#endif
#endregion Constants


#region Parameters
#if DEBUG_DRAW_3D_IMPLEMENTATION
	/// <summary>
	/// Recall all calls from DebugDraw3D instance to its singleton if needed
	/// </summary>
	public static bool RecallToSingleton { get => debug_draw_3d != null ? (bool)debug_draw_3d.Get("recall_to_singleton") : default; set => debug_draw_3d?.Set("recall_to_singleton", value); }

	/// <summary>
	/// Enable or disable all debug draw
	/// </summary>
	public static bool DebugEnabled { get => debug_draw_3d != null ? (bool)debug_draw_3d.Get("debug_enabled") : default; set => debug_draw_3d?.Set("debug_enabled", value); }

	/// <summary>
	/// Freezing 3d debugging instances
	/// </summary>
	public static bool Freeze3dRender { get => debug_draw_3d != null ? (bool)debug_draw_3d.Get("freeze_3d_render") : default; set => debug_draw_3d?.Set("freeze_3d_render", value); }

	/// <summary>
	/// Debug for debug...
	/// </summary>
	public static bool VisibleInstanceBounds { get => debug_draw_3d != null ? (bool)debug_draw_3d.Get("visible_instance_bounds") : default; set => debug_draw_3d?.Set("visible_instance_bounds", value); }

	/// <summary>
	/// Geometry culling based on camera frustum.
	/// It is not recommended to use with the current implementation.
	/// </summary>
	public static bool UseFrustumCulling { get => debug_draw_3d != null ? (bool)debug_draw_3d.Get("use_frustum_culling") : default; set => debug_draw_3d?.Set("use_frustum_culling", value); }

	/// <summary>
	/// Force use camera placed on edited scene.
	/// Usable for editor.
	/// </summary>
	public static bool ForceUseCameraFromScene { get => debug_draw_3d != null ? (bool)debug_draw_3d.Get("force_use_camera_from_scene") : default; set => debug_draw_3d?.Set("force_use_camera_from_scene", value); }

	/// <summary>
	/// Base offset for all graphs
	/// </summary>
	public static Vector2 GraphsBaseOffset { get => debug_draw_3d != null ? (Vector2)debug_draw_3d.Get("graphs_base_offset") : default; set => debug_draw_3d?.Set("graphs_base_offset", value); }

	/// <summary>
	/// Layers on which the geometry will be displayed
	/// </summary>
	public static int GeometryRenderLayers { get => debug_draw_3d != null ? (int)debug_draw_3d.Get("geometry_render_layers") : default; set => debug_draw_3d?.Set("geometry_render_layers", value); }

	/// <summary>
	/// Position of text block
	/// </summary>
	public static int TextBlockPosition { get => debug_draw_3d != null ? (int)debug_draw_3d.Get("text_block_position") : default; set => debug_draw_3d?.Set("text_block_position", value); }

	/// <summary>
	/// Offset from the corner selected in 'text_block_position'
	/// </summary>
	public static Vector2 TextBlockOffset { get => debug_draw_3d != null ? (Vector2)debug_draw_3d.Get("text_block_offset") : default; set => debug_draw_3d?.Set("text_block_offset", value); }

	/// <summary>
	/// Text padding for each line
	/// </summary>
	public static Vector2 TextPadding { get => debug_draw_3d != null ? (Vector2)debug_draw_3d.Get("text_padding") : default; set => debug_draw_3d?.Set("text_padding", value); }

	/// <summary>
	/// How long text remain shown after being invoked.
	/// </summary>
	public static float TextDefaultDuration { get => debug_draw_3d != null ? (float)debug_draw_3d.Get("text_default_duration") : default; set => debug_draw_3d?.Set("text_default_duration", value); }

	/// <summary>
	/// Default color of the text
	/// </summary>
	public static Color TextForegroundColor { get => debug_draw_3d != null ? (Color)debug_draw_3d.Get("text_foreground_color") : default; set => debug_draw_3d?.Set("text_foreground_color", value); }

	/// <summary>
	/// Background color of the text
	/// </summary>
	public static Color TextBackgroundColor { get => debug_draw_3d != null ? (Color)debug_draw_3d.Get("text_background_color") : default; set => debug_draw_3d?.Set("text_background_color", value); }

	/// <summary>
	/// Custom text Font
	/// </summary>
	public static Font TextCustomFont { get => debug_draw_3d != null ? (Font)debug_draw_3d.Get("text_custom_font") : default; set => debug_draw_3d?.Set("text_custom_font", value); }

	/// <summary>
	/// Default color of line with hit
	/// </summary>
	public static Color LineHitColor { get => debug_draw_3d != null ? (Color)debug_draw_3d.Get("line_hit_color") : default; set => debug_draw_3d?.Set("line_hit_color", value); }

	/// <summary>
	/// Default color of line after hit
	/// </summary>
	public static Color LineAfterHitColor { get => debug_draw_3d != null ? (Color)debug_draw_3d.Get("line_after_hit_color") : default; set => debug_draw_3d?.Set("line_after_hit_color", value); }

	/// <summary>
	/// Custom 'Viewport' to use for frustum culling.
	/// </summary>
	public static Viewport CustomViewport { get => debug_draw_3d != null ? (Viewport)debug_draw_3d.Get("custom_viewport") : default; set => debug_draw_3d?.Set("custom_viewport", value); }

	/// <summary>
	/// Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
	/// </summary>
	public static CanvasItem CustomCanvas { get => debug_draw_3d != null ? (CanvasItem)debug_draw_3d.Get("custom_canvas") : default; set => debug_draw_3d?.Set("custom_canvas", value); }

#else
	public static bool RecallToSingleton = default;
	public static bool DebugEnabled = default;
	public static bool Freeze3dRender = default;
	public static bool VisibleInstanceBounds = default;
	public static bool UseFrustumCulling = default;
	public static bool ForceUseCameraFromScene = default;
	public static Vector2 GraphsBaseOffset = default;
	public static int GeometryRenderLayers = default;
	public static int TextBlockPosition = default;
	public static Vector2 TextBlockOffset = default;
	public static Vector2 TextPadding = default;
	public static float TextDefaultDuration = default;
	public static Color TextForegroundColor = default;
	public static Color TextBackgroundColor = default;
	public static Font TextCustomFont = default;
	public static Color LineHitColor = default;
	public static Color LineAfterHitColor = default;
	public static Viewport CustomViewport = default;
	public static CanvasItem CustomCanvas = default;
#endif

#endregion Parameters


#region Init
#if DEBUG_DRAW_3D_IMPLEMENTATION
	public DebugDrawCS(){
		_register_setting(is_need_to_initialize_lib_setting, false);
		_register_setting(is_need_to_initialize_lib_setting + ".debug", true);
		
		var f = new File();
		if ((Engine.EditorHint || (bool)ProjectSettings.GetSetting(is_need_to_initialize_lib_setting))
			&& f.FileExists("res://addons/debug_draw_3d/libs/debug_draw_3d.gdns")){
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
		if (!Engine.EditorHint && debug_draw_3d != null){
			if(debug_draw_3d.Call("get_singleton") == null)
				AddChild(debug_draw_3d);
		}
	}

	protected void _register_setting(string name, object default_value){
		if (!ProjectSettings.HasSetting(name))
			ProjectSettings.SetSetting(name, default_value);

		ProjectSettings.SetInitialValue(name, default_value);
		ProjectSettings.AddPropertyInfo(new Dictionary {{"name", name}, {"type", Variant.Type.Bool}});
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
#endif
#endregion Init


#region Draw Functions
#if DEBUG_DRAW_3D_IMPLEMENTATION
	/// <summary>
	/// Returns a dictionary with rendering statistics.
	/// Some data can be delayed by 1 frame.
	/// </summary>
	public static Dictionary GetRenderStats(){
		if (debug_draw_3d != null){
			return (Dictionary)debug_draw_3d.Call("get_render_stats");
		}
		else{
			return new Dictionary();
		}
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
	/// <param name="transform">Transform of the Sphere</param>
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
	/// <param name="transform">Transform of the Cylinder</param>
	/// <param name="color">Cylinder color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawCylinder(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_cylinder", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw box
	/// </summary>
	/// <param name="position">Position of the Box</param>
	/// <param name="size">Size of the Box</param>
	/// <param name="color">Box color</param>
	/// <param name="is_box_centered">Use 'position' as the center of the box, not as the bottom corner</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawBox(Vector3 position, Vector3 size, Color? color = null, bool is_box_centered = false, float duration = 0f){
		debug_draw_3d?.Call("draw_box", position, size, color == null? empty_color : color, is_box_centered, duration);
	}

	/// <summary>
	/// Draw box
	/// </summary>
	/// <param name="transform">Transform of the Box</param>
	/// <param name="color">Box color</param>
	/// <param name="is_box_centered">Use 'transform' as the center of the box, not as the bottom corner</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawBoxXf(Transform transform, Color? color = null, bool is_box_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_box_xf", transform, color == null? empty_color : color, is_box_centered, duration);
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
	/// <param name="start">Start point</param>
	/// <param name="end">End point</param>
	/// <param name="hit">Hit point</param>
	/// <param name="is_hit">Is hit</param>
	/// <param name="hit_size">Size of the hit point</param>
	/// <param name="hit_color">Color of the hit point and line before hit</param>
	/// <param name="after_hit_color">Color of line after hit position</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLineHit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_hit", start, end, hit, is_hit, hit_size, hit_color == null? empty_color : hit_color, after_hit_color == null? empty_color : after_hit_color, duration);
	}

	/// <summary>
	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// </summary>
	/// <param name="start">Start point</param>
	/// <param name="end">End point</param>
	/// <param name="is_hit">Is hit</param>
	/// <param name="unit_offset_of_hit">Unit offset on the line where the hit occurs</param>
	/// <param name="hit_size">Size of the hit point</param>
	/// <param name="hit_color">Color of the hit point and line before hit</param>
	/// <param name="after_hit_color">Color of line after hit position</param>
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
	/// Draw many lines
	/// </summary>
	/// <param name="lines">Array of line points. 1 line = 2 Vector3. The size of the array must be even.</param>
	/// <param name="color">Lines color</param>
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
	/// <param name="color">Lines Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawLinePath(Vector3[] path, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_line_path", path, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw arrow
	/// </summary>
	/// <param name="Transform">Transform of the Arrow</param>
	/// <param name="color">Arrow color</param>
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
	/// <param name="arrow_size">Size of the arrow</param>
	/// <param name="absolute_size">Is the 'arrowSize' absolute or relative to the length of the line?</param>
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
	/// <param name="arrow_size">Size of the arrow</param>
	/// <param name="absolute_size">Is the 'arrowSize' absolute or relative to the length of the line?</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawArrowRay(Vector3 origin, Vector3 direction, float length, Color? color = null, float arrow_size = 0.5f, bool absolute_size = false, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_ray", origin, direction, length, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// <summary>
	/// Draw a sequence of points connected by lines with arrows
	/// </summary>
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="arrow_size">Size of the arrow</param>
	/// <param name="absolute_size">Is the 'arrowSize' absolute or relative to the length of the line?</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawArrowPath(Vector3[] path, Color? color = null, float arrow_size = 0.75f, bool absolute_size = true, float duration = 0f){
		debug_draw_3d?.Call("draw_arrow_path", path, color == null? empty_color : color, arrow_size, absolute_size, duration);
	}

	/// <summary>
	/// Draw a sequence of points connected by lines with billboard squares
	/// </summary>
	/// <param name="path">Sequence of points</param>
	/// <param name="points_color">Color of points</param>
	/// <param name="lines_color">Color of lines</param>
	/// <param name="size">Size of squares</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawPointPath(Vector3[] path, float size = 0.25f, Color? points_color = null, Color? lines_color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_point_path", path, size, points_color == null? empty_color : points_color, lines_color == null? empty_color : lines_color, duration);
	}

	/// <summary>
	/// Draw a square that will always be turned towards the camera
	/// </summary>
	/// <param name="position">Center position of square</param>
	/// <param name="size">Square size</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawSquare(Vector3 position, float size = 0.2f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_square", position, size, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw a sequence of points using billboard squares
	/// </summary>
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="size">Size of squares</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawPoints(Vector3[] points, float size = 0.25f, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_points", points, size, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw 3 intersecting lines with the given transformations
	/// </summary>
	/// <param name="transform">Transform of lines</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawPosition(Transform transform, Color? color = null, float duration = 0f){
		debug_draw_3d?.Call("draw_position", transform, color == null? empty_color : color, duration);
	}

	/// <summary>
	/// Draw 3 lines with the given transformations and arrows at the ends
	/// </summary>
	/// <param name="transform">Transform of lines</param>
	/// <param name="color">Color</param>
	/// <param name="is_centered">If 'true', then the lines will intersect in the center of the transform</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawGizmo(Transform transform, Color? color = null, bool is_centered = false, float duration = 0f){
		debug_draw_3d?.Call("draw_gizmo", transform, color == null? empty_color : color, is_centered, duration);
	}

	/// <summary>
	/// Draw simple grid with given size and subdivision
	/// </summary>
	/// <param name="origin">Grid origin</param>
	/// <param name="x_size">Direction and size of the X side. As an axis in the Basis.</param>
	/// <param name="y_size">Direction and size of the Y side. As an axis in the Basis.</param>
	/// <param name="subdivision">Number of cells for the X and Y axes</param>
	/// <param name="color">Lines color</param>
	/// <param name="is_centered">Draw lines relative to origin</param>
	/// <param name="duration">Duration of existence in seconds</param>
	public static void DrawGrid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2 subdivision, Color? color = null, bool is_centered = true, float duration = 0f){
		debug_draw_3d?.Call("draw_grid", origin, x_size, y_size, subdivision, color == null? empty_color : color, is_centered, duration);
	}

	/// <summary>
	/// Draw simple grid with given size and subdivision
	/// </summary>
	/// <param name="transform">Transform of the Grid</param>
	/// <param name="subdivision">Number of cells for the X and Y axes</param>
	/// <param name="color">Lines color</param>
	/// <param name="is_centered">Draw lines relative to origin</param>
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
	/// <param name="group_title">Group title and ID</param>
	/// <param name="group_priority">Group priority</param>
	/// <param name="group_color">Group color</param>
	/// <param name="show_title">Whether to show the title</param>
	public static void BeginTextGroup(string group_title, int group_priority = 0, Color? group_color = null, bool show_title = true){
		debug_draw_3d?.Call("begin_text_group", group_title, group_priority, group_color == null? empty_color : group_color, show_title);
	}

	/// <summary>
	/// End text group. Should be called after 'begin_text_group' if you don't need more than one group.
	/// If you need to create 2+ groups just call again 'begin_text_group' and this function in the end.
	/// </summary>
	public static void EndTextGroup(){
		debug_draw_3d?.Call("end_text_group");
	}

	/// <summary>
	/// Add or update text in overlay
	/// </summary>
	/// <param name="key">The name of the field, if there is a 'value', otherwise the whole string will be equal to the 'key'</param>
	/// <param name="value">Value of field</param>
	/// <param name="priority">Priority of this line. Lower value is higher position.</param>
	/// <param name="color_of_value">Value color</param>
	/// <param name="duration">Expiration time</param>
	public static void SetText(string key, object value = null, int priority = 0, Color? color_of_value = null, float duration = -1f){
		debug_draw_3d?.Call("set_text", key, value, priority, color_of_value == null? empty_color : color_of_value, duration);
	}

	/// <summary>
	/// Create new graph with custom data.
	/// To get more information about the returned class, please see the documentation.
	/// </summary>
	/// <param name="title">Title of the graph</param>
	public static GraphParameters CreateGraph(string title){
		if (debug_draw_3d != null){
			return (GraphParameters)(Reference)debug_draw_3d.Call("create_graph", title);
		}
		else{
			return default;
		}
	}

	/// <summary>
	/// Create new graph with custom data.
	/// To get more information about the returned class, please see the documentation.
	/// </summary>
	/// <param name="title">Title of the graph</param>
	public static GraphParameters CreateFpsGraph(string title){
		if (debug_draw_3d != null){
			return (GraphParameters)(Reference)debug_draw_3d.Call("create_fps_graph", title);
		}
		else{
			return default;
		}
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
	public static GraphParameters GetGraphConfig(string title){
		if (debug_draw_3d != null){
			return (GraphParameters)(Reference)debug_draw_3d.Call("get_graph_config", title);
		}
		else{
			return default;
		}
	}

	/// <summary>
	/// Get all graph names
	/// </summary>
	public static string[] GetGraphNames(){
		if (debug_draw_3d != null){
			return (string[])debug_draw_3d.Call("get_graph_names");
		}
		else{
			return new string[0];
		}
	}

#else
#pragma warning disable IDE0060 // Remove unused parameter
	public static Dictionary GetRenderStats() => new Dictionary();
	public static void Clear3dObjects() {}
	public static void Clear2dObjects() {}
	public static void ClearAll() {}
	public static void DrawSphere(Vector3 position, float radius = 0.5f, Color? color = null, float duration = 0f) {}
	public static void DrawSphereXf(Transform transform, Color? color = null, float duration = 0f) {}
	public static void DrawSphereHd(Vector3 position, float radius = 0.5f, Color? color = null, float duration = 0f) {}
	public static void DrawSphereHdXf(Transform transform, Color? color = null, float duration = 0f) {}
	public static void DrawCylinder(Transform transform, Color? color = null, float duration = 0f) {}
	public static void DrawBox(Vector3 position, Vector3 size, Color? color = null, bool is_box_centered = false, float duration = 0f) {}
	public static void DrawBoxXf(Transform transform, Color? color = null, bool is_box_centered = true, float duration = 0f) {}
	public static void DrawAabb(AABB aabb, Color? color = null, float duration = 0f) {}
	public static void DrawAabbAb(Vector3 a, Vector3 b, Color? color = null, float duration = 0f) {}
	public static void DrawLineHit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f) {}
	public static void DrawLineHitOffset(Vector3 start, Vector3 end, bool is_hit, float unit_offset_of_hit = 0.5f, float hit_size = 0.25f, Color? hit_color = null, Color? after_hit_color = null, float duration = 0f) {}
	public static void DrawLine(Vector3 a, Vector3 b, Color? color = null, float duration = 0f) {}
	public static void DrawLines(Vector3[] lines, Color? color = null, float duration = 0f) {}
	public static void DrawRay(Vector3 origin, Vector3 direction, float length, Color? color = null, float duration = 0f) {}
	public static void DrawLinePath(Vector3[] path, Color? color = null, float duration = 0f) {}
	public static void DrawArrow(Transform transform, Color? color = null, float duration = 0f) {}
	public static void DrawArrowLine(Vector3 a, Vector3 b, Color? color = null, float arrow_size = 0.5f, bool absolute_size = false, float duration = 0f) {}
	public static void DrawArrowRay(Vector3 origin, Vector3 direction, float length, Color? color = null, float arrow_size = 0.5f, bool absolute_size = false, float duration = 0f) {}
	public static void DrawArrowPath(Vector3[] path, Color? color = null, float arrow_size = 0.75f, bool absolute_size = true, float duration = 0f) {}
	public static void DrawPointPath(Vector3[] path, float size = 0.25f, Color? points_color = null, Color? lines_color = null, float duration = 0f) {}
	public static void DrawSquare(Vector3 position, float size = 0.2f, Color? color = null, float duration = 0f) {}
	public static void DrawPoints(Vector3[] points, float size = 0.25f, Color? color = null, float duration = 0f) {}
	public static void DrawPosition(Transform transform, Color? color = null, float duration = 0f) {}
	public static void DrawGizmo(Transform transform, Color? color = null, bool is_centered = false, float duration = 0f) {}
	public static void DrawGrid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2 subdivision, Color? color = null, bool is_centered = true, float duration = 0f) {}
	public static void DrawGridXf(Transform transform, Vector2 subdivision, Color? color = null, bool is_centered = true, float duration = 0f) {}
	public static void DrawCameraFrustum(Camera camera, Color? color = null, float duration = 0f) {}
	public static void DrawCameraFrustumPlanes(Array camera_frustum, Color? color = null, float duration = 0f) {}
	public static void BeginTextGroup(string group_title, int group_priority = 0, Color? group_color = null, bool show_title = true) {}
	public static void EndTextGroup() {}
	public static void SetText(string key, object value = null, int priority = 0, Color? color_of_value = null, float duration = -1f) {}
	public static GraphParameters CreateGraph(string title) => default;
	public static GraphParameters CreateFpsGraph(string title) => default;
	public static void GraphUpdateData(string title, float data) {}
	public static void RemoveGraph(string title) {}
	public static void ClearGraphs() {}
	public static GraphParameters GetGraphConfig(string title) => default;
	public static string[] GetGraphNames() => new string[0];
#pragma warning restore IDE0060 // Remove unused parameter
#endif
#endregion Draw Functions


	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////


	public class GraphParameters
	{
#if DEBUG_DRAW_3D_IMPLEMENTATION
		readonly Reference orig_ref;
		public GraphParameters(Reference reference) { orig_ref = reference; }
		public static explicit operator GraphParameters(Reference reference) => reference != null ? new GraphParameters(reference) : null;

#else
#pragma warning disable IDE0060 // Remove unused parameter
		public GraphParameters(Reference reference) { }
#pragma warning restore IDE0060 // Remove unused parameter
#endif


#region Parameters
#if DEBUG_DRAW_3D_IMPLEMENTATION
		/// <summary>
		/// Is Graph enabled
		/// </summary>
		public bool Enabled { get => orig_ref != null ? (bool)orig_ref.Get("enabled") : default; set => orig_ref?.Set("enabled", value); }

		/// <summary>
		/// Draw Graph title
		/// </summary>
		public bool ShowTitle { get => orig_ref != null ? (bool)orig_ref.Get("show_title") : default; set => orig_ref?.Set("show_title", value); }

		/// <summary>
		/// Switch between frame time and FPS modes
		/// Only for FPS Graphs
		/// </summary>
		public bool FrametimeMode { get => orig_ref != null ? (bool)orig_ref.Get("frametime_mode") : default; set => orig_ref?.Set("frametime_mode", value); }

		/// <summary>
		/// Draw a graph line aligned vertically in the center
		/// </summary>
		public bool CenteredGraphLine { get => orig_ref != null ? (bool)orig_ref.Get("centered_graph_line") : default; set => orig_ref?.Set("centered_graph_line", value); }

		/// <summary>
		/// Sets the text visibility *GraphTextFlags*
		/// </summary>
		public int ShowTextFlags { get => orig_ref != null ? (int)orig_ref.Get("show_text_flags") : default; set => orig_ref?.Set("show_text_flags", value); }

		/// <summary>
		/// The size of the graph.
		/// </summary>
		public Vector2 Size { get => orig_ref != null ? (Vector2)orig_ref.Get("size") : default; set => orig_ref?.Set("size", value); }

		/// <summary>
		/// The size of the buffer where the values are stored.
		/// </summary>
		public int BufferSize { get => orig_ref != null ? (int)orig_ref.Get("buffer_size") : default; set => orig_ref?.Set("buffer_size", value); }

		/// <summary>
		/// Offset from the corner selected in position
		/// </summary>
		public Vector2 Offset { get => orig_ref != null ? (Vector2)orig_ref.Get("offset") : default; set => orig_ref?.Set("offset", value); }

		/// <summary>
		/// FPS Graph position *BlockPosition*
		/// </summary>
		public int Position { get => orig_ref != null ? (int)orig_ref.Get("position") : default; set => orig_ref?.Set("position", value); }

		/// <summary>
		/// Graph line color
		/// </summary>
		public Color LineColor { get => orig_ref != null ? (Color)orig_ref.Get("line_color") : default; set => orig_ref?.Set("line_color", value); }

		/// <summary>
		/// Color of the info text
		/// </summary>
		public Color TextColor { get => orig_ref != null ? (Color)orig_ref.Get("text_color") : default; set => orig_ref?.Set("text_color", value); }

		/// <summary>
		/// Background color
		/// </summary>
		public Color BackgroundColor { get => orig_ref != null ? (Color)orig_ref.Get("background_color") : default; set => orig_ref?.Set("background_color", value); }

		/// <summary>
		/// Border color
		/// </summary>
		public Color BorderColor { get => orig_ref != null ? (Color)orig_ref.Get("border_color") : default; set => orig_ref?.Set("border_color", value); }

		/// <summary>
		/// Border color
		/// </summary>
		public string TextSuffix { get => orig_ref != null ? (string)orig_ref.Get("text_suffix") : ""; set => orig_ref?.Set("text_suffix", value); }

		/// <summary>
		/// Custom Font
		/// </summary>
		public Font CustomFont { get => orig_ref != null ? (Font)orig_ref.Get("custom_font") : default; set => orig_ref?.Set("custom_font", value); }

#else
		public bool Enabled = default;
		public bool ShowTitle = default;
		public bool FrametimeMode = default;
		public bool CenteredGraphLine = default;
		public int ShowTextFlags = default;
		public Vector2 Size = default;
		public int BufferSize = default;
		public Vector2 Offset = default;
		public int Position = default;
		public Color LineColor = default;
		public Color TextColor = default;
		public Color BackgroundColor = default;
		public Color BorderColor = default;
		public string TextSuffix = "";
		public Font CustomFont = default;
#endif
#endregion Parameters
	}
}

