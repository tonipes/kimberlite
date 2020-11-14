incl_fmt = [
	'include',
]

src_fmt = [
  'src/format.cc', 
  'src/os.cc'
]

lib_fmt = static_library('fmt', src_fmt, include_directories : incl_fmt)

dep_fmt = declare_dependency(include_directories : incl_fmt, link_with : lib_fmt)
