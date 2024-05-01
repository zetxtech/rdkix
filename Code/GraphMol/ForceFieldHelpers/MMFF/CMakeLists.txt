rdkix_test(testMMFFForceFieldHelpers testMMFFHelpers.cpp
           LINK_LIBRARIES 
           ForceFieldHelpers
           DistGeomHelpers FileParsers MolTransforms SmilesParse)

rdkix_test(testMMFFForceFieldHelpersMultiThread testMultiThread.cpp
           LINK_LIBRARIES 
           ForceFieldHelpers
           DistGeomHelpers FileParsers MolTransforms SmilesParse)
