
rdkix_library(Fingerprints
              Fingerprints.cpp PatternFingerprints.cpp MorganFingerprints.cpp
              AtomPairs.cpp MACCS.cpp MHFP.cpp FingerprintGenerator.cpp 
              AtomPairGenerator.cpp MorganGenerator.cpp RDKixFPGenerator.cpp 
              FingerprintUtil.cpp TopologicalTorsionGenerator.cpp
              LINK_LIBRARIES DataStructs Subgraphs SubstructMatch SmilesParse GraphMol RDGeneral
              )
target_compile_definitions(Fingerprints PRIVATE RDKIX_FINGERPRINTS_BUILD)

rdkix_headers(AtomPairs.h
              Fingerprints.h
              MorganFingerprints.h
              MACCS.h
              MHFP.h
              FingerprintGenerator.h
              AtomPairGenerator.h
              MorganGenerator.h
              RDKixFPGenerator.h
              TopologicalTorsionGenerator.h
              FingerprintUtil.h
              DEST GraphMol/Fingerprints)

rdkix_test(testFingerprints test1.cpp LINK_LIBRARIES 
           Fingerprints FileParsers )

rdkix_test(testMHFPFingerprint testMHFPFingerprint.cpp LINK_LIBRARIES 
           Fingerprints FileParsers )

rdkix_test(testFingerprintGenerators testFingerprintGenerators.cpp LINK_LIBRARIES 
           Fingerprints FileParsers )

rdkix_catch_test(fpTestCatch catch_tests.cpp LINK_LIBRARIES Fingerprints FileParsers)

rdkix_catch_test(fpGenTestCatch fpgen_catch_tests.cpp LINK_LIBRARIES Fingerprints FileParsers)

if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()