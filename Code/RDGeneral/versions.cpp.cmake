#include <RDGeneral/versions.h>

const char * RDKix::rdkixVersion = "@RDKix_RELEASENAME@";

// The Boost version as detected at build time.
// CMake's Boost_LIB_VERSION is defined by the FindBoost.cmake module
// to be the same as the value from <boost/version.hpp>
const char * RDKix::boostVersion = "@Boost_LIB_VERSION@";

// The system/compiler on which RDKix was built as detected at build time.
const char * RDKix::rdkixBuild = "@RDKix_BUILDNAME@";
