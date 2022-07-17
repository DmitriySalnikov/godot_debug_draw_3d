# Application.mk
APP_STL := c++_static

APP_ABI := all
#APP_ABI := arm64-v8a

APP_DEBUG := true

APP_CFLAGS := \
-DGDNATIVE_LIBRARY \
-DNO_EDITOR