include_directories( ${RDKix_ExternalDir}/AvalonTools )
rdkix_python_extension(pyAvalonTools
                       pyAvalonTools.cpp
                       DEST Avalon
                       LINK_LIBRARIES
                       AvalonLib )
add_pytest(pyAvalonTools
         ${CMAKE_CURRENT_SOURCE_DIR}/testAvalonTools.py)
