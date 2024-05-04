
rdkix_library(PartialCharges
              GasteigerCharges.cpp GasteigerParams.cpp
              LINK_LIBRARIES GraphMol RDGeneral)
target_compile_definitions(PartialCharges PRIVATE RDKIX_PARTIALCHARGES_BUILD)

rdkix_headers(GasteigerCharges.h
              GasteigerParams.h DEST GraphMol/PartialCharges)

rdkix_test(testPartialCharges test1.cpp LINK_LIBRARIES  PartialCharges
SmilesParse )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
