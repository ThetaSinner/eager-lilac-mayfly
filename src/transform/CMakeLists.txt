# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${transform_lib_name}
  TreeWalker.cpp
  TypeDeductionEngine.cpp
)

# Disable prefixing library name with 'lib'
set_target_properties(${transform_lib_name} PROPERTIES PREFIX "")

target_compile_features(${transform_lib_name} PRIVATE cxx_range_for)