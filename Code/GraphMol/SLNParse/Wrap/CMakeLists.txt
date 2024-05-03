remove_definitions(-DRDKIX_SLNPARSE_BUILD)
rdkix_python_extension(rdSLNParse rdSLNParse.cpp
                       DEST Chem
                       LINK_LIBRARIES
                       SLNParse )

add_pytest(pySLNParse ${CMAKE_CURRENT_SOURCE_DIR}/testSLN.py)
