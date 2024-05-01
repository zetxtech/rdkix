remove_definitions(-DRDKIX_FORCEFIELD_BUILD)
rdkix_python_extension(rdForceField ForceField.cpp
                       DEST ForceField
                       LINK_LIBRARIES ForceFieldHelpers )
add_pytest(pyForceFieldConstraints ${CMAKE_CURRENT_SOURCE_DIR}/testConstraints.py)
