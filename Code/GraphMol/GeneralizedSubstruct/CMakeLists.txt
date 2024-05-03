
rdkix_library(GeneralizedSubstruct 
              XQMol.cpp TextIO.cpp
              LINK_LIBRARIES MolEnumerator TautomerQuery SubstructMatch SmilesParse GraphMol) 
target_compile_definitions(GeneralizedSubstruct PRIVATE RDKIX_GENERALIZEDSUBSTRUCT_BUILD)

rdkix_headers(XQMol.h DEST GraphMol/GeneralizedSubstruct)

if(RDK_USE_BOOST_SERIALIZATION)
rdkix_catch_test(generalizedSubstructCatch catch_tests.cpp LINK_LIBRARIES GenericGroups FileParsers GeneralizedSubstruct)
endif()

if (RDK_BUILD_PYTHON_WRAPPERS)
    add_subdirectory(Wrap)
endif ()
