add_subdirectory(Alignment)  
add_subdirectory(EigenSolvers)
add_subdirectory(Optimizer)

rdkix_headers(Matrix.h
              SquareMatrix.h
              SymmMatrix.h
              Vector.h 
              Conrec.h DEST Numerics)

rdkix_test(testMatrices testMatrices.cpp LINK_LIBRARIES RDGeneral)
rdkix_catch_test(testConrec testConrec.cpp LINK_LIBRARIES RDGeometryLib )
