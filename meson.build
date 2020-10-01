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

build_assets = true
# build_assets = false

subdir('source')
subdir('assets')
subdir('examples')
subdir('test')
