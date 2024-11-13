remove_definitions(-DRDKIX_DISTGEOMETRY_BUILD)
rdkix_python_extension(DistGeom DistGeom.cpp
                       DEST DistanceGeometry
LINK_LIBRARIES DistGeometry )


add_pytest(pyDistGeom ${CMAKE_CURRENT_SOURCE_DIR}/rough_test.py)




