# find LIBEVENT
# an event notification library (http://LIBEVENT.org/)
#
# Usage:
# LIBEVENT_INCLUDE_DIRS, where to find LIBEVENT headers
# LIBEVENT_LIBRARIES, LIBEVENT libraries
# LIBEVENT_FOUND, If false, do not try to use LIBEVENT

set(LIBEVENT_ROOT CACHE PATH "Root directory of LIBEVENT installation")
set(LIBEVENT_EXTRA_PREFIXES /usr/local /opt/local "$ENV{HOME}" ${LIBEVENT_ROOT} "$ENV{LIBEVENT_ROOT_DIR}" "$ENV{LIBEVENT_ROOT}")
foreach(prefix ${LIBEVENT_EXTRA_PREFIXES})
  list(APPEND LIBEVENT_INCLUDE_PATHS "${prefix}/include")
  list(APPEND LIBEVENT_LIBRARIES_PATHS "${prefix}/lib")
endforeach()

# Looking for "event.h" will find the Platform SDK include dir on windows
# so we also look for a peer header like evhttp.h to get the right path
find_path(LIBEVENT_INCLUDE_DIRS evhttp.h event.h PATHS ${LIBEVENT_INCLUDE_PATHS})

# "lib" prefix is needed on Windows in some cases
# newer versions of LIBEVENT use three libraries
find_library(LIBEVENT_LIBRARIES NAMES event event_core event_extra LIBEVENT PATHS ${LIBEVENT_LIBRARIES_PATHS})

if (LIBEVENT_LIBRARIES AND LIBEVENT_INCLUDE_DIRS)
  set(LIBEVENT_FOUND TRUE)
  set(LIBEVENT_LIBRARIES ${LIBEVENT_LIBRARIES})
else ()
  set(LIBEVENT_FOUND FALSE)
endif ()

if (LIBEVENT_FOUND)
  if (NOT LIBEVENT_FIND_QUIETLY)
    message(STATUS "Found LIBEVENT: ${LIBEVENT_LIBRARIES}")
  endif ()
else ()
  if (LIBEVENT_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find LIBEVENT.")
  endif ()
  message(STATUS "LIBEVENT NOT found.")
endif ()

mark_as_advanced(
    LIBEVENT_LIBRARIES
    LIBEVENT_INCLUDE_DIRS
  )

