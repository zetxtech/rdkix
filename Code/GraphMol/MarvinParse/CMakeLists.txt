
rdkix_library(MarvinParser
              MarvinParser.cpp
              MarvinWriter.cpp
              MarvinDefs.cpp
              LINK_LIBRARIES GenericGroups Depictor GraphMol ChemReactions )
target_compile_definitions(MarvinParser PRIVATE RDKIX_MARVINPARSER_BUILD)

rdkix_headers(MarvinParser.h
              DEST GraphMol/MarvinParse)

rdkix_test(testMrvToMol testMrvToMol.cpp
           LINK_LIBRARIES MarvinParser FileParsers SubstructMatch )


