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

# Dependencies
dep_bx            = subproject('bx').get_variable('bx_dep')
dep_bimg          = subproject('bimg').get_variable('bimg_dep')
dep_bgfx          = subproject('bgfx').get_variable('bgfx_dep')
dep_meshoptimizer = subproject('bgfx').get_variable('meshoptimizer_dep')
dep_spdlog        = subproject('spdlog').get_variable('spdlog_headeronly_dep')

dep_sdl2        = dependency('sdl2')
dep_vulkan      = dependency('vulkan')
dep_threads     = dependency('threads')


incl_kb = [
  include_directories('include'),
  include_directories('thirdparty'),
  include_directories('subprojects'),
]

subdir('config')
subdir('src')
