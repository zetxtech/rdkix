# moved back library build directives here

rdkix_library(ForceFieldHelpers UFF/AtomTyper.cpp UFF/Builder.cpp
              MMFF/AtomTyper.cpp MMFF/Builder.cpp CrystalFF/TorsionAngleM6.cpp
              CrystalFF/TorsionPreferences.cpp CrystalFF/TorsionAngleContribs.cpp
              LINK_LIBRARIES SmilesParse SubstructMatch ForceField)
target_compile_definitions(ForceFieldHelpers PRIVATE RDKIX_FORCEFIELDHELPERS_BUILD)

rdkix_headers(FFConvenience.h DEST GraphMol/ForceFieldHelpers)
rdkix_headers(UFF/AtomTyper.h
              UFF/Builder.h UFF/UFF.h DEST GraphMol/ForceFieldHelpers/UFF)
rdkix_headers(MMFF/AtomTyper.h
              MMFF/Builder.h MMFF/MMFF.h DEST GraphMol/ForceFieldHelpers/MMFF)
rdkix_headers(CrystalFF/TorsionAngleM6.h CrystalFF/TorsionPreferences.h
              CrystalFF/TorsionAngleContribs.h
              DEST GraphMol/ForceFieldHelpers/CrystalFF)

rdkix_catch_test(forceFieldHelpersCatch catch_tests.cpp 
LINK_LIBRARIES ForceFieldHelpers SmilesParse ForceField )

add_subdirectory(MMFF)
add_subdirectory(UFF)
add_subdirectory(CrystalFF)
if(RDK_BUILD_PYTHON_WRAPPERS)
add_subdirectory(Wrap)
endif()
