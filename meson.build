project(
  'kimberlite',
  ['cpp', 'c'],
  version: 'alpha',
  license : 'MIT',
  default_options : [
    'cpp_std=c++17',
  ]
)

fs = import('fs')

if meson.is_cross_build()
  message('Is crossbuild')
endif

kb_build_machine  = build_machine.system()
kb_host_machine   = host_machine.system()

message('Build machine:\t' + kb_build_machine)
message('Host machine:\t' + kb_host_machine)

kb_desc = 'Kimberlite Framework'

cppc = meson.get_compiler('cpp')

kb_cpp_args = [ ]

kb_install_exec_dir = 'Contents/MacOS'
kb_install_asset_dir = 'Contents/Resources'

kbdeps = []

dep_sdl2 = []

if host_machine.system() == 'emscripten'
  dep_sdl2 = []
  kb_cpp_args += ['-s', 'WASM=1']
  kb_cpp_args += ['-s', 'USE_SDL=2']
  kb_cpp_args += ['-s', 'MAX_WEBGL_VERSION=2']
  kb_cpp_args += ['-s', 'ALLOW_MEMORY_GROWTH=1']
else 
  dep_sdl2 = dependency('sdl2', fallback: ['sdl2', 'sdl2_dep'])
endif

dep_bx      = subproject('bx')    .get_variable('bx_dep')
dep_bimg    = subproject('bimg')  .get_variable('bimg_dep')
dep_bgfx    = subproject('bgfx')  .get_variable('bgfx_dep')
dep_fmt     = subproject('fmt')   .get_variable('fmt_dep')

subdir('libraries')
subdir('bin')
subdir('shader')
subdir('scripts')
subdir('assets')
subdir('source')
subdir('config')
subdir('misc')
