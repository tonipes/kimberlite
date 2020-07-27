project(
  'kimberlite',
  ['cpp', 'c', 'swift'],
  version: 'alpha',
  license : 'MIT',
  default_options : [
    'cpp_std=c++17',
  ]
)

# add_global_link_arguments(['-Xlinker', '-lc++'], language: 'swift')
add_global_link_arguments(['-Xlinker', '-lc++', '-framework', 'Foundation'], language: 'swift')

fs = import('fs')

kb_desc = 'Kimberlite Framework'

if meson.is_cross_build()
  message('Is crossbuild')
endif

kb_build_machine  = build_machine.system()
kb_host_machine   = host_machine.system()

cppc = meson.get_compiler('cpp')

kb_build_tools = true

kb_cpp_args = [ ]

dep_sdl2    = dependency('sdl2',    fallback: ['sdl2',    'sdl2_dep'])
dep_bx      = dependency('bx',      fallback: ['bx',      'bx_dep'])
dep_bimg    = dependency('bimg',    fallback: ['bimg',    'bimg_dep'])
dep_bgfx    = dependency('bgfx',    fallback: ['bgfx',    'bgfx_dep'])
dep_fmt     = dependency('fmt',     fallback: ['fmt',     'fmt_dep'])
dep_soloud  = dependency('soloud',  fallback: ['soloud',  'soloud_dep'])
dep_yaml    = dependency('libyaml', fallback: ['libyaml', 'libyaml_dep'])

shaderc_exe = subproject('bgfx').get_variable('shaderc_exe')

subdir('embed')
subdir('libraries')
subdir('source')
subdir('examples')
