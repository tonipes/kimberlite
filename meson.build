project(
  'Kimberlite',
  ['cpp', 'c'],
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
endif

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
  '-Wno-nullability-completeness',
  '-Werror=return-type',
  '-Wno-missing-braces',
]

kb_link_args = ['-Wl,-no_pie']

if get_option('buildtype').startswith('debug')
  kb_compile_args += '-DDEBUG'
endif

foreach arg : kb_compile_args
  add_project_arguments(arg, language : 'c')
  add_project_arguments(arg, language : 'cpp')
endforeach


kb_cpp_args = []

dep_sdl2    = dependency('sdl2',    fallback: ['sdl2',    'sdl2_dep'])
dep_fmt     = dependency('fmt',     fallback: ['fmt',     'fmt_dep'])
dep_soloud  = dependency('soloud',  fallback: ['soloud',  'soloud_dep'])
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

if not doxygen.found()
  message('Doxygen not found. Can not generate documentation')
else
  doc_config = configuration_data()

  doc_config.set('TITLE', meson.project_name())
  doc_config.set('DESC',  kb_desc)
  doc_config.set('LOGO',  '')
  doc_config.set('SRC_PATH', join_paths(meson.source_root(), 'source'))

  if dot.found()
    doc_config.set('HAVE_DOT', 'YES')
  else
    doc_config.set('HAVE_DOT', 'NO')
  endif

  doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: doc_config,
  )

  html_target = custom_target(
    'docs',
    input: doxyfile,
    output: 'doc',
    command: [doxygen, doxyfile],
    build_by_default: false,
  )

endif
