remove_definitions(-DRDKIX_RASCALMCES_BUILD)

rdkix_python_extension(rdRascalMCES
        rdRascalMCES.cpp
        DEST Chem
        LINK_LIBRARIES RascalMCES)

add_pytest(pyMolDraw2D ${CMAKE_CURRENT_SOURCE_DIR}/testRascalMCES.py)
