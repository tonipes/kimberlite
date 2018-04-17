project('Traingrid',
  'cpp', 'c',
  version: 'alpha',
  license : 'MIT',
  default_options : ['cpp_std=c++17'])

description = 'Some-day-a-game'

asset_install_dir = 'dunno'

# Build Scripts
script_common_args = []
scripts_dir = join_paths(meson.source_root(), 'build_scripts')

# Dependencies
deps = []

dep_lua         = dependency('lua',   fallback : ['lua',  'lua_dep'])
dep_sdl2        = dependency('sdl2',  fallback : ['sdl2', 'sdl2_dep'])
dep_glew        = dependency('glew')
dep_gl          = dependency('gl')
dep_glm         = dependency('glm')
dep_libzip      = dependency('libzip')
dep_threads     = dependency('threads')
dep_sdl2_mixer  = dependency('SDL2_mixer')

# Configuration
build_config = configuration_data()

build_config.set('LOG_LEVEL',           0)
build_config.set('LOG_CACHE',           100)
build_config.set('PROFILER_CACHE',      1000)
build_config.set('ENTITY_START_COUNT',  10)
build_config.set('MAX_COMPONENT_TYPES', 8192)
build_config.set('MAX_EVENT_TYPES',     8192)
build_config.set('MAX_GL_LOG_SIZE',     1024)
build_config.set('TITLE',               '"@0@"'.format(meson.project_name()))
build_config.set('DESCRIPTION',         '"@0@"'.format(description))
build_config.set('LOGO', '')
build_config.set('RESOURCE_PACKAGE_EXT', '"zip"')

include_dirs = [
  include_directories('src'),
]

subdir('assets')
subdir('config')
subdir('doc')
subdir('src')
subdir('test')
