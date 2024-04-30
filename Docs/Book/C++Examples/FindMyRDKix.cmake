# Attempts to find RDKix libraries using the current value of $RDBASE
# or, failing that, a version in my home directory
# It returns the static (.a) libraries not the .so ones because that's
# easiest for shipping (on Unix anyway. This may have to change once I start
# dealing with Windows as well.)
#
# It will define
# RDKIX_FOUND as MyRDKix_FOUND if it finds everything it needs
# RDKIX_INCLUDE_DIR
# RDKIX_LIBRARIES as requested

set(RDKIX_DIR $ENV{RDBASE})
if(NOT RDKIX_DIR)
  message( WARNING "Using RDKix at /home/cosgrove/RDKix_2013_09_1" )
  set(RDKIX_DIR "/home/cosgrove/RDKix_2013_09_1")
endif(NOT RDKIX_DIR)

set(RDKIX_INCLUDE_DIR ${RDKIX_DIR}/Code)

set(RDKIX_FOUND "MyRDKix_FOUND")
# libraries, as specified in the COMPONENTS
foreach(component ${MyRDKix_FIND_COMPONENTS})
  message( "Looking for RDKix component ${component}" )
  find_file( MyRDKix_LIBRARY_${component}
    libRDKix${component}.so
    PATH ${RDKIX_DIR}/lib NO_DEFAULT_PATH)
  message("MyRDKix_LIBRARY_${component} : ${MyRDKix_LIBRARY_${component}}")
  if(NOT ${MyRDKix_LIBRARY_${component}})
    message(FATAL_ERROR "Didn't find RDKix ${component} library.")
  endif(NOT ${MyRDKix_LIBRARY_${component}})
  set(RDKIX_LIBRARIES ${RDKIX_LIBRARIES} ${MyRDKix_LIBRARY_${component}})
endforeach(component)

message("RDKIX_INCLUDE_DIR : ${RDKIX_INCLUDE_DIR}")
message("RDKIX_LIBRARIES : ${RDKIX_LIBRARIES}")
message("RDKIX_FOUND : ${RDKIX_FOUND}")
