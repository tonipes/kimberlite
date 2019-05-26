project(
  'Kimberlite',
  'cpp', 'c',
  version: 'alpha',
  license : 'MIT',
  default_options : ['cpp_std=c++17']
)

kb_desc = 'Game engine'

# Build Scripts
scripts_dir = join_paths(meson.source_root(), 'build_scripts')

# Common compile args
wg_cpp_args = []
cppc = meson.get_compiler('cpp')

foreach flag : [
      # '-Wall',
      # '-Wextra',
    ]
  if cppc.has_argument(flag)
    wg_cpp_args += flag
  endif
endforeach

# Dependencies
dep_lua         = dependency('lua')
dep_sdl2        = dependency('sdl2')
dep_vulkan      = dependency('vulkan')
dep_threads     = dependency('threads')
dep_filesystem  = declare_dependency(link_args : ['-lstdc++fs'])

include_dirs_kb = [
  include_directories('src'),
  include_directories('build'),
]

subdir('config')
subdir('assets')
subdir('src')
subdir('test')
subdir('doc')
