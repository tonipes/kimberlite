project(
  'Wargrid',
  'cpp', 'c',
  version: 'alpha',
  license : 'MIT',
  default_options : ['cpp_std=c++17']
)

# Settings

wg_cpp_args = []

cppc = meson.get_compiler('cpp')

foreach flag : [
      '-Wparentheses',
      '-Wtype-limits',
      '-Wstrict-prototypes',
      '-Wlong-long',
      '-Winit-self',
      '-Wnull-dereference',
      '-Wformat-pedantic',
      '-Wfloat-overflow-conversion',
      '-Wfloat-zero-conversion',
      '-Wfor-loop-analysis',
      '-Wimplicit-fallthrough',
      '-Wsometimes-uninitialized',
      '-Wunneeded-internal-declaration',
      '-Wunreachable-code',
      '-Wunreachable-code-loop-increment',
      '-Wcast-align',
      '-Wno-long-long',
    ]
  if cppc.has_argument(flag)
    wg_cpp_args += flag
  endif
endforeach

description = 'Turn-based strategy game'

asset_install_dir = 'assets'

# Build Scripts
script_common_args = []
scripts_dir = join_paths(meson.source_root(), 'build_scripts')

# Dependencies
deps = []

dep_lua         = dependency('lua',   fallback : ['lua',  'lua_dep'])
dep_sdl2        = dependency('sdl2',  fallback : ['sdl2', 'sdl2_dep'])
dep_glew        = dependency('glew',  fallback : ['glew', 'glew_dep'])
dep_threads     = dependency('threads')

dep_filesystem  = declare_dependency(link_args : ['-lstdc++fs'])

# Tools 
build_tool_dir = join_paths(meson.source_root(), 'build_scripts')

build_version_tool  = find_program(join_paths(build_tool_dir, 'build_version.py'))
build_hash_tool     = find_program(join_paths(build_tool_dir, 'build_hash.py'))
build_uuid_tool     = find_program(join_paths(build_tool_dir, 'build_uuid.py'))
build_time_tool     = find_program(join_paths(build_tool_dir, 'build_time.py'))
build_sysinfo_tool  = find_program(join_paths(build_tool_dir, 'build_sysinfo.py'))

# Configuration
build_config = configuration_data()

# Dot for doxygen
if find_program('dot', required : false).found()
  build_config.set('HAVE_DOT', 'YES')
else
  build_config.set('HAVE_DOT', 'NO')
endif

build_config.set('WARGRID_SRC_PATH',        '"@0@"'.format(join_paths(meson.source_root(), 'src')))
build_config.set('WARGRID_TITLE',           '"@0@"'.format(meson.project_name()))
build_config.set('WARGRID_DESC',            '"@0@"'.format(description))
build_config.set('WARGRID_LOGO',            '""')
build_config.set('WARGRID_BUILD_VERSION',   '"@0@"'.format(run_command(build_version_tool).stdout().strip()))
build_config.set('WARGRID_BUILD_HASH',      '"@0@"'.format(run_command(build_hash_tool).stdout().strip()))
build_config.set('WARGRID_BUILD_UUID',      '"@0@"'.format(run_command(build_uuid_tool).stdout().strip()))
build_config.set('WARGRID_BUILD_TIME',      '"@0@"'.format(run_command(build_time_tool).stdout().strip()))
build_config.set('WARGRID_BUILD_SYS_INFO',  '"@0@"'.format(run_command(build_sysinfo_tool).stdout().strip()))

configure_file(output : 'version.h', configuration : build_config)

include_dirs = [
  include_directories('src'),
  include_directories('build'),
]

subdir('assets')
subdir('config')
subdir('doc')
subdir('src')
subdir('test')
