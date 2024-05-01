
rdkix_library(MolCatalog MolCatalogEntry.cpp MolCatalogParams.cpp
              LINK_LIBRARIES GraphMol Catalogs)
target_compile_definitions(MolCatalog PRIVATE RDKIX_MOLCATALOG_BUILD)

rdkix_headers(MolCatalogEntry.h
              MolCatalog.h
              MolCatalogParams.h DEST GraphMol/MolCatalog)

rdkix_test(testMolCatalog test1.cpp LINK_LIBRARIES 
MolCatalog FileParsers SmilesParse DataStructs )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()



