project(
  'Kimberlite',
  ['cpp', 'c', 'objcpp'],
  version: 'alpha',
  license : 'MIT',
  default_options : [
    'c_std=c99',
    'cpp_std=c++14',
    'cpp_rtti=false',
    # 'cpp_eh=none',    # Disable exceptions
  ]
)

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'c')
  add_project_arguments('-DDEBUG', language : 'cpp')
  add_project_arguments('-DDEBUG', language : 'objcpp')
endif

# option('kb_graphics_lib', type : 'combo', choices : ['vulkan', 'metal'], value : 'vulkan')

kb_graphics_lib = 'metal'
# kb_graphics_lib = 'vulkan'

fs = import('fs')

kb_desc = 'Kimberlite Framework'

if meson.is_cross_build()
  message('Is crossbuild')
endif

kb_build_machine  = build_machine.system()
kb_host_machine   = host_machine.system()

cppc = meson.get_compiler('cpp')

kb_build_tools = true

kb_compile_args = [
  # '-Werror=return-type',
  # '-fmessage-length=0',
  # '-Werror',
  # '-Wcast-qual',
  # '-Wall',
  # '-Wextra',
  # '-Wwrite-strings',
  # '-Winit-self',
  # '-Wcast-align',
  # '-Wpointer-arith',
  # '-Wstrict-aliasing',
  # '-Wformat=2',
  # '-Wmissing-declarations',
  # '-Wmissing-include-dirs',
  # '-Wno-unused-parameter',
  # '-Wuninitialized',
  # '-Wold-style-definition',
  # '-Wstrict-prototypes',
  # '-Wmissing-prototypes',
  '-Wno-nullability-completeness',
  '-Wno-non-virtual-dtor',
  '-Wno-missing-braces',
]

kb_link_args = ['-Wl,-no_pie']

if get_option('buildtype').startswith('debug')
  kb_compile_args += '-DDEBUG'
endif

foreach arg : kb_compile_args
  add_project_arguments(arg, language : 'c')
  add_project_arguments(arg, language : 'cpp')
  add_project_arguments(arg, language : 'objcpp')
endforeach

kb_cpp_args = []
kb_objcpp_args = ['-std=c++14']

dep_sdl2    = dependency('sdl2')
dep_vulkan  = dependency('vulkan')

subdir('libraries')

build_assets = true
# build_assets = false

subdir('source')
subdir('assets')
subdir('examples')
subdir('test')

# Documentation

doxygen = find_program('doxygen', required : false)
dot = find_program('dot', required : false)

# if not doxygen.found()
#   message('Doxygen not found. Can not generate documentation')
# else
#   doc_config = configuration_data()

#   doc_config.set('TITLE', meson.project_name())
#   doc_config.set('DESC',  kb_desc)
#   doc_config.set('LOGO',  '')
#   doc_config.set('SRC_PATH', join_paths(meson.source_root(), 'source'))

#   if dot.found()
#     doc_config.set('HAVE_DOT', 'YES')
#   else
#     doc_config.set('HAVE_DOT', 'NO')
#   endif

#   doxyfile = configure_file(
#     input: 'Doxyfile.in',
#     output: 'Doxyfile',
#     configuration: doc_config,
#   )

#   html_target = custom_target(
#     'docs',
#     input: doxyfile,
#     output: 'doc',
#     command: [doxygen, doxyfile],
#     build_by_default: false,
#   )

# endif
