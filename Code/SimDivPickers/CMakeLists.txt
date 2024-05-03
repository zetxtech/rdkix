
rdkix_library(SimDivPickers
              DistPicker.cpp MaxMinPicker.cpp HierarchicalClusterPicker.cpp
              LINK_LIBRARIES hc RDGeneral)
target_compile_definitions(SimDivPickers PRIVATE RDKIX_SIMDIVPICKERS_BUILD)

rdkix_headers(DistPicker.h LeaderPicker.h 
              HierarchicalClusterPicker.h
              MaxMinPicker.h DEST SimDivPickers)

rdkix_test(testSimDivPickers testPickers.cpp LINK_LIBRARIES SimDivPickers)

rdkix_catch_test(pickersTestsCatch catch_tests.cpp
           LINK_LIBRARIES SimDivPickers DataStructs)


if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
