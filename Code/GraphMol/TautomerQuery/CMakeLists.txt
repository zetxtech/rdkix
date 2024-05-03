if(RDK_USE_BOOST_SERIALIZATION AND Boost_SERIALIZATION_LIBRARY)
    set(RDKix_SERIALIZATION_LIBS ${Boost_SERIALIZATION_LIBRARY})
else()
    message("== Making TautomerQuery without boost Serialization support")
    set(RDKix_SERIALIZATION_LIBS )
endif()

rdkix_library(TautomerQuery 
              TautomerQuery.cpp
              LINK_LIBRARIES GraphMol RDGeneral MolStandardize ${RDKix_SERIALIZATION_LIBS})
target_compile_definitions(TautomerQuery PRIVATE RDKIX_TAUTOMERQUERY_BUILD)

rdkix_headers(TautomerQuery.h
              DEST GraphMol/TautomerQuery)

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()

rdkix_catch_test(tautomerQueryTestCatch catch_tests.cpp LINK_LIBRARIES TautomerQuery )
