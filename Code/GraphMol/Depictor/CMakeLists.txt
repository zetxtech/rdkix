
remove_definitions(-DRDKIX_GRAPHMOL_BUILD)

if(RDK_BUILD_COORDGEN_SUPPORT)

    if(MSVC AND (NOT RDK_INSTALL_DLLS_MSVC))
      add_definitions(-DSTATIC_COORDGEN)
    else(MSVC AND (NOT RDK_INSTALL_DLLS_MSVC))
      add_definitions(-DIN_COORDGEN)
    endif(MSVC AND (NOT RDK_INSTALL_DLLS_MSVC))

    include_directories(${coordgen_INCLUDE_DIRS})
endif()

rdkix_library(Depictor
              RDDepictor.cpp EmbeddedFrag.cpp DepictUtils.cpp Templates.cpp
              LINK_LIBRARIES ${RDK_COORDGEN_LIBS} SmilesParse SubstructMatch MolTransforms GraphMol RDGeneral)
target_compile_definitions(Depictor PRIVATE RDKIX_DEPICTOR_BUILD)

rdkix_headers(DepictUtils.h
              EmbeddedFrag.h
              RDDepictor.h DEST GraphMol/Depictor)

rdkix_test(testDepictor testDepictor.cpp
           LINK_LIBRARIES Depictor ChemTransforms FileParsers SmilesParse MolAlign)

FILE(GLOB TEST_OUTPUT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_data/*.out.sdf")
get_directory_property(extra_clean_files ADDITIONAL_MAKE_CLEAN_FILES)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
        "${extra_clean_files};${TEST_OUTPUT_FILES}")


rdkix_catch_test(depictorCatch catch_tests.cpp LINK_LIBRARIES Depictor SmilesParse FileParsers)


if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()