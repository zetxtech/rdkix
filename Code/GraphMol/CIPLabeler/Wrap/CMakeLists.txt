remove_definitions(-DRDKIX_CIPLABELER_BUILD)

rdkix_python_extension(rdCIPLabeler rdCIPLabeler.cpp
                       DEST Chem
                       LINK_LIBRARIES GraphMol CIPLabeler RDBoost)

add_pytest(pyCIPLabelsWrapTest ${CMAKE_CURRENT_SOURCE_DIR}/pyCIPLabelWrapTest.py)

add_pytest(pyCIPLabelsValidation ${CMAKE_CURRENT_SOURCE_DIR}/pyCIPLabelsValidation.py)
