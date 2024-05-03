remove_definitions(-DRDKIX_ALIGNMENT_BUILD)
rdkix_python_extension(rdAlignment
                       rdAlignment.cpp
                       DEST Numerics 
                       LINK_LIBRARIES Alignment)

add_pytest(pyAlignment ${CMAKE_CURRENT_SOURCE_DIR}/testAlignment.py)

