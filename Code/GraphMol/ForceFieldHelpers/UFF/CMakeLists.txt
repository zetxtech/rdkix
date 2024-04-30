remove_definitions(-DRDKIX_FORCEFIELDHELPERS_BUILD)
rdkix_test(testUFFForceFieldHelpers testUFFHelpers.cpp
           LINK_LIBRARIES 
           ForceFieldHelpers
           DistGeomHelpers FileParsers MolTransforms SmilesParse)
