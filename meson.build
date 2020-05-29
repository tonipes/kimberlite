project(
  'Kimberlite',
  ['cpp', 'c'],
  version: 'alpha',
  license : 'MIT',
  default_options : [
    'cpp_std=c++17',
  ]
)

if meson.is_cross_build()
  message('Is crossbuild')
endif

kb_build_machine = build_machine.system()
kb_host_machine = host_machine.system()

message('Build machine:\t' + kb_build_machine)
message('Host machine:\t' + kb_host_machine)

kb_desc = 'Kimberlite Framework'

cppc = meson.get_compiler('cpp')

kb_cpp_args = [ ]

kbdeps = []

if host_machine.system() == 'darwin'
  kbdeps += dependency('appleframeworks', modules : ['Metal', 'QuartzCore', 'Cocoa'])
endif

dep_sdl2  = dependency('sdl2')
dep_zip   = dependency('libzip')

# dep_zip   = dependency('libzip', fallback : ['libzip', 'libzip_dep'])

# deps      = dependency('libzip')

# dep_vulkan      = dependency('vulkan')
# dep_threads     = dependency('threads')

subdir('libraries')
subdir('bin')
subdir('shader')
subdir('scripts')
subdir('source')
subdir('assets')
subdir('config')
