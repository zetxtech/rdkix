
rdkix_library(GenericGroups 
              GenericGroups.cpp 
              LINK_LIBRARIES GraphMol RDGeneral)
target_compile_definitions(GenericGroups PRIVATE RDKIX_GENERICGROUPS_BUILD)

rdkix_headers(GenericGroups.h DEST GraphMol/GenericGroups)

rdkix_catch_test(testGenericGroups generic_tests.cpp 
    LINK_LIBRARIES GenericGroups FileParsers MarvinParser SmilesParse SubstructMatch)
