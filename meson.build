project('traingrid',
        'cpp',
        version : '0.0.1',
        license : 'MIT',
        default_options : ['cpp_std=c++1z'])

# Scripts
script_common_args = ['--dir', meson.source_root()]
scripts_dir = 'scripts'

config_vars = [
  ['version.py', 'build_version'],
  ['hash.py', 'build_hash'],
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


# Dependencies
static = get_option('static')

sdl2_dep        = dependency('sdl2',        version : '>=2.0.5',    static : static)
sdl2_mixer_dep  = dependency('SDL2_mixer',                          static : static)
glew_dep        = dependency('glew',                                static : static)

deps = [sdl2_dep, glew_dep]


include_dirs = [
  include_directories('src')
]

link_libs = [

]

subdir('src')
