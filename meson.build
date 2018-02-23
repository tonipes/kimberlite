project('mathtest',
  'cpp',
  version : '0.0.1',
  license : 'MIT',
  default_options : ['cpp_std=c++17'])


# Scripts
script_common_args = []
scripts_dir = join_paths(meson.source_root(), 'scripts')

# Configuration
config_vars = [
  ['build_version.py',  'build_version'],
  ['build_hash.py',     'build_hash'],
  ['build_time.py',     'build_time'],
  ['build_uuid.py',     'build_uuid'],
  ['build_sysinfo.py',  'build_sysinfo'],
]

confs = []

foreach p : config_vars
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

build_config.set('log_level', 0)

# Build options
enable_audio = get_option('enable_audio')
enable_profiling = get_option('enable_profiling')
enable_asset_pack = get_option('enable_asset_pack')

# Dependencies
static = get_option('static')
deps = []

deps += dependency('sdl2',            static : static)
deps += dependency('glew',            static : static)
deps += dependency('gl',              static : static)
deps += dependency('libzip',          static : static)

if enable_audio
  deps += dependency('SDL2_mixer',  static : static)
endif


include_dirs = [
  # include_directories('src')
]

link_libs = []


subdir('assets')
subdir('src')

configure_file(configuration : build_config, output : 'build_config.h')

traingrid_exe = executable('traingrid',
                            launcher_src,
                            include_directories : include_dirs,
                            link_with : link_libs,
                            dependencies: deps)
