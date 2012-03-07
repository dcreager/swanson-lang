# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------
# Copyright © 2011, RedJack, LLC.
# All rights reserved.
#
# Please see the LICENSE.txt file in this distribution for license
# details.
# ----------------------------------------------------------------------

include_directories(../include libswanson)

#-----------------------------------------------------------------------
# Build the libraries

macro(ragel ragel_output)
    set(_output ${ragel_output})
    set(_src ${ragel_output}.rl)
    add_custom_command(
        OUTPUT ${_output}
        COMMAND ${RAGEL} -C -G1 -o ${_output} ${_src}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${_src}
        COMMENT "Ragel ${ragel_output}"
    )
    list(APPEND RAGEL_SRC ${_output})
endmacro(ragel)

file(GLOB_RECURSE LIBSWANSON_SRC libswanson/*.c)
ragel(libswanson/s0-parser.c)

add_library(libswanson SHARED ${LIBSWANSON_SRC} ${RAGEL_SRC})
set_target_properties(libswanson PROPERTIES
    OUTPUT_NAME swanson
    SOVERSION 0.0.0)
target_link_libraries(libswanson
    ${CORK_LIBRARIES}
)


file(GLOB_RECURSE LIBLAGAVULIN_SRC liblagavulin/*.c)

set_property(
    SOURCE liblagavulin/block.c
    PROPERTY COMPILE_FLAGS -fomit-frame-pointer
)

add_library(liblagavulin SHARED ${LIBLAGAVULIN_SRC})
set_target_properties(liblagavulin PROPERTIES
    OUTPUT_NAME lagavulin
    SOVERSION 0.0.0)
target_link_libraries(liblagavulin
    ${CORK_LIBRARIES} libswanson
)


install(
    TARGETS liblagavulin libswanson
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

#-----------------------------------------------------------------------
# Generate the pkg-config files

set(prefix ${CMAKE_INSTALL_PREFIX})

configure_file(liblagavulin.pc.in liblagavulin.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/liblagavulin.pc
        DESTINATION lib/pkgconfig)

configure_file(libswanson.pc.in libswanson.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libswanson.pc
        DESTINATION lib/pkgconfig)
