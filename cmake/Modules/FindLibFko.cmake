# Try to find Libfko

find_path(LIBFKO_INCLUDE_DIR fko.h)

find_library(LIBFKO_LIBRARY NAMES fko libfko)

set(LIBFKO_LIBRARIES ${LIBFKO_LIBRARY} )
set(LIBFKO_INCLUDE_DIRS ${LIBFKO_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibFko  DEFAULT_MSG
                                  LIBFKO_LIBRARY LIBFKO_INCLUDE_DIR)

mark_as_advanced(LIBFKO_INCLUDE_DIR LIBFKO_LIBRARY )
