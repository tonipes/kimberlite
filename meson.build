project(
  'kimberlite',
  ['cpp', 'c'],
  version: 'alpha',
  license : 'MIT',
  default_options : [
    'c_std=c99',
    'cpp_std=c++14',
    'cpp_rtti=false',
    # 'cpp_eh=none',    # Disable exceptions
  ]
)

fs = import('fs')

kb_desc = 'Kimberlite Framework'

if meson.is_cross_build()
  message('Is crossbuild')
endif

kb_build_machine  = build_machine.system()
kb_host_machine   = host_machine.system()

cppc = meson.get_compiler('cpp')

kb_build_tools = true

kb_cpp_args = [
  '-Wno-nullability-completeness',
  '-Werror=return-type',
  '-Wno-missing-braces',
]

dep_sdl2    = dependency('sdl2',    fallback: ['sdl2',    'sdl2_dep'])
dep_fmt     = dependency('fmt',     fallback: ['fmt',     'fmt_dep'])
dep_soloud  = dependency('soloud',  fallback: ['soloud',  'soloud_dep'])
dep_vulkan  = dependency('vulkan')

subdir('libraries')

# src_kimberlite = [
#   'source/audio.cpp',
#   'source/core.cpp',
#   'source/graphics.cpp',
#   'source/input.cpp',
# ]

# incl_kimberlite = [
#   'include'
# ]

# deps_kimberlite = [
#   dep_sdl2,
#   dep_vulkan,
#   dep_fmt,
#   dep_soloud,
#   dep_vkmemalloc,
#   dep_stb,
#   dep_spirvreflect,
#   dep_libyaml,
# ]

# exe_bin2c = executable(
#   'bin2c', 
#   ['source/bin2c.cpp'],
#   include_directories : incl_kimberlite,
#   dependencies        : deps_kimberlite,
#   cpp_args            : kb_cpp_args,
# )

# exe_geomc = executable(
#   'geomc', 
#   ['source/geomc.cpp'],
#   include_directories : incl_kimberlite,
#   dependencies        : deps_kimberlite + [ dep_meshoptimizer, dep_cgltf ],
#   cpp_args            : kb_cpp_args,
# )

# exe_texturec = executable(
#   'texturec', 
#   ['source/texturec.cpp'],
#   include_directories : incl_kimberlite,
#   dependencies        : deps_kimberlite + [ dep_stb ],
#   cpp_args            : kb_cpp_args,
# )

# exe_fontc = executable(
#   'fontc', 
#   ['source/fontc.cpp'],
#   include_directories : incl_kimberlite,
#   dependencies        : deps_kimberlite + [ dep_stb ],
#   cpp_args            : kb_cpp_args,
# )

# lib_kimberlite = static_library(
#   'kimberlite',
#   src_kimberlite,
#   include_directories : incl_kimberlite,
#   dependencies        : deps_kimberlite,
#   cpp_args            : kb_cpp_args,
# )

# dep_kimberlite = declare_dependency(
#   link_with           : lib_kimberlite,
#   dependencies        : deps_kimberlite,
#   include_directories : incl_kimberlite,
# )

build_assets = true

subdir('source')
subdir('assets')
subdir('examples')

# kbtools     = alias_target('kbtools', [exe_bin2c, exe_geomc, exe_texturec, exe_fontc])
# kb          = alias_target('kb', [lib_kimberlite])
