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

file(GLOB_RECURSE LIBSWANSON_SRC libswanson/*.c)

add_library(libswanson SHARED ${LIBSWANSON_SRC})
set_target_properties(libswanson PROPERTIES
    OUTPUT_NAME swanson
    SOVERSION 0.0.0)
target_link_libraries(libswanson
)

install(
    TARGETS
      libswanson
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

#-----------------------------------------------------------------------
# Generate the pkg-config file

set(prefix ${CMAKE_INSTALL_PREFIX})
configure_file(libswanson.pc.in libswanson.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libswanson.pc
        DESTINATION lib/pkgconfig)
