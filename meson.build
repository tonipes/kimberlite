project('traingrid',
        'cpp',
        version : '0.0.1',
        license : 'MIT',
        default_options : ['cpp_std=c++1z'])

# Dependencies
static = get_option('static')

sdl2_dep        = dependency('sdl2',        version : '>=2.0.5',    static : static)
sdl2_mixer_dep  = dependency('SDL2_mixer',                          static : static)
glew_dep        = dependency('glew',                                static : static)

deps = [sdl2_dep, glew_dep]

sources = [
  'src/Main.cpp',
  'src/app/App.cpp',
  'src/game/Game.cpp',
  # 'src/context/EngineContext.cpp',
  'src/context/FileContext.cpp',
  'src/context/GraphicsContext.cpp',
]

include_dirs = [
  include_directories('src')
]

link_libs = [

]


traingrid_exe = executable('traingrid', sources,
                            include_directories : include_dirs,
                            link_with : link_libs,
                            dependencies: deps)
