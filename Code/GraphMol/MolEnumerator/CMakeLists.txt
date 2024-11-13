rdkix_library(MolEnumerator
              MolEnumerator.cpp PositionVariation.cpp LinkNode.cpp RepeatUnit.cpp
              LINK_LIBRARIES FileParsers ChemReactions ChemTransforms 
              SubstructMatch GraphMol RDGeneral)
target_compile_definitions(MolEnumerator PRIVATE RDKIX_MOLENUMERATOR_BUILD)

rdkix_headers(MolEnumerator.h LinkNode.h DEST GraphMol/MolEnumerator)

rdkix_catch_test(testMolEnumerator enumerator_catch.cpp 
LINK_LIBRARIES MolEnumerator SmilesParse FileParsers )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
