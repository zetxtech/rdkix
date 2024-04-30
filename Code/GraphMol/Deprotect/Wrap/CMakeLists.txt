remove_definitions(-DRDKIX_SUBSTRUCTLIBRARY_BUILD)
rdkix_python_extension(rdDeprotect
                       rdDeprotect.cpp
                       DeprotectWrap.cpp
                       DEST Chem
                       LINK_LIBRARIES
                       Deprotect )

add_pytest(pyDeprotect ${CMAKE_CURRENT_SOURCE_DIR}/rough_test.py)
