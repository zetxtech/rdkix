
rdkix_library(Alignment AlignPoints.cpp LINK_LIBRARIES RDGeometryLib)
target_compile_definitions(Alignment PRIVATE RDKIX_ALIGNMENT_BUILD)

rdkix_headers(AlignPoints.h DEST Numerics/Alignment)

rdkix_test(testAlignment testAlignment.cpp LINK_LIBRARIES Alignment )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
