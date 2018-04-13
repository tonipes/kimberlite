project('Traingrid',
  'cpp', 'c',
  version: 'alpha',
  license : 'MIT',
  default_options : ['cpp_std=c++17'])

description = 'Some-day-a-game'

asset_install_dir = 'dunno'

# Scripts
script_common_args = []
scripts_dir = join_paths(meson.source_root(), 'build_scripts')
build_info_header_dir = join_paths(meson.source_root(), 'build_info')

# Version info
version_info_def = [
  ['build_version.py',  'build_version.cpp', '0'],
  ['build_hash.py',     'build_hash.cpp',    '0'],
  ['build_time.py',     'build_time.cpp',    '0'],
  ['build_uuid.py',     'build_uuid.cpp',    '0'],
  ['build_sysinfo.py',  'build_sysinfo.cpp', '0'],
]

version_info_targets = []

foreach c: version_info_def
  version_info_targets += vcs_tag('vcs_tag_@0@'.format(c[1].underscorify()),
    input: join_paths(build_info_header_dir, c[1]),
    fallback: c[2],
    output: c[1],
    command: [join_paths(scripts_dir, c[0])],
    replace_string: '@IN@'
  )
endforeach

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

# Dependencies
deps = []

deps += dependency('lua',   fallback : ['lua',  'lua_dep'])
deps += dependency('sdl2',  fallback : ['sdl2', 'sdl2_dep'])
deps += dependency('zlib',  fallback : ['zlib', 'zlib_dep'])
deps += dependency('glew')
deps += dependency('gl')
deps += dependency('glm')
deps += dependency('libzip')
deps += dependency('threads')
deps += dependency('SDL2_mixer')


# Documentation
doxygen = find_program('doxygen', required : false)
if not doxygen.found()
  message('Doxygen not found. Can not generate documentation')
endif

if find_program('dot', required : false).found()
  build_config.set('HAVE_DOT', 'YES')
else
  build_config.set('HAVE_DOT', 'NO')
endif

include_dirs = [
  include_directories('src')
]

link_libs = []

subdir('src')
subdir('config')
subdir('doc')
subdir('test')

src_base = src_base + version_info_targets

# Tests
test_exe = executable('unit_test',  
    src_base + test_sources,
    include_directories : include_dirs,
    link_with : link_libs,
    dependencies: deps
)

test('Unit Tests', test_exe)


# Asset target
asset_build_script = find_program(join_paths(meson.source_root(), 'assets.sh'))

asset_build_target = custom_target('assets',
  output      : 'build_assets',
  input       : 'assets',
  command     : [asset_build_script, '-b'],
  install     : true,
  install_dir : asset_install_dir
)

configure_file(configuration : build_config, output : 'build_config.h')

traingrid_exe = executable('traingrid',
                            launcher_src + src_base,
                            include_directories : include_dirs,
                            link_with : link_libs,
                            dependencies: deps)
