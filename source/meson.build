incl_kimberlite = [ '../include' ]

src_kimberlite_extra = [
  'kbextra/cliargs.cpp',
]

src_kimberlite_core = [
  'kb/array.cpp',
  'kb/audio.cpp',
  'kb/alloc.cpp',
  'kb/hash.cpp',
  'kb/log.cpp',
  'kb/math.cpp',
  'kb/rand.cpp',
  'kb/thread.cpp',
  'kb/time.cpp',
  'kb/crt.cpp',
  'kb/table.cpp',
  'kb/freelist.cpp',
  'kb/vertex.cpp',
  'kb/sampler.cpp',
  'kb/input.cpp',
  'kb/geometry.cpp',
  'kb/texture.cpp',
  'kb/font.cpp',
  'kb/gizmo.cpp',
  'kb/renderer.cpp',
  'kb/material.cpp'
]

src_kimberlite = [
  'platform/platform_audio_soloud.cpp',
  'platform/platform_rwops_sdl.cpp',
  'platform/platform_surface_sdl.cpp',
  'platform/platform_input_sdl.cpp',
  'platform/platform_macos.mm',
] + src_kimberlite_core + src_kimberlite_extra


deps_kimberlite = [
  dep_sdl2,
  dep_vulkan,
  dep_fmt,
  dep_soloud,
  dep_vkmemalloc,
  dep_stb,
]

if host_machine.system() == 'darwin'
  appleframeworks_dep = dependency('appleframeworks', modules : ['Metal', 'QuartzCore', 'Cocoa'])
  deps_kimberlite += appleframeworks_dep
endif

kb_setup_args = []

if kb_graphics_lib.startswith('vulkan')
  kb_setup_args += ['-DKB_SETUP_GRAPHICS_LIB_VULKAN']
  src_kimberlite += ['platform/platform_graphics_vulkan.cpp']
endif

if kb_graphics_lib.startswith('metal')
  kb_setup_args += ['-DKB_SETUP_GRAPHICS_LIB_METAL']
  src_kimberlite += ['platform/platform_graphics_metal.mm']
endif

if kb_graphics_lib.startswith('noop')
  kb_setup_args += ['-DKB_SETUP_GRAPHICS_LIB_NOOP']
  src_kimberlite += ['platform/platform_graphics_noop.cpp']
endif

kb_cpp_args += kb_setup_args
kb_objcpp_args += kb_setup_args

lib_kimberlite = static_library('kimberlite', 
  src_kimberlite,
  include_directories : incl_kimberlite,
  dependencies        : deps_kimberlite,
  cpp_args            : kb_cpp_args,
  objcpp_args         : kb_objcpp_args,
)

dep_kimberlite = declare_dependency(
  link_with           : lib_kimberlite,
  dependencies        : deps_kimberlite,
  include_directories : incl_kimberlite,
)

exe_bin2c = executable(
  'bin2c', 
  ['bin2c.cpp'],
  include_directories : incl_kimberlite,
  dependencies        : deps_kimberlite,
  cpp_args            : kb_cpp_args,
)

exe_geomc = executable(
  'geomc', 
  ['geomc.cpp', 'kb/array.cpp'],
  include_directories : incl_kimberlite,
  dependencies        : deps_kimberlite + [ dep_meshoptimizer, dep_cgltf ],
  cpp_args            : kb_cpp_args,
)

exe_texturec = executable(
  'texturec', 
  ['texturec.cpp'],
  include_directories : incl_kimberlite,
  dependencies        : deps_kimberlite + [ dep_stb ],
  cpp_args            : kb_cpp_args,
)

exe_fontc = executable(
  'fontc', 
  ['fontc.cpp'],
  include_directories : incl_kimberlite,
  dependencies        : deps_kimberlite + [ dep_stb ],
  cpp_args            : kb_cpp_args,
)
