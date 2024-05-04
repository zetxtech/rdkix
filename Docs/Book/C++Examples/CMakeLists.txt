cmake_minimum_required( VERSION 3.5 )

project(RDKixSV)

set(RDBASE $ENV{RDBASE})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${RDBASE}/Code/cmake/Modules")

# note that if you haven't installed/built the toolkit with CoordGen, you'll
# have problems with this.
add_definitions("-DRDK_BUILD_COORDGEN_SUPPORT=ON")
add_compile_options(-Wall)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME ON)
find_package( Boost COMPONENTS iostreams filesystem system)
find_package( Cairo REQUIRED )

# specify where CMake can find the RDKix libraries
include_directories ( ${RDBASE}/Code ${CAIRO_INCLUDE_DIRS} )
link_directories ( ${RDBASE}/lib )

set(RDKix_LIBS RDKixChemReactions RDKixFileParsers RDKixSmilesParse RDKixDepictor
	       RDKixRDGeometryLib RDKixRDGeneral RDKixSubstructMatch RDKixSubgraphs
	       RDKixMolDraw2D RDKixGraphMol RDKixDistGeometry RDKixDistGeomHelpers
  	       RDKixMolAlign RDKixOptimizer RDKixForceField RDKixForceFieldHelpers
	       RDKixAlignment RDKixForceField  RDKixMolTransforms RDKixEigenSolvers )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

find_package (Threads)
set(RDKix_THREAD_LIBS Threads::Threads)

set( LIBS ${RDKIX_LIBRARIES} Boost::iostreams ${RDKix_THREAD_LIBS}
     ${CAIRO_LIBRARIES} z  )

include_directories(${RDKIX_INCLUDE_DIR})
foreach( exnum RANGE 1 17 )
  message( "Example : ${exnum}" )
  set( exe "example${exnum}" )
  set( src "${exe}.cpp" )
  add_executable( ${exe} ${src} )
  target_link_libraries( ${exe} ${LIBS} ${RDKix_LIBS})
endforeach( exnum )

# add_executable( test_example test_example.cpp )
# target_link_libraries( test_example ${LIBS} )
