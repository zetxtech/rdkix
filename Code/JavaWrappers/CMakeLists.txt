find_package(SWIG 4.2 REQUIRED)
include(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${RDKix_CodeDir})
include_directories(${Boost_INCLUDE_DIRS})

SET(CMAKE_SWIG_FLAGS "")


set(swigRDKixLibSuffix "")
if(RDK_SWIG_STATIC AND (NOT WIN32))
  set(swigRDKixLibSuffix "_static")
endif()
set(swigRDKixLibList "")
if(RDK_BUILD_AVALON_SUPPORT)
  set(swigRDKixLibList "${swigRDKixLibList}AvalonLib;avalon_clib;")
endif(RDK_BUILD_AVALON_SUPPORT)
if(RDK_BUILD_INCHI_SUPPORT)
  set(swigRDKixLibList "${swigRDKixLibList}RDInchiLib;${INCHI_LIBRARIES};")
endif(RDK_BUILD_INCHI_SUPPORT)
set(swigRDKixLibList "${swigRDKixLibList}"
  "RascalMCES;GeneralizedSubstruct;Abbreviations;ScaffoldNetwork;MolHash;RGroupDecomposition;SubstructLibrary;TautomerQuery;"
  "MolEnumerator;"
  "MolStandardize;FilterCatalog;Catalogs;FMCS;MolDraw2D;FileParsers;SmilesParse;MarvinParser;"
  "Depictor;SubstructMatch;ChemReactions;Fingerprints;ChemTransforms;"
  "Subgraphs;GraphMol;DataStructs;Trajectory;Descriptors;"
  "PartialCharges;MolTransforms;DistGeomHelpers;DistGeometry;"
  "ForceFieldHelpers;ForceField;EigenSolvers;Optimizer;MolAlign;O3AAlign;"
  "Alignment;SimDivPickers;RDGeometryLib;RDStreams;RDGeneral;"
)
if(RDK_USE_URF)
  set(swigRDKixLibList "${swigRDKixLibList};${RDK_URF_LIBS};")
endif(RDK_USE_URF)
set(swigRDKixLibs "")
foreach(swigRDKixLib ${swigRDKixLibList})
  set(swigRDKixLibs "${swigRDKixLibs}${swigRDKixLib}${swigRDKixLibSuffix};")
endforeach()




set(RDKix_Wrapper_Libs ${swigRDKixLibs})

message("SUFFIX: ${swigRDKixLibSuffix}")
message("JAVA_LIBS: ${RDKix_Wrapper_Libs}")


if(RDK_BUILD_SWIG_JAVA_WRAPPER)
add_subdirectory(gmwrapper)
endif()

if(RDK_BUILD_SWIG_CSHARP_WRAPPER)
add_subdirectory(csharp_wrapper)
endif()
