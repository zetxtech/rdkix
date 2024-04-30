
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-fPIC)
endif()

set( RDKixSV_SRCS rdkixsv_main.cc
  MolDisplay2DWidget.cc
  RDKixSVMainWindow.cc
  RDKixSVPanel.cc
  RDKixSVSettings.cc
  QTGet2Strings.cc
  QT4SelectItems.cc )

set( RDKixSV_INCS MolDisplay2DWidget.H
  RDKixSVMainWindow.H
  RDKixSVPanel.H
  RDKixSVSettings.H
  QTGet2Strings.H
  QT4SelectItems.H )

set( LIBS 
  ${RDKix_THREAD_LIBS} MolDraw2DQt 
  FileParsers SmilesParse Depictor RDGeometryLib
  RDGeneral SubstructMatch Subgraphs GraphMol RDGeometryLib
   z QtDependencies)

find_package(Boost ${RDK_BOOST_VERSION} COMPONENTS program_options)

add_executable( rdkixsv ${RDKixSV_SRCS}
  ${RDKixSV_MOC_SRCS} ${RDKixSV_INCS} )

target_link_libraries( rdkixsv PUBLIC MolDraw2D ${LIBS} Boost::program_options rdkix_base )
