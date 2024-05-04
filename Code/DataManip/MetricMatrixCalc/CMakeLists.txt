### Template library
rdkix_headers(MetricFuncs.h
              MetricMatrixCalc.h DEST DataManip/MetricMatrixCalc)

rdkix_test(testMatCalc testMatCalc.cpp LINK_LIBRARIES RDGeneral)

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()


