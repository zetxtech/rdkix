
rdkix_library(Optimizer 
              BFGSOpt.cpp LinearSearch.cpp
              LINK_LIBRARIES RDGeometryLib Trajectory RDGeneral)
target_compile_definitions(Optimizer PRIVATE RDKIX_OPTIMIZER_BUILD)

rdkix_headers(BFGSOpt.h DEST Numerics/Optimizer)

rdkix_test(testOptimizer testOptimizer.cpp LINK_LIBRARIES Optimizer )


