project('Traingrid',
  'cpp', 'c',
  license : 'MIT',
  default_options : ['cpp_std=c++17'])

description = 'Some-day-a-game'

# Scripts
script_common_args = []
scripts_dir = join_paths(meson.source_root(), 'scripts')

# Configuration
config_scripts = [
  ['build_version.py',  'BUILD_VERSION'],
  ['build_hash.py',     'BUILD_HASH'],
  ['build_time.py',     'BUILD_TIME'],
  ['build_uuid.py',     'BUILD_UUID'],
  ['build_sysinfo.py',  'BUILD_SYSINFO'],
]

confs = []

foreach p : config_scripts
  c_script = find_program(join_paths(scripts_dir, p[0]))
  c_command = [c_script, script_common_args]
  c_res = run_command(c_command, '{string}')

  if c_res.stderr() != ''
    message(c_res.stderr().strip())
  endif

  if c_res.returncode() != 0
    error('Couldn\'t get @0@: error @1@'.format(p[1], c_res.returncode()))
  endif

  c_str = c_res.stdout().strip()
  confs += [[p[1], c_str]]
endforeach

# Configuration
build_config = configuration_data()
foreach c : confs
  build_config.set(c[0], '"@0@"'.format(c[1]))
endforeach

build_config.set('LOG_LEVEL', 0)
build_config.set('LOG_CACHE', 100)
build_config.set('PROFILER_CACHE', 1000)
build_config.set('ENTITY_START_COUNT', 10)
build_config.set('MAX_COMPONENT_TYPES', 8192)
build_config.set('MAX_EVENT_TYPES', 8192)
build_config.set('TITLE', '"@0@"'.format(meson.project_name()))
build_config.set('DESCRIPTION', '"@0@"'.format(description))
build_config.set('LOGO', '')

# Build options
enable_audio = get_option('enable_audio')
enable_profiling = get_option('enable_profiling')
enable_asset_pack = get_option('enable_asset_pack')

# Dependencies
static = get_option('static')
deps = []

deps += dependency('lua',   fallback : ['lua',  'lua_dep'])
deps += dependency('json',  fallback : ['json', 'json_dep'])
deps += dependency('sdl2',  fallback : ['sdl2', 'sdl2_dep'])
deps += dependency('zlib',  fallback : ['zlib', 'zlib_dep'])
deps += dependency('glew')
deps += dependency('gl')
deps += dependency('glm')
deps += dependency('libzip')
deps += dependency('threads')

# if enable_audio
deps += dependency('SDL2_mixer')
# endif

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


subdir('assets')
subdir('src')
subdir('doc')

configure_file(configuration : build_config, output : 'build_config.h')

traingrid_exe = executable('traingrid',
                            launcher_src,
                            include_directories : include_dirs,
                            link_with : link_libs,
                            dependencies: deps)
