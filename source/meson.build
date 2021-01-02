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
  'kb/sampler.cpp',
  'kb/input.cpp',
  'kb/texture.cpp',
  'kb/graphics.cpp',
  'kbextra/vertex.cpp',
  'kbextra/material.cpp',
  'kbextra/geometry.cpp',
  'kbextra/font.cpp',
  'kbextra/gizmo.cpp',
]

src_kimberlite = [] + src_kimberlite_core + src_kimberlite_extra


deps_kimberlite = [
  dep_fmt,
]

kb_setup_args = []

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
