incl = incl_kimberlite + [ '../source' ]

exe_bin2c = executable(

  'bin2c', 
  ['bin2c.cpp'],
  include_directories : incl,
  dependencies        : deps_kimberlite,
  cpp_args            : kb_cpp_args,
)

exe_geomc = executable(
  'geomc', 
  ['geomc.cpp'],
  include_directories : incl,
  dependencies        : deps_kimberlite + [ dep_meshoptimizer, dep_cgltf ],
  cpp_args            : kb_cpp_args,
)

exe_texturec = executable(
  'texturec', 
  ['texturec.cpp'],
  include_directories : incl,
  dependencies        : deps_kimberlite + [ dep_stb ],
  cpp_args            : kb_cpp_args,
)

exe_fontc = executable(
  'fontc', 
  ['fontc.cpp'],
  include_directories : incl,
  dependencies        : deps_kimberlite + [ dep_stb ],
  cpp_args            : kb_cpp_args,
)
