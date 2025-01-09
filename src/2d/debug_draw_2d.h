#pragma once

#include "common/colors.h"
#include "utils/compiler.h"

#include <memory>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/font.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDrawManager;
class DebugDraw2DConfig;
class DebugDraw2DStats;
class GroupedText;

/**
 * @brief
 * Singleton class for calling debugging 2D methods.
 *
 * Currently, this class supports drawing an overlay with text.
 */
class DebugDraw2D : public Object {
	GDCLASS(DebugDraw2D, Object)

	friend DebugDrawManager;
	static DebugDraw2D *singleton;

private:
	DebugDrawManager *root_node = nullptr;

	// 2d
	static constexpr const char *s_marked_dirty = "marked_dirty";

	bool _is_drawing_frame = true;
	bool _canvas_need_update = true;
	Ref<Font> _font;
	Ref<DebugDraw2DStats> stats_2d;

	Callable call_canvas_item_draw_cache;
	uint64_t default_control_id = 0;

#ifndef DISABLE_DEBUG_RENDERING
	std::unique_ptr<GroupedText> grouped_text;
#endif

#ifndef DISABLE_DEBUG_RENDERING
	void _finish_frame_and_update();
	void _clear_all_internal();
	void _set_custom_canvas_internal(Control *_canvas);
#endif

	void _on_canvas_item_draw(Control *ci);
	inline bool _is_enabled_override() const;

	void process_start(double delta);
	void process_end(double delta);
	void physics_process_start(double delta);
	void physics_process_end(double delta);

#pragma region Exposed Parameter Values

	// GENERAL

	bool debug_enabled = true;
	uint64_t custom_control_id = 0;

	Ref<DebugDraw2DConfig> config;

#pragma endregion // Exposed Parameter Values

protected:
	/// @private
	static void _bind_methods();

public:
	DebugDraw2D();
	~DebugDraw2D();

	/// @private
	void init(DebugDrawManager *root);

	/**
	 * Get singleton. Not available in GDScript.
	 */
	static DebugDraw2D *get_singleton() {
		return singleton;
	};
	/// @private
	void mark_canvas_dirty();
	/// @private
	bool is_drawing_frame() const;

	/// @private
	Node *get_root_node();

#pragma region Exposed Parameters
	/// @private
	void set_empty_color(const Color &_col);
	/**
	 * Get the color that is used as the default parameter for `draw_*` calls.
	 */
	Color get_empty_color() const;

	/**
	 * Set whether debug drawing works or not.
	 */
	void set_debug_enabled(const bool &_state);
	bool is_debug_enabled() const;

	/**
	 * Set the configuration global for everything in DebugDraw2D.
	 */
	void set_config(Ref<DebugDraw2DConfig> _cfg);
	/**
	 * Get the DebugDraw2DConfig.
	 */
	Ref<DebugDraw2DConfig> get_config() const;

	/**
	 * Set a custom Control to be used as the canvas for drawing the graphic.
	 *
	 * You can use any Control, even one that is in a different window.
	 */
	void set_custom_canvas(Control *_canvas);
	Control *get_custom_canvas() const;
#pragma endregion // Exposed Parametes

#pragma region Exposed Draw Functions

	/**
	 * Returns the DebugDraw2DStats instance with the current statistics.
	 *
	 * Some data can be delayed by 1 frame.
	 */
	Ref<DebugDraw2DStats> get_render_stats();

	/**
	 * Clear all 2D objects
	 */
	void clear_all();

#pragma region Text
	/**
	 * Begin a text group to which all of the following text from DebugDraw2D.set_text will be added
	 *
	 * @param group_title Group title and ID
	 * @param group_priority Group priority based on which groups will be sorted from top to bottom.
	 * @param group_color Main color of the group
	 * @param show_title Whether to show the title
	 * @param title_size Title font size
	 * @param text_size Text font size
	 */
	void begin_text_group(String group_title, int group_priority = 0, Color group_color = Colors::white_smoke, bool show_title = true, int title_size = 14, int text_size = 12);
	/**
	 * Ends the text group. Should be called after DebugDraw2D.begin_text_group.
	 *
	 * If you need to create multiple groups, just call DebugDraw2D.begin_text_group again and this function at the end.
	 */
	void end_text_group();
	/**
	 * Add or update text in an overlay
	 *
	 * @param key Left value if 'value' is set, otherwise the entire string is 'key'
	 * @param value Value of field
	 * @param priority Priority of this line. Lower value is higher position
	 * @param color_of_value Value color
	 * @param duration Expiration time
	 */
	void set_text(String key, Variant value = Variant(), int priority = 0, Color color_of_value = Colors::empty_color, real_t duration = -1);

	/**
	 * Clear all text
	 */
	void clear_texts();
#pragma endregion // Text
#pragma endregion // Exposed Draw Functions
};
