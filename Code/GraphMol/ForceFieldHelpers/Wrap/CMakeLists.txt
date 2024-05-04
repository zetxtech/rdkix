remove_definitions(-DRDKIX_FORCEFIELDHELPERS_BUILD)
rdkix_python_extension(rdForceFieldHelpers rdForceFields.cpp
                       DEST Chem
                       LINK_LIBRARIES
                       ForceFieldHelpers ) 

add_pytest(pyForceFieldHelpers ${CMAKE_CURRENT_SOURCE_DIR}/testHelpers.py)




