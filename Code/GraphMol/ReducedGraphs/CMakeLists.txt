
rdkix_library(ReducedGraphs
              ReducedGraphs.cpp
              LINK_LIBRARIES SubstructMatch SmilesParse GraphMol RDGeneral)
target_compile_definitions(ReducedGraphs PRIVATE RDKIX_REDUCEDGRAPHS_BUILD)

rdkix_headers(ReducedGraphs.h
              DEST GraphMol/ReducedGraphs)

rdkix_test(testReducedGraphs test1.cpp LINK_LIBRARIES
           ReducedGraphs )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()

