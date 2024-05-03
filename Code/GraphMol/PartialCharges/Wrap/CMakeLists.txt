remove_definitions(-DRDKIX_PARTIALCHARGES_BUILD)
rdkix_python_extension(rdPartialCharges
                       rdPartialCharges.cpp
                       DEST Chem 
                       LINK_LIBRARIES PartialCharges ) 

add_pytest(pyPartialCharges ${CMAKE_CURRENT_SOURCE_DIR}/testPartialCharges.py)
