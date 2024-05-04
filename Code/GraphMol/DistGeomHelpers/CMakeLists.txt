
rdkix_library(DistGeomHelpers BoundsMatrixBuilder.cpp Embedder.cpp EmbedderUtils.cpp
              LINK_LIBRARIES ForceFieldHelpers SubstructMatch GraphMol DistGeometry Alignment
                )
target_compile_definitions(DistGeomHelpers PRIVATE RDKIX_DISTGEOMHELPERS_BUILD)

rdkix_headers(BoundsMatrixBuilder.h
              Embedder.h DEST GraphMol/DistGeomHelpers)

rdkix_test(testDistGeomHelpers testDgeomHelpers.cpp
           LINK_LIBRARIES
           DistGeomHelpers MolAlign MolTransforms FileParsers SmilesParse  )

rdkix_catch_test(distGeomHelpersCatch catch_tests.cpp 
LINK_LIBRARIES DistGeomHelpers MolAlign MolTransforms FileParsers SmilesParse )


if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()