project(
  'Kimberlite',
  ['cpp', 'c'],
  version: 'alpha',
  license : 'MIT',
  default_options : ['cpp_std=c++17']
)

kb_desc = 'Kimberlite Framework'

# Common compile args
kb_cpp_args = []
cppc = meson.get_compiler('cpp')

foreach flag : [
      # TODO: Use proper options
      '-Wall',
      '-fno-exceptions',
      '-DSPDLOG_NO_EXCEPTIONS', # TODO: Use spdlog build options
    ]
  if cppc.has_argument(flag)
    kb_cpp_args += flag
  endif
endforeach

subdir('libraries')

dep_applefw = dependency('appleframeworks', modules : ['Metal', 'QuartzCore', 'Cocoa'])

dep_sdl2        = dependency('sdl2')
dep_vulkan      = dependency('vulkan')
dep_threads     = dependency('threads')


incl_kb = [
  include_directories('include'),
]

subdir('config')
subdir('src')
subdir('tools')
