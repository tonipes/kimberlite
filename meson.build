project(
  'Kimberlite',
  ['cpp', 'c'],
  version: 'alpha',
  license : 'MIT',
  default_options : ['cpp_std=c++17']
)

kb_desc = 'Game engine'

# Build Scripts
scripts_dir = join_paths(meson.source_root(), 'build_scripts')

# Common compile args
kb_cpp_args = []
cppc = meson.get_compiler('cpp')

foreach flag : [
      # '-Wall',
      # '-Wextra',
    ]
  if cppc.has_argument(flag)
    kb_cpp_args += flag
  endif
endforeach

# Dependencies
dep_sdl2        = dependency('sdl2',    fallback: ['sdl2',    'sdl2_dep'])
dep_vulkan      = dependency('vulkan')
dep_threads     = dependency('threads')

dep_apple_foundation   = dependency('appleframeworks', modules : ['Foundation'])
dep_apple_metal        = dependency('appleframeworks', modules : ['Metal'])
dep_apple_opengl       = dependency('appleframeworks', modules : ['OpenGL'])
dep_apple_coregraphics = dependency('appleframeworks', modules : ['CoreGraphics'])
dep_apple_quartzcore   = dependency('appleframeworks', modules : ['QuartzCore'])

have_objcpp = add_languages('objcpp', required : false)
have_objc   = add_languages('objc', required : false)


kb_deps = [dep_vulkan, dep_threads, dep_sdl2]

kb_include_dirs = [
  include_directories('src'),
  include_directories('build'),
  include_directories('thirdparty/bgfx/include'),
  include_directories('thirdparty/bimg/include'),
  include_directories('thirdparty/bx/include'),
]

subdir('thirdparty')
subdir('build_info')
subdir('config')
subdir('src')
# subdir('test')
# subdir('assets')
# subdir('doc')
