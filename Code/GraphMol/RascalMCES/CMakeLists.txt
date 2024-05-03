
rdkix_library(RascalMCES
        RascalMCES.cpp RascalCluster.cpp RascalButinaCluster.cpp
        lap_a_la_scipy.cpp PartitionSet.cpp RascalResult.cpp
        LINK_LIBRARIES SmilesParse FileParsers ChemTransforms SubstructMatch GraphMol)
target_compile_definitions(RascalMCES PRIVATE RDKIX_RASCALMCES_BUILD)

rdkix_headers(RascalMCES.h RascalOptions.h RascalClusterOptions.h RascalResult.h
        DEST GraphMol/RascalMCES)

rdkix_catch_test(testRascalMCES mces_catch.cpp LINK_LIBRARIES RascalMCES)
rdkix_catch_test(testRascalCluster mces_cluster_catch.cpp LINK_LIBRARIES RascalMCES)

if (RDK_BUILD_PYTHON_WRAPPERS)
    add_subdirectory(Wrap)
endif ()
