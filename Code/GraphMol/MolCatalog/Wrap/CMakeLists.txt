remove_definitions(-DRDKIX_MOLCATALOG_BUILD)
rdkix_python_extension(rdMolCatalog rdMolCatalog.cpp
                       DEST Chem
                       LINK_LIBRARIES MolCatalog )

add_pytest(pyMolCatalog ${CMAKE_CURRENT_SOURCE_DIR}/rough_test.py)



