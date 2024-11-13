remove_definitions(-DRDKIX_GRAPHMOL_BUILD)
add_definitions(-DRDKIX_DEPROTECT_BUILD)
rdkix_library(Deprotect
              Deprotect.cpp
              LINK_LIBRARIES ChemReactions 
	      FilterCatalog Fingerprints DataStructs Depictor
              FileParsers SubstructMatch ChemTransforms GraphMol)


rdkix_headers(Deprotect.h
              DEST GraphMol/Deprotect)

if(RDK_BUILD_PYTHON_WRAPPERS)
  add_subdirectory(Wrap)
endif()

rdkix_catch_test(deprotectTest deprotectTest.cpp LINK_LIBRARIES Deprotect )

