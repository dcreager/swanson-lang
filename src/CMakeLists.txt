# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------
# Copyright © 2011, RedJack, LLC.
# All rights reserved.
#
# Please see the LICENSE.txt file in this distribution for license
# details.
# ----------------------------------------------------------------------

include_directories(../include)

#-----------------------------------------------------------------------
# Build the library

file(GLOB_RECURSE LIBLAGAVULIN_SRC liblagavulin/*.c)

add_library(liblagavulin SHARED ${LIBLAGAVULIN_SRC})
set_target_properties(liblagavulin PROPERTIES
    OUTPUT_NAME lagavulin
    SOVERSION 0.0.0)
target_link_libraries(liblagavulin
    ${CORK_LIBRARIES}
)

install(
    TARGETS
      liblagavulin
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

#-----------------------------------------------------------------------
# Generate the pkg-config file

set(prefix ${CMAKE_INSTALL_PREFIX})
configure_file(liblagavulin.pc.in liblagavulin.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/liblagavulin.pc
        DESTINATION lib/pkgconfig)
