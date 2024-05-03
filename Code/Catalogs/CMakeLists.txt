
rdkix_library(Catalogs 
              Catalog.cpp CatalogParams.cpp CatalogEntry.cpp
              LINK_LIBRARIES RDGeneral)
target_compile_definitions(Catalogs PRIVATE RDKIX_CATALOGS_BUILD)

rdkix_headers(CatalogEntry.h
              Catalog.h
              CatalogParams.h DEST Catalogs)


