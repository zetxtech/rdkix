
rdkix_library(ShapeHelpers ShapeEncoder.cpp ShapeUtils.cpp
              LINK_LIBRARIES MolTransforms GraphMol RDGeneral)
target_compile_definitions(ShapeHelpers PRIVATE RDKIX_SHAPEHELPERS_BUILD)

rdkix_headers(ShapeEncoder.h
              ShapeUtils.h DEST GraphMol/ShapeHelpers)

rdkix_test(testShapeHelpers testShapeHelpers.cpp
           LINK_LIBRARIES ShapeHelpers FileParsers MolAlign
SmilesParse FileParsers SubstructMatch )

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()

