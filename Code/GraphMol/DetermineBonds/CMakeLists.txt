
rdkix_library(DetermineBonds DetermineBonds.cpp
              LINK_LIBRARIES EHTLib GraphMol RDGeneral)

target_compile_definitions(DetermineBonds PRIVATE RDKIX_DETERMINEBONDS_BUILD)

rdkix_headers(DetermineBonds.h DEST GraphMol/DetermineBonds)

rdkix_catch_test(determineBondsCatchTest catch_tests.cpp 
           LINK_LIBRARIES DetermineBonds FileParsers SmilesParse DistGeomHelpers ForceFieldHelpers)

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()           