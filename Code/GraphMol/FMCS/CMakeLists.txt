
rdkix_library(FMCS
              FMCS.cpp Seed.cpp MaximumCommonSubgraph.cpp SubstructMatchCustom.cpp
              LINK_LIBRARIES Depictor SmilesParse FileParsers ChemTransforms SubstructMatch GraphMol )
target_compile_definitions(FMCS PRIVATE RDKIX_FMCS_BUILD)

rdkix_headers(FMCS.h
              Graph.h
              DEST GraphMol/FMCS)

rdkix_test(testFMCS testFMCS_Unit.cpp LINK_LIBRARIES
FMCS )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
add_subdirectory(Test)
