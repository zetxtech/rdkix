
rdkix_library(MolHash
              hashfunctions.cpp normalize.cpp
              LINK_LIBRARIES Depictor GraphMol)
target_compile_definitions(MolHash PRIVATE RDKIX_MOLHASH_BUILD)

rdkix_headers(MolHash.h nmmolhash.h           
              DEST GraphMol/MolHash)

rdkix_catch_test(molHashCatchTest catch_tests.cpp  LINK_LIBRARIES SmilesParse FileParsers MolHash)

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()

