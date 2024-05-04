remove_definitions(-DRDKIX_FILTERCATALOG_BUILD)
rdkix_python_extension(rdfiltercatalog
                       rdfiltercatalog.cpp
                       FilterCatalog.cpp
                       DEST Chem
                       LINK_LIBRARIES
                       FilterCatalog )

add_pytest(pyFilterCatalog ${CMAKE_CURRENT_SOURCE_DIR}/rough_test.py)
