#pragma once

#define DD3D_MAJOR 1
#define DD3D_MINOR 4
#define DD3D_PATCH 4
#define DD3D_VERSION ((DD3D_MAJOR << (8 * 3)) + (DD3D_MINOR << (8 * 2)) + (DD3D_PATCH << (8 * 1)))

#define _DD3D_VERSION_STR_TEXT(text) #text
#define _DD3D_VERSION_STR(major, minor, patch) _DD3D_VERSION_STR_TEXT(major) "." _DD3D_VERSION_STR_TEXT(minor) "." _DD3D_VERSION_STR_TEXT(patch)
#define DD3D_VERSION_STR _DD3D_VERSION_STR(DD3D_MAJOR, DD3D_MINOR, DD3D_PATCH)
