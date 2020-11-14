meshoptimizer_src = [
  'src/meshoptimizer/allocator.cpp',
  'src/meshoptimizer/clusterizer.cpp',
  'src/meshoptimizer/indexcodec.cpp',
  'src/meshoptimizer/indexgenerator.cpp',
  'src/meshoptimizer/overdrawanalyzer.cpp',
  'src/meshoptimizer/overdrawoptimizer.cpp',
  'src/meshoptimizer/simplifier.cpp',
  'src/meshoptimizer/spatialorder.cpp',
  'src/meshoptimizer/stripifier.cpp',
  'src/meshoptimizer/vcacheanalyzer.cpp',
  'src/meshoptimizer/vcacheoptimizer.cpp',
  'src/meshoptimizer/vertexcodec.cpp',
  'src/meshoptimizer/vertexfilter.cpp',
  'src/meshoptimizer/vfetchanalyzer.cpp',
  'src/meshoptimizer/vfetchoptimizer.cpp',
]

meshoptimizer = static_library(
	'meshoptimizer',
	meshoptimizer_src,
	include_directories : 'src',
)

dep_meshoptimizer = declare_dependency(
	link_with           : meshoptimizer,
	include_directories : 'src',
)
