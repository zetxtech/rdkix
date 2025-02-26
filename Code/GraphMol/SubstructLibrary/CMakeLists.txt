if(RDK_USE_BOOST_SERIALIZATION AND Boost_SERIALIZATION_LIBRARY)
    set(RDKix_SERIALIZATION_LIBS ${Boost_SERIALIZATION_LIBRARY})
else()
    message("== Making SubstructLibrary without boost Serialization support")
    set(RDKix_SERIALIZATION_LIBS )
endif()

rdkix_library(SubstructLibrary
              SubstructLibrary.cpp
	      PatternFactory.cpp
              LINK_LIBRARIES  TautomerQuery MolStandardize Fingerprints SubstructMatch SmilesParse
              GraphMol Catalogs DataStructs RDGeneral ${RDKix_SERIALIZATION_LIBS})
target_compile_definitions(SubstructLibrary PRIVATE RDKIX_SUBSTRUCTLIBRARY_BUILD)

rdkix_headers(SubstructLibrary.h
              SubstructLibrarySerialization.h
              PatternFactory.h
              DEST GraphMol/SubstructLibrary)

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()

rdkix_test(substructLibraryTest substructLibraryTest.cpp
           LINK_LIBRARIES TautomerQuery MolStandardize SubstructLibrary FileParsers )

rdkix_catch_test(substructLibraryCatchTest catch_tests.cpp LINK_LIBRARIES SubstructLibrary FileParsers )
