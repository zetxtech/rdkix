file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/RDPaths.py 
"import os
# unset so to trigger exceptions and track use: RDBaseDir=os.environ['RDBASE']
RDCodeDir=os.path.join(r'${PYTHON_INSTDIR}','rdkix')
# not really hard-coded alternative RDCodeDir=os.path.dirname(__file__)
_share = os.path.join(r'${CMAKE_INSTALL_PREFIX}', r'${RDKix_ShareDir}')
RDDataDir=os.path.join(_share,'Data')
RDDocsDir=os.path.join(_share,'Docs')
RDProjDir=os.path.join(_share,'Projects')
RDContribDir=os.path.join(_share,'Contrib')
")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RDPaths.py 
  DESTINATION ${RDKix_PythonDir}
  COMPONENT python)

if(NOT RDK_INSTALL_INTREE AND RDK_INSTALL_PYTHON_TESTS)
  set(RDKix_PythonSourceTestsDir ${RDKix_PythonDir})
else()
  set(RDKix_PythonSourceTestsDir ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_pythonpytest(pythonSourceTests ${RDKix_PythonSourceTestsDir} )


add_subdirectory(Chem)
