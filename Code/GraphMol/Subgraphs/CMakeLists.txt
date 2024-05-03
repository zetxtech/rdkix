
rdkix_library(Subgraphs Subgraphs.cpp SubgraphUtils.cpp
              LINK_LIBRARIES GraphMol RDGeneral )
target_compile_definitions(Subgraphs PRIVATE RDKIX_SUBGRAPHS_BUILD)

rdkix_headers(Subgraphs.h
              SubgraphUtils.h DEST GraphMol/Subgraphs)

target_include_directories(Subgraphs PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/External/boost-numeric-bindings>)

rdkix_test(testSubgraphs1 test1.cpp LINK_LIBRARIES Subgraphs SmilesParse )
rdkix_test(testSubgraphs2 test2.cpp LINK_LIBRARIES SmilesParse Subgraphs )
rdkix_catch_test(subgraphsCatch catch_tests.cpp LINK_LIBRARIES SmilesParse Subgraphs)
