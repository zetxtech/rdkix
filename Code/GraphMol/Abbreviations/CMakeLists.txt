rdkix_library(Abbreviations
              Abbreviations.cpp
              AbbreviationsUtils.cpp
              LINK_LIBRARIES SmilesParse 
              SubstructMatch GraphMol RDGeneral)
target_compile_definitions(Abbreviations PRIVATE RDKIX_ABBREVIATIONS_BUILD)

rdkix_headers(Abbreviations.h DEST GraphMol/Abbreviations)

rdkix_catch_test(testAbbreviations catch_tests.cpp 
LINK_LIBRARIES Abbreviations SmilesParse FileParsers )

if(RDK_BUILD_PYTHON_WRAPPERS)
  add_subdirectory(Wrap)
endif()
