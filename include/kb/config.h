#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KB_FOURCC(a, b, c, d) (((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24)))

#define KB_CONFIG_MAX_GAMEPADS                  8
#define KB_CONFIG_MAX_INDEX_BUFFERS             1024
#define KB_CONFIG_MAX_MATERIALS                 1024
#define KB_CONFIG_MAX_MESHES                    1024
#define KB_CONFIG_MAX_PROGRAMS                  1024
#define KB_CONFIG_MAX_COMMAND_BUFFERS           1024
#define KB_CONFIG_MAX_SURFACES                  1
#define KB_CONFIG_MAX_TEXTURES                  1024
#define KB_CONFIG_MAX_VERTEX_BUFFERS            1024
#define KB_CONFIG_MAX_MUSICS                    1024
#define KB_CONFIG_MAX_SOUNDS                    1024
#define KB_CONFIG_FILE_ROOT_SIZE                512
#define KB_CONFIG_MAX_NAME_SIZE                 512
#define KB_CONFIG_MAX_GIZMOS                    1024
#define KB_CONFIG_GIZMO_CACHE_SIZE              1024
#define KB_CONFIG_GIZMO_STACK_SIZE              32
#define KB_CONFIG_MAX_TEXTURE_SAMPLERS          16
#define KB_CONFIG_MAX_UNIFORM_BUFFERS           16
#define KB_CONFIG_MAX_TEXTURE_SAMPLERS          16
#define KB_CONFIG_MAX_SHADER_STRING_LEN         512
#define KB_CONFIG_MAX_DRAW_CALLS                1024
#define KB_CONFIG_MAX_FRAMES_IN_FLIGHT          4
#define KB_CONFIG_MAX_RENDER_THREADS            4
#define KB_CONFIG_MAX_VERTEX_ATTRIBS            32
#define KB_CONFIG_MAX_VERTEX_BINDINGS           32
#define KB_CONFIG_MAX_DESCRIPTOR_SETS           8
#define KB_CONFIG_MAX_DESCRIPTOR_BINDINGS       8
#define KB_CONFIG_MAX_PUSH_CONSTANT_RANGES      8
#define KB_CONFIG_MAX_PUSH_CONSTANT_SIZE        1024
#define KB_CONFIG_MAX_DESCRIPTOR_WRITES         16
#define KB_CONFIG_MAX_GEOMETRIES                128
#define KB_CONFIG_TRANSIENT_BUFFER_SIZE         1024 * KB_CONFIG_MAX_DRAW_CALLS
#define KB_CONFIG_ENGINE_NAME                   "Kimberlite"
#define KB_CONFIG_STATS_SAMPLE_COUNT            100
#define KB_VGA_FONT_SMALL                       0

#define KB_CONFIG_FILE_MAGIC_GEOM               KB_FOURCC('K', 'B', 'G', 'E')

#ifdef __cplusplus
}
#endif
