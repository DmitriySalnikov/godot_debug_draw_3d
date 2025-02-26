#pragma once
#ifndef DISABLE_DEBUG_RENDERING

#include "config_scope_3d.h"
#include "render_instances_enums.h"
#include "utils/utils.h"

#include <functional>
#include <list>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/label3d.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDraw3DStats;

class NodesContainer {
	friend class DebugDraw3D;
	class DebugDraw3D *owner;
	Node *root;

	bool no_depth_test = false;

	struct DelayedNode {
		double expiration_time;
		bool is_used_one_time;

		DelayedNode() :
				expiration_time(-1000),
				is_used_one_time(true) {}

		_FORCE_INLINE_ bool is_expired() const {
			return expiration_time < 0 ? is_used_one_time : false;
		}

		_FORCE_INLINE_ bool is_expired_physics() const {
			return expiration_time <= 0;
		}

		_FORCE_INLINE_ void update_expiration(const double &p_delta) {
			expiration_time -= p_delta;
			is_used_one_time = true;
		}
	};

	struct TextNodeItem : public DelayedNode {
		Label3D *node;
		uint32_t opts_hash;
		uint32_t text_hash;
		double unused_time;

		TextNodeItem() :
				DelayedNode(),
				node(nullptr),
				opts_hash(0),
				text_hash(0),
				unused_time(-1000) {
			DEV_PRINT_STD("New " NAMEOF(TextNodeItem) " created\n");
		};

		TextNodeItem(Label3D *lbl, uint32_t opts_hash, uint32_t text_hash) :
				DelayedNode(),
				node(lbl),
				opts_hash(opts_hash),
				text_hash(text_hash),
				unused_time(-1000) {
			DEV_PRINT_STD("New " NAMEOF(TextNodeItem) " created\n");
		};
	};

	enum ShrinkTimers : char {
		TIME_UNUSED_LABEL3D_MOVE = 1,
		TIME_UNUSED_LABEL3D_DELETE = 5,
	};

	struct LabelsPool {
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::list<TextNodeItem> > > recent;
		std::list<TextNodeItem> unused;
		std::list<TextNodeItem> used;
		NodesContainer *owner;

		size_t nodes_count = 0;
		size_t used_count = 0;

	public:
		TextNodeItem *get(uint32_t opts_hash, uint32_t text_hash);
		void update_unused(double delta, bool is_physics);
		void clear_pools();

		_FORCE_INLINE_ void for_each(std::function<void(TextNodeItem &)> func) {
			for (auto &r : recent) {
				for (auto &rt : r.second) {
					for (auto &i : rt.second) {
						func(i);
					}
				}
			}
			for (auto &i : unused) {
				func(i);
			}
			for (auto &i : used) {
				func(i);
			}
		}
	};

	int32_t render_layers = 1;
	double process_delta_sum = 0;
	double physics_delta_sum = 0;
	bool is_frame_rendered = false;

	TextNodeItem _create_text_node_item(uint32_t opts_hash, uint32_t text_hash);
	void _destroy_text_node_item(TextNodeItem &item);
	LabelsPool text_pools[(int)ProcessType::MAX];

public:
	NodesContainer(class DebugDraw3D *p_owner, Node *p_root, bool p_no_depth_test);
	~NodesContainer();

	void update_geometry(double p_delta);
	void update_geometry_physics_start(double p_delta);
	void update_geometry_physics_end(double p_delta);

	void update_expiration_delta(const double &p_delta, const ProcessType &p_proc);
	void update_unused(const double &p_delta, const ProcessType &p_proc = ProcessType::MAX);

	void set_render_layer_mask(int32_t p_layers);
	int32_t get_render_layer_mask() const;

	void add_or_update_text(const DebugDraw3DScopeConfig::Data *p_cfg, const Vector3 &position, const String text, int size, const Color &color, const real_t &duration);

	void get_render_stats(Ref<DebugDraw3DStats> &p_stats) const;
};

#endif