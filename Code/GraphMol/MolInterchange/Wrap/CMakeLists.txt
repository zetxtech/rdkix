remove_definitions(-DRDKIX_MOLINTERCHANGE_BUILD)
rdkix_python_extension(rdMolInterchange
                       rdMolInterchange.cpp
                       DEST Chem
                       LINK_LIBRARIES MolInterchange)

add_pytest(pyMolInterchange ${CMAKE_CURRENT_SOURCE_DIR}/testMolInterchange.py)
