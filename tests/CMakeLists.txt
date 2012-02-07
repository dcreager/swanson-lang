# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------
# Copyright © 2011, RedJack, LLC.
# All rights reserved.
#
# Please see the LICENSE.txt file in this distribution for license
# details.
# ----------------------------------------------------------------------

include_directories(../include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)

#-----------------------------------------------------------------------
# Check for prerequisite libraries

find_package(PkgConfig)

pkg_check_modules(CHECK REQUIRED check)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

#-----------------------------------------------------------------------
# Build the test cases

macro(make_test test_name)
    add_executable(${test_name} ${test_name}.c)
    target_link_libraries(
        ${test_name} ${CHECK_LIBRARIES}
        libswanson liblagavulin
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro(make_test)

make_test(test-blocks)
make_test(test-engine)
make_test(test-metamodel)
make_test(test-s0-parser)
make_test(test-scopes)
make_test(test-types)

add_executable(simple-speed-test simple-speed-test.c)
target_link_libraries(simple-speed-test libswanson liblagavulin)

add_executable(s0-evaluate s0-evaluate.c)
target_link_libraries(s0-evaluate libswanson)

add_executable(type-satisfies type-satisfies.c)
target_link_libraries(type-satisfies libswanson)

#-----------------------------------------------------------------------
# Command-line tests

# This will automatically find any test cases in tests/ that should be
# run using the run-script helper script.  This lets you pass in
# arbitary command-line options and stdin to a compiled executable, and
# verify its stdout and stderr.  The expectation is that your
# directories will be named tests/${category}/${test_name}

configure_file(run-test ${CMAKE_BINARY_DIR}/run-test COPYONLY)
configure_file(train ${CMAKE_BINARY_DIR}/train COPYONLY)

file(GLOB_RECURSE OUTPUT_TESTS command)
foreach(OUTPUT_TEST ${OUTPUT_TESTS})
    get_filename_component(test_dir ${OUTPUT_TEST} PATH)
    get_filename_component(test_suffix ${test_dir} NAME)
    get_filename_component(test_parent_dir ${test_dir} PATH)
    get_filename_component(test_prefix ${test_parent_dir} NAME)
    set(test_name "${test_prefix}:${test_suffix}")

    add_test(NAME ${test_name}
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
             COMMAND ${CMAKE_BINARY_DIR}/run-test ${test_dir})
endforeach(OUTPUT_TEST ${OUTPUT_TESTS})
