remove_definitions(-DRDKIX_TAUTOMERQUERY_BUILD)
rdkix_python_extension(rdTautomerQuery
                       rdTautomerQuery.cpp
                       DEST Chem
                       LINK_LIBRARIES
                       TautomerQuery )

add_pytest(pyTautomerQuery ${CMAKE_CURRENT_SOURCE_DIR}/rough_test.py)
