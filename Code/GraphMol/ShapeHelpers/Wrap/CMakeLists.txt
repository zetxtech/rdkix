remove_definitions(-DRDKIX_SHAPEHELPERS_BUILD)
rdkix_python_extension(rdShapeHelpers rdShapeHelpers.cpp
                       DEST Chem
		       LINK_LIBRARIES
                       ShapeHelpers )

add_pytest(pyShapeHelpers
         ${CMAKE_CURRENT_SOURCE_DIR}/testShapeHelpers.py)


