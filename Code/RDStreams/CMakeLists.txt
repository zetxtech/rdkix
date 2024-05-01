
rdkix_library(RDStreams streams.cpp)
target_compile_definitions(RDStreams PRIVATE RDKIX_RDSTREAMS_BUILD)

rdkix_headers(streams.h DEST RDStreams)
