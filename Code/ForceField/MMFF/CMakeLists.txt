if(RDK_TEST_MMFF_COMPLIANCE)
  if((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_data/MMFF94_reference.log)
    OR (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test_data/MMFF94s_reference.log))
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/test_data/MMFF_reference_logs.tgz
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data
    )
  endif()
rdkix_test(testMMFFForceField testMMFFForceField.cpp
    LINK_LIBRARIES
    DistGeomHelpers ForceFieldHelpers  
    FileParsers MolTransforms SmilesParse SubstructMatch MolAlign )
endif(RDK_TEST_MMFF_COMPLIANCE)
