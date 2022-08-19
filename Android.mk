# Android.mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := godot-cpp
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := godot-cpp/bin/libgodot-cpp.android.release.x86.a
endif
ifeq ($(TARGET_ARCH_ABI),x86_64)
    LOCAL_SRC_FILES := godot-cpp/bin/libgodot-cpp.android.release.x86_64.a
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := godot-cpp/bin/libgodot-cpp.android.release.armv7.a
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
    LOCAL_SRC_FILES := godot-cpp/bin/libgodot-cpp.android.release.arm64v8.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := debug_draw
LOCAL_CPPFLAGS := -std=c++14 -O3
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_LDLIBS := -llog 
#LOCAL_CFLAGS := -D_DEF

LOCAL_SRC_FILES := \
debug_draw_3d/colors.cpp \
debug_draw_3d/data_graphs.cpp \
debug_draw_3d/debug_draw.cpp \
debug_draw_3d/debug_geometry_container.cpp \
debug_draw_3d/editor_plugin.cpp \
debug_draw_3d/geometry_generators.cpp \
debug_draw_3d/grouped_text.cpp \
debug_draw_3d/math_utils.cpp \
debug_draw_3d/register_types.cpp \
debug_draw_3d/render_instances.cpp \
debug_draw_3d/utils.cpp

LOCAL_C_INCLUDES := \
godot-cpp/godot-headers \
godot-cpp/include/ \
godot-cpp/include/core \
godot-cpp/include/gen \

LOCAL_STATIC_LIBRARIES := godot-cpp

include $(BUILD_SHARED_LIBRARY)