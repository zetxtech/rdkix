if(RDK_USE_BOOST_SERIALIZATION AND Boost_SERIALIZATION_LIBRARY)
    set(RDKix_SERIALIZATION_LIBS ${Boost_SERIALIZATION_LIBRARY})
else()
      message("== Making EnumerateLibrary without boost Serialization support")
    set(RDKix_SERIALIZATION_LIBS )
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5)
  if(Boost_VERSION LESS  106200)
    set_property(SOURCE Enumerate/Enumerate.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -fpermissive ")
  endif()
endif()

rdkix_library(ChemReactions
              Reaction.cpp MDLParser.cpp DaylightParser.cpp ReactionPickler.cpp
	            ReactionWriter.cpp ReactionDepict.cpp ReactionFingerprints.cpp ReactionUtils.cpp 
              MoleculeParser.cpp ReactionRunner.cpp PreprocessRxn.cpp SanitizeRxn.cpp
              PNGParser.cpp CDXMLParser.cpp
                    Enumerate/Enumerate.cpp
                    Enumerate/EnumerationPickler.cpp
                    Enumerate/EvenSamplePairs.cpp
              LINK_LIBRARIES
              FilterCatalog Descriptors Fingerprints DataStructs Depictor
              FileParsers SubstructMatch ChemTransforms GraphMol ${RDKix_SERIALIZATION_LIBS})
target_compile_definitions(ChemReactions PRIVATE RDKIX_CHEMREACTIONS_BUILD)

rdkix_headers(Reaction.h
              ReactionParser.h
              ReactionPickler.h
              ReactionFingerprints.h
              ReactionUtils.h
              ReactionRunner.h
              PreprocessRxn.h
              SanitizeRxn.h
              DEST GraphMol/ChemReactions)

rdkix_headers(Enumerate/Enumerate.h
              Enumerate/EnumerateBase.h
              Enumerate/EnumerateTypes.h
              Enumerate/EnumerationPickler.h
              Enumerate/EnumerationStrategyBase.h
              Enumerate/CartesianProduct.h
              Enumerate/RandomSample.h
              Enumerate/RandomSampleAllBBs.h
              DEST GraphMol/ChemReactions/Enumerate)

rdkix_test(testReaction testReaction.cpp LINK_LIBRARIES
 ChemReactions )

rdkix_test(testReactionFingerprints testReactionFingerprints.cpp LINK_LIBRARIES
ChemReactions )

rdkix_test(testEnumeration Enumerate/testEnumerate.cpp LINK_LIBRARIES
ChemReactions  )

rdkix_catch_test(rxnTestCatch catch_tests.cpp LINK_LIBRARIES ChemReactions )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
