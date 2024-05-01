
remove_definitions(-DRDKIX_GRAPHMOL_BUILD)
rdkix_library(Trajectory Trajectory.cpp
              LINK_LIBRARIES GraphMol RDGeneral )
target_compile_definitions(Trajectory PRIVATE RDKIX_TRAJECTORY_BUILD)

rdkix_headers(Snapshot.h Trajectory.h DEST GraphMol/Trajectory)

rdkix_test(trajectoryTest trajectoryTest.cpp
           LINK_LIBRARIES Trajectory FileParsers
           SmilesParse SubstructMatch ForceFieldHelpers )
