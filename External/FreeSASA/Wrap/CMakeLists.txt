include_directories( ${RDKix_ExternalDir}/FreeSASA )
rdkix_python_extension(rdFreeSASA 
                       rdFreeSASA.cpp 
                       DEST Chem
                       LINK_LIBRARIES 
                       FreeSASALib )
add_pytest(pyFreeSASA
         ${CMAKE_CURRENT_SOURCE_DIR}/testFreeSASA.py)
