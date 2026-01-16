#ifdef TOOLS_ENABLED

#include "editor_menu_extensions.h"
#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include "gen/editor_resources.gen.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/callback_tweener.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/link_button.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/window.hpp>
GODOT_WARNING_RESTORE()

void DebugDrawMenuExtensionPlugin::_on_copy_clicked(const String copy_string, const uint64_t root_id) {
	DisplayServer::get_singleton()->clipboard_set(copy_string);

	PanelContainer *root = Object::cast_to<PanelContainer>(ObjectDB::get_instance(root_id));
	auto *lbl = memnew(Label);
	lbl->set_text("Copied!");
	lbl->set_h_size_flags(Control::SIZE_SHRINK_CENTER);
	lbl->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	lbl->add_theme_color_override("font_color", Color("000000"));
	lbl->set_modulate(Color(1, 1, 1, 0));

	Ref<StyleBoxFlat> nrm;
	nrm.instantiate();
	nrm->set_bg_color(Color("bbbbbb"));
	nrm->set_corner_radius_all(16);
	nrm->set_content_margin_all(7);
	lbl->add_theme_stylebox_override("normal", nrm);

	auto tween = lbl->create_tween();
	tween->tween_property(lbl, "modulate", Color(1, 1, 1, 0.75), 0.1)->set_trans(Tween::TRANS_QUART)->set_ease(Tween::EASE_IN_OUT);
	tween->tween_property(lbl, "modulate", Color(1, 1, 1, 0), 1)->set_delay(1.5)->set_trans(Tween::TRANS_QUART)->set_ease(Tween::EASE_IN);
	tween->tween_callback(callable_mp((Node *)lbl, &Node::queue_free));

	root->add_child(lbl);
}

void DebugDrawMenuExtensionPlugin::open_about_window() {
	if (about_window) {
		_on_closing_about_window();
	}

	auto load_texture = [](auto bytes) -> Ref<ImageTexture> {
		Ref<Image> image;
		image.instantiate();
		image->load_png_from_buffer(Utils::convert_to_packed_array<PackedByteArray>(bytes));
		// Verbose mode:
		// WARNING: texture_storage.cpp:2265 - Image format RGB8 not supported by hardware, converting to RGBA8.
		image->convert(Image::FORMAT_RGBA8);
		Ref<ImageTexture> tex;
		tex.instantiate();
		tex->set_image(image);
		return tex;
	};

	auto add_button = [](String text) -> Button * {
		auto *btn = memnew(Button);
		btn->set_text(text);
		btn->set_v_size_flags(Control::SIZE_SHRINK_END | Control::SIZE_EXPAND);

		Ref<StyleBoxFlat> nrm;
		nrm.instantiate();
		nrm->set_bg_color(Color("282b35"));
		btn->add_theme_stylebox_override("normal", nrm);

		Ref<StyleBoxFlat> hvr;
		hvr.instantiate();
		hvr->set_bg_color(Color("2a2d38"));
		hvr->set_border_color(Color("634d25"));
		hvr->set_border_width_all(2);
		hvr->set_corner_radius_all(2);
		btn->add_theme_stylebox_override("hover", hvr);

		Ref<StyleBoxFlat> prs;
		prs.instantiate();
		prs->set_bg_color(Color("2a2d38"));
		prs->set_border_color(Color("f59031"));
		prs->set_border_width_all(2);
		prs->set_corner_radius_all(2);
		btn->add_theme_stylebox_override("pressed", prs);

		return btn;
	};

	auto *root = memnew(PanelContainer);
	auto *base = memnew(VBoxContainer);
	auto *title_con = memnew(HBoxContainer);
	auto *icon_tex_rect = memnew(TextureRect);

	float scale = get_editor_interface()->get_editor_scale();

	auto connect_action = [this, root](Control *ctrl, AboutAction action) {
		ctrl->connect("gui_input", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_gui_input).bind((int)action, root->get_instance_id()));
	};

	{
		auto *sep8 = memnew(Control);
		sep8->set_custom_minimum_size(Vector2(0, 8 * scale));
		base->add_child(sep8);
	}

	{
		auto icon_tex = load_texture(DD3DEditorResources::images_icon_3d_32_png);
		icon_tex_rect->set_texture(icon_tex);
		icon_tex_rect->set_expand_mode(TextureRect::EXPAND_FIT_WIDTH);
		icon_tex_rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
		auto *title = memnew(Label);
		title->set_text("Debug Draw 3D");
		title->add_theme_font_size_override("font_size", 31);

		title_con->add_child(icon_tex_rect);
		title_con->add_child(title);
		title_con->set_alignment(BoxContainer::ALIGNMENT_CENTER);

		base->add_child(title_con);
	}

	{
		auto *hsep = memnew(HSeparator);
		base->add_child(hsep);
	}

	{
		uint64_t root_id = root->get_instance_id();
		String copy_all_text = "";
		auto add_label = [root_id, base, &copy_all_text](String text) {
			auto *lbl = memnew(LinkButton);
			lbl->set_text(text);
			lbl->set_underline_mode(LinkButton::UNDERLINE_MODE_ON_HOVER);
			lbl->connect("pressed", callable_mp_static(&DebugDrawMenuExtensionPlugin::_on_copy_clicked).bind(text + " ", root_id));
			copy_all_text += text + "\n";
			base->add_child(lbl);
		};

		add_label("Library version: " DD3D_VERSION_STR);
		add_label("OS: " + OS::get_singleton()->get_name());
		add_label("OS Version: " + OS::get_singleton()->get_version());
		add_label("CPU: " + OS::get_singleton()->get_processor_name());
		add_label("Arch: " + Engine::get_singleton()->get_architecture_name());
		add_label("GPU: " + RenderingServer::get_singleton()->get_video_adapter_name());
		add_label("Renderer: " + (String)ProjectSettings::get_singleton()->get_setting("rendering/renderer/rendering_method"));
		add_label("Godot: " + (String)Engine::get_singleton()->get_version_info()["string"]);

#ifdef REAL_T_IS_DOUBLE
		String precision = "double";
		precision += ", fix_precision_enabled="
#ifdef FIX_PRECISION_ENABLED
					 "yes";
#else
					 "no";
#endif

		precision += ", shader_world_coords_enabled="
#ifdef DISABLE_SHADER_WORLD_COORDS
					 "no";
#else
					 "yes";
#endif
#else
		String precision = "single";
#endif
		add_label("Precision: " + precision);

		auto *btn = add_button("Copy all");
		btn->connect("pressed", callable_mp_static(&DebugDrawMenuExtensionPlugin::_on_copy_clicked).bind(copy_all_text, root_id));

		base->add_child(btn);
	}

	{
		auto *hsep = memnew(HSeparator);
		base->add_child(hsep);
	}

	{
		auto *title = memnew(Label);
		title->set_text("Support my work:");
		title->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
		title->add_theme_font_size_override("font_size", 21);
		base->add_child(title);

		auto *hbox = memnew(HBoxContainer);

		auto boosty_tex = load_texture(DD3DEditorResources::images_boosty_pixel_png);
		auto *boosty_tex_rect = memnew(TextureRect);
		boosty_tex_rect->set_texture(boosty_tex);
		boosty_tex_rect->set_texture_filter(CanvasItem::TEXTURE_FILTER_NEAREST);
		boosty_tex_rect->set_custom_minimum_size(Vector2(176 * scale, 64 * scale));
		boosty_tex_rect->set_expand_mode(TextureRect::EXPAND_FIT_HEIGHT_PROPORTIONAL);
		boosty_tex_rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
		boosty_tex_rect->set_h_size_flags(Control::SIZE_SHRINK_CENTER | Control::SIZE_EXPAND);
		boosty_tex_rect->set_v_size_flags(Control::SIZE_SHRINK_CENTER);
		boosty_tex_rect->set_default_cursor_shape(Control::CURSOR_POINTING_HAND);
		boosty_tex_rect->set_tooltip_text(url_boosty_page);
		connect_action(boosty_tex_rect, ABOUT_OPEN_BOOSTY);
		hbox->add_child(boosty_tex_rect);

		auto *coin_box = memnew(VBoxContainer);
		coin_box->set_alignment(BoxContainer::ALIGNMENT_CENTER);
		coin_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		hbox->add_child(coin_box);

		auto usdt_tex = load_texture(DD3DEditorResources::images_USDT_TRC20_pixel_png);
		auto *usdt_tex_rect = memnew(TextureRect);
		usdt_tex_rect->set_texture(usdt_tex);
		usdt_tex_rect->set_texture_filter(CanvasItem::TEXTURE_FILTER_NEAREST);
		usdt_tex_rect->set_custom_minimum_size(Vector2(140 * scale, 140 * scale));
		usdt_tex_rect->set_expand_mode(TextureRect::EXPAND_FIT_HEIGHT);
		usdt_tex_rect->set_stretch_mode(TextureRect::STRETCH_SCALE);
		usdt_tex_rect->set_h_size_flags(Control::SIZE_SHRINK_CENTER);
		usdt_tex_rect->set_default_cursor_shape(Control::CURSOR_IBEAM);
		usdt_tex_rect->set_tooltip_text(address_usdt);
		connect_action(usdt_tex_rect, ABOUT_COPY_USDT);

		auto *lbl = memnew(Label);
		lbl->set_text("USDT-TRC20");
		lbl->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
		lbl->add_theme_font_size_override("font_size", 18);
		coin_box->add_child(usdt_tex_rect);
		coin_box->add_child(lbl);

		base->add_child(hbox);
	}

	{
		auto *hsep = memnew(HSeparator);
		base->add_child(hsep);
	}

	{
		auto *btn = add_button("Open the project page");
		btn->connect("pressed", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_id_pressed).bind(MENU_OPEN_LIBRARY_SITE));
		base->add_child(btn);

		btn = add_button("Report an issue");
		btn->connect("pressed", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_id_pressed).bind(MENU_OPEN_REPORT_ISSUE));
		base->add_child(btn);

		btn = add_button("Open the documentation");
		btn->connect("pressed", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_id_pressed).bind(MENU_OPEN_DOCUMENTATION_SITE));
		base->add_child(btn);

		{
			auto *sep8 = memnew(Control);
			sep8->set_custom_minimum_size(Vector2(0, 4 * scale));
			base->add_child(sep8);
		}

		btn = add_button("Close");
		btn->connect("pressed", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_closing_about_window));

		base->add_child(btn);
	}

	auto *scroll = memnew(ScrollContainer);
	scroll->add_child(base);
	base->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	base->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	root->add_child(scroll);
	{
		Ref<StyleBoxFlat> style;
		style.instantiate();
		style->set_bg_color(Color("21232b"));
		style->set_content_margin_all(4);
		root->add_theme_stylebox_override("panel", style);
	}

	about_window = memnew(Window);
	about_window->set_title("About Debug Draw 3D");
	about_window->add_child(root);
	root->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	about_window->set_initial_position(Window::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
	about_window->set_exclusive(true);
	about_window->set_transient(true);
	if (Utils::is_current_godot_version_in_range(4, 5, 0)) {
		// FLAG_MINIMIZE_DISABLED = 11
		about_window->set_flag((Window::Flags)11, true);
		// FLAG_MAXIMIZE_DISABLED = 12
		about_window->set_flag((Window::Flags)12, true);
	}

	about_window->set_min_size(Vector2i((int)(333 * scale), (int)(333 * scale)));
	about_window->connect("close_requested", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_closing_about_window));
	about_window->set_size(Vector2((int)(540 * scale), (int)(732 * scale)));

	get_editor_interface()->get_base_control()->add_child(about_window);
}

void DebugDrawMenuExtensionPlugin::_on_closing_about_window() {
	if (about_window) {
		about_window->queue_free();
		about_window = nullptr;
	}
}

inline String DebugDrawMenuExtensionPlugin::_get_plugin_name() const {
	return String("Generate C# Binding for DebugDraw");
}

void DebugDrawMenuExtensionPlugin::_enter_tree() {
	ZoneScoped;
	DEV_PRINT_STD("%s _enter_tree\n", NAMEOF(DebugDrawMenuExtensionPlugin));

	PopupMenu *menu = memnew(PopupMenu);
	menu->connect("id_pressed", callable_mp(this, &DebugDrawMenuExtensionPlugin::_on_id_pressed));
	menu->add_item("About...", MENU_OPEN_ABOUT_WINDOW);
	menu->add_item("Open the project page", MENU_OPEN_LIBRARY_SITE);
	menu->add_item("Report an issue", MENU_OPEN_REPORT_ISSUE);
	menu->add_item("Open the documentation", MENU_OPEN_DOCUMENTATION_SITE);

	add_tool_submenu_item(menu_item_name, menu);

	// HACK: to change item icon
	PopupMenu *parent = cast_to<PopupMenu>(menu->get_parent());

	Ref<Image> img;
	img.instantiate();
	img->load_png_from_buffer(Utils::convert_to_packed_array<PackedByteArray>(DD3DEditorResources::images_icon_3d_32_png));
	int icon_size = parent->get_theme_constant("class_icon_size", "Editor");
	parent->add_theme_constant_override("icon_max_width", icon_size ? icon_size : 16);
	parent->set_item_icon(parent->get_item_count() - 1, ImageTexture::create_from_image(img));
}

void DebugDrawMenuExtensionPlugin::_exit_tree() {
	// print on exit == crash
	// DEV_PRINT(NAMEOF(DebugDrawMenuExtensionPlugin) " _exit_tree");

	remove_tool_menu_item(menu_item_name);
}

void DebugDrawMenuExtensionPlugin::_on_id_pressed(int p_id) {
	ZoneScoped;
	switch ((MenuItemId)p_id) {
		case MENU_OPEN_ABOUT_WINDOW: {
			open_about_window();
			break;
		}
		case MENU_OPEN_LIBRARY_SITE: {
			OS::get_singleton()->shell_open(url_project_page);
			break;
		}
		case MENU_OPEN_REPORT_ISSUE: {
			OS::get_singleton()->shell_open(url_report_issue);
			break;
		}
		case MENU_OPEN_DOCUMENTATION_SITE: {
			OS::get_singleton()->shell_open(url_docs_page);
			break;
		}
		default:
			PRINT_ERROR("DD3D: Menu item " + String::num_int64(p_id) + " not implemented.");
			break;
	}
}

void DebugDrawMenuExtensionPlugin::_on_gui_input(const Ref<godot::InputEvent> ie, int action_id, uint64_t root_id) {
	auto *iemb = cast_to<InputEventMouseButton>(ie.ptr());
	if (iemb && !iemb->is_pressed() && iemb->get_button_index() == MOUSE_BUTTON_LEFT) {
		switch ((AboutAction)action_id) {
			case ABOUT_OPEN_BOOSTY:
				OS::get_singleton()->shell_open(url_boosty_page);
				break;
			case ABOUT_COPY_USDT:
				DebugDrawMenuExtensionPlugin::_on_copy_clicked(address_usdt, root_id);
				break;
			default:
				PRINT_ERROR("DD3D: Action " + String::num_int64(action_id) + " not implemented.");
				break;
		}
	}
}

DebugDrawMenuExtensionPlugin::DebugDrawMenuExtensionPlugin() {
	DEV_PRINT_STD("%s constructor\n", NAMEOF(DebugDrawMenuExtensionPlugin));

	menu_item_name = "Debug Draw";
}

DebugDrawMenuExtensionPlugin::~DebugDrawMenuExtensionPlugin() {
	// DEV_PRINT_STD(NAMEOF(DebugDrawMenuExtensionPlugin) " deconstructor\n");
	// remove_tool_menu_item(menu_item_name);
}

#endif