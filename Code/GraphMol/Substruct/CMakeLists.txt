
rdkix_library(SubstructMatch 
              SubstructMatch.cpp SubstructUtils.cpp 
              LINK_LIBRARIES GenericGroups GraphMol RDGeneral)
target_compile_definitions(SubstructMatch PRIVATE RDKIX_SUBSTRUCTMATCH_BUILD)

rdkix_headers(SubstructMatch.h
              SubstructUtils.h DEST GraphMol/Substruct)

rdkix_test(testSubstructMatch test1.cpp LINK_LIBRARIES FileParsers SmilesParse SubstructMatch)

rdkix_catch_test(substructTestCatch catch_tests.cpp LINK_LIBRARIES FileParsers SmilesParse SubstructMatch)
