remove_definitions(-DRDKIX_FMCS_BUILD)
rdkix_python_extension(rdFMCS
                       rdFMCS.cpp
                       DEST Chem 
                       LINK_LIBRARIES FMCS)

add_pytest(pyFMCS ${CMAKE_CURRENT_SOURCE_DIR}/testFMCS.py)
