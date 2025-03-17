#include "nodes_container.h"

#ifndef DISABLE_DEBUG_RENDERING
#include "config_3d.h"
#include "debug_draw_3d.h"
#include "stats_3d.h"

#include <array>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/templates/hashfuncs.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

NodesContainer::TextNodeItem *NodesContainer::LabelsPool::get(uint32_t opts_hash, uint32_t text_hash) {
	ZoneScoped;

	if (auto &r = recent[opts_hash][text_hash]; !r.empty()) {
		ZoneScopedN("Returning first recent node.");
		used.splice(used.begin(), r, r.begin());
		return &used.front();
	}

	// if no recent same
	if (!unused.empty()) {
		ZoneScopedN("Returning first unused node.");
		used.splice(used.begin(), unused, unused.begin());
		return &used.front();
	}

	{
		ZoneScopedN("Creating");

		// int to_create = Math::clamp((int)delayed.size(), 2, 1024);
		int to_create = 2;
		for (int i = 0; i < to_create; i++) {
			unused.push_front(owner->_create_text_node_item(opts_hash, text_hash));
		}
		used.splice(used.begin(), unused, unused.begin());
	}
	return &used.front();
}

void NodesContainer::LabelsPool::update_unused(double delta, bool is_physics) {
	ZoneScoped;

	nodes_count = 0;
	{
		ZoneScopedN("Delete nodes");
		size_t destroyed = 0;

		size_t idx = 0;
		for (auto it = unused.begin(); it != unused.end();) {
			if (it->unused_time < 0) {
				// Save at least 32 (+1) nodes
				if (idx > 32) {
					DEV_PRINT_STD("Destroyed %s\n", it->node->get_text().utf8().ptr());

					owner->_destroy_text_node_item(*it);
					it = unused.erase(it);
					destroyed++;
				} else {
					++it;
				}
			} else {
				it->unused_time -= delta;
				nodes_count++;
				++it;
			}
			idx++;
		}

		if (destroyed) {
			DEV_PRINT_STD("Destroyed unused Label3D nodes: %" PRIu64 ". %" PRIu64 " nodes remain.\n", destroyed, unused.size());
		}
	}

	{
		ZoneScopedN("Move to unused");

		size_t moved = 0;

		for (auto &r : recent) {
			for (auto &rt : r.second) {
				for (auto it = rt.second.begin(); it != rt.second.end();) {
					if (it->unused_time < 0) {
						it->unused_time = TIME_UNUSED_LABEL3D_DELETE;

						auto next = std::next(it);
						unused.splice(unused.begin(), rt.second, it);
						// DEV_PRINT_STD("Moved %s\n", it->node->get_text().utf8().ptr());
						it = next;
						moved++;
					} else {
						it->unused_time -= delta;
						++it;
					}

					nodes_count++;
				}
			}
		}

		if (moved) {
			// DEV_PRINT_STD("Moved Label3D nodes to unused: %" PRIu64 ".\n", moved);
		}
	}

	{
		ZoneScopedN("Move to recent");

		used_count = 0;
		for (auto it = used.begin(); it != used.end();) {
			if (is_physics ? it->is_expired_physics() : it->is_expired()) {
				ZoneScopedN("Hide");
				it->node->set_visible(false);

				auto &rt = recent[it->opts_hash][it->text_hash];
				auto next = std::next(it);
				rt.splice(rt.begin(), used, it);
				it = next;
			} else {
				used_count++;
				it->update_expiration(delta);
				++it;
			}

			nodes_count++;
		}
	}

	{
		ZoneScopedN("Clear empty lists");

		std::vector<uint32_t> keys_to_remove;
		bool is_something_cleared = false;

		// Clear empty lists for the String hashes
		for (auto &r : recent) { // each options group
			for (auto &rt : r.second) { // each text hash
				if (rt.second.empty()) {
					keys_to_remove.push_back(rt.first);
				}
			}

			if (!keys_to_remove.empty()) {
				for (auto &k : keys_to_remove) {
					r.second.erase(k);
				}

				is_something_cleared = true;
				// DEV_PRINT_STD("Removed TextNodeItems lists: %" PRIu64 ".\n", keys_to_remove.size());
			}
			keys_to_remove.clear();
		}

		// Clear option groups
		if (is_something_cleared) {
			for (auto &r : recent) {
				if (r.second.empty()) {
					keys_to_remove.push_back(r.first);
				}
			}

			if (!keys_to_remove.empty()) {
				for (auto &k : keys_to_remove) {
					recent.erase(k);
				}
				// DEV_PRINT_STD("Removed TextNodeItems option groups: %" PRIu64 ".\n", keys_to_remove.size());
			}
			keys_to_remove.clear();
		}
	}
}

void NodesContainer::LabelsPool::clear_pools() {
	ZoneScoped;
	for_each([&](TextNodeItem &i) {
		owner->_destroy_text_node_item(i);
	});

	recent.clear();
	unused.clear();
	used.clear();

	nodes_count = 0;
	used_count = 0;
}

NodesContainer::TextNodeItem NodesContainer::_create_text_node_item(uint32_t opts_hash, uint32_t text_hash) {
	ZoneScoped;
	Label3D *lbl = memnew(Label3D);
	lbl->set_layer_mask(render_layers);
	lbl->set_visible(false);
	lbl->set_draw_flag(Label3D::DrawFlags::FLAG_DISABLE_DEPTH_TEST, no_depth_test);
	lbl->set_billboard_mode(BaseMaterial3D::BillboardMode::BILLBOARD_ENABLED);
	root->add_child(lbl);
	return TextNodeItem(lbl, opts_hash, text_hash);
}

void NodesContainer::_destroy_text_node_item(TextNodeItem &item) {
	ZoneScoped;
	root->remove_child(item.node);
	item.node->queue_free();
	DEV_PRINT_STD(NAMEOF(TextNodeItem) "'s Label3D node is destroyed\n");
}

NodesContainer::NodesContainer(DebugDraw3D *p_owner, Node *p_root, bool p_no_depth_test) {
	for (auto &p : text_pools) {
		p.owner = this;
	}

	owner = p_owner;
	root = p_root;
	no_depth_test = p_no_depth_test;
}

NodesContainer::~NodesContainer() {
	LOCK_GUARD(owner->datalock);
	for (auto &p : text_pools) {
		p.clear_pools();
	}
}

void NodesContainer::update_expiration_delta(const double &p_delta, const ProcessType &p_proc) {
	ZoneScoped;

	if (p_proc == ProcessType::PHYSICS_PROCESS) {
		physics_delta_sum += p_delta;
	} else {
		process_delta_sum += p_delta;
	}
}

void NodesContainer::update_geometry(double p_delta) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);

	_render_queued_nodes();

	// accumulate a time delta to delete objects in any case after their timers expire.
	update_expiration_delta(p_delta, ProcessType::PROCESS);

	// Do not update geometry if frozen
	if (owner->get_config()->is_freeze_3d_render())
		return;

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		ZoneScopedN("Reset Label3Ds");
		update_unused(p_delta);
		return;
	}

	// Update render layers
	if (render_layers != owner->get_config()->get_geometry_render_layers()) {
		set_render_layer_mask(owner->get_config()->get_geometry_render_layers());
	}

	update_unused(p_delta, ProcessType::PROCESS);

	is_frame_rendered = true;
}

void NodesContainer::update_geometry_physics_start(double p_delta) {
	ZoneScoped;
	if (is_frame_rendered) {
		update_unused(physics_delta_sum, ProcessType::PHYSICS_PROCESS);
		is_frame_rendered = false;
		physics_delta_sum = 0;
	}
}

void NodesContainer::update_geometry_physics_end(double p_delta) {
	ZoneScoped;
	physics_delta_sum += p_delta;
}

void NodesContainer::update_unused(const double &p_delta, const ProcessType &p_proc) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	if (p_proc == ProcessType::MAX) {
		for (int p = 0; p < (int)ProcessType::MAX; p++) {
			text_pools[p].update_unused(p_delta, p == (int)ProcessType::PHYSICS_PROCESS);
		}
	} else {
		text_pools[(int)p_proc].update_unused(p_delta, p_proc == ProcessType::PHYSICS_PROCESS);
	}
}

void NodesContainer::set_render_layer_mask(int32_t p_layers) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	if (render_layers != p_layers) {
		for (auto &proc : text_pools) {
			proc.for_each([&p_layers](TextNodeItem &i) {
				i.node->set_layer_mask(p_layers);
			});
		}

		render_layers = p_layers;
	}
}

int32_t NodesContainer::get_render_layer_mask() const {
	return render_layers;
}

void NodesContainer::add_or_update_text(const DebugDraw3DScopeConfig::Data *p_cfg, const Vector3 &position, const String text, int size, const Color &color, const real_t &duration) {
	ZoneScoped;

	uint32_t opts_hash = hash_murmur3_one_32(size);
	opts_hash = hash_murmur3_one_64((uint64_t)p_cfg->text_font.ptr(), opts_hash);
	opts_hash = hash_murmur3_one_float(color.r, opts_hash);
	opts_hash = hash_murmur3_one_float(color.g, opts_hash);
	opts_hash = hash_murmur3_one_float(color.b, opts_hash);
	opts_hash = hash_murmur3_one_float(color.a, opts_hash);
	opts_hash = hash_murmur3_one_32(p_cfg->text_outline_color_hash, opts_hash);
	opts_hash = hash_murmur3_one_32((uint32_t)p_cfg->text_outline_size, opts_hash);

	uint32_t text_hash = text.hash();

	// if called from the main thread, just add/update
	if (auto *os = OS::get_singleton(); os->get_thread_caller_id() == os->get_main_thread_id()) {
		auto *lblit = text_pools[(int)(Engine::get_singleton()->is_in_physics_frame() ? ProcessType::PHYSICS_PROCESS : ProcessType::PROCESS)].get(opts_hash, text_hash);

		lblit->expiration_time = duration;
		lblit->is_used_one_time = false;
		lblit->opts_hash = opts_hash;
		lblit->text_hash = text_hash;
		lblit->unused_time = Math::clamp((double)duration * 2, (double)1, DBL_MAX);

		Label3D *lbl3d = lblit->node;
		lbl3d->set_position(position);
		lbl3d->set_visible(true);
		lbl3d->set_text(text);
		lbl3d->set_font(p_cfg->text_font);
		lbl3d->set_font_size(size);
		lbl3d->set_modulate(color);
		lbl3d->set_outline_modulate(p_cfg->text_outline_color);
		lbl3d->set_outline_size(p_cfg->text_outline_size);
	} else {
		// wait for the end of the frame to display anything from the user thread
		deferred_text_queue.emplace(
				/* real_t duration */ duration,
				/* uint64_t opts_hash */ opts_hash,
				/* uint64_t text_hash */ text_hash,
				/* Vector3 position */ position,
				/* String text */ text,
				/* Ref<Font> font */ p_cfg->text_font,
				/* int font_size */ size,
				/* Color color */ color,
				/* Color outline_color */ p_cfg->text_outline_color,
				/* int outline_size */ p_cfg->text_outline_size);
	}
}

void NodesContainer::_render_queued_nodes() {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);

	while (!deferred_text_queue.empty()) {
		ZoneScopedN("Updating item");
		const auto &i = deferred_text_queue.front();

		// Use ProcessType::PROCESS as the default value for user threads
		auto *lblit = text_pools[(int)ProcessType::PROCESS].get(i.opts_hash, i.text_hash);

		lblit->expiration_time = i.duration;
		lblit->is_used_one_time = false;
		lblit->opts_hash = i.opts_hash;
		lblit->text_hash = i.text_hash;
		lblit->unused_time = Math::clamp((double)i.duration * 2, (double)1, DBL_MAX);

		{
			ZoneScopedN("Setting node properties");
			Label3D *lbl3d = lblit->node;
			lbl3d->set_position(i.position);
			lbl3d->set_visible(true);
			lbl3d->set_text(i.text);
			lbl3d->set_font(i.font);
			lbl3d->set_font_size(i.font_size);
			lbl3d->set_modulate(i.color);
			lbl3d->set_outline_modulate(i.outline_color);
			lbl3d->set_outline_size(i.outline_size);
		}

		deferred_text_queue.pop();
	}
}

void NodesContainer::get_render_stats(Ref<DebugDraw3DStats> &p_stats) const {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);

	const int p = (int)ProcessType::PROCESS;
	const int py = (int)ProcessType::PHYSICS_PROCESS;

	p_stats->set_nodes_stats(
			/* p_nodes_label3d_visible */ text_pools[p].used_count,
			/* p_nodes_label3d_visible_physics */ text_pools[py].used_count,
			/* p_nodes_label3d_exists */ text_pools[p].nodes_count,
			/* p_nodes_label3d_exists_physics */ text_pools[py].nodes_count);
}

#endif