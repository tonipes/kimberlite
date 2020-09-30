kbtest_sources = [
  'test_array.cpp',
  'test_main.cpp',
  'test_hash.cpp',
  'test_table.cpp',
  'test_freelist.cpp',
]

kbtest_deps = [
  dep_kimberlite,
  dep_catch,
]

link_args = []

kbtest_exe = executable('kbtest',  
  kbtest_sources,
  dependencies        : kbtest_deps,
)

test('kbtest', kbtest_exe)
