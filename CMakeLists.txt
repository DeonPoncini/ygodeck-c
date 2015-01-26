cmake_minimum_required(VERSION 2.8)
project(ygodeck-c)

################################
# Compiler setup
################################

set(C_DECK_VERSION "0.0.1")

################################
# Third party libraries
################################
find_package(mindbw)
find_package(ygodata)
find_package(ygodata-c)
find_package(ygodeck)
find_package(zephyr)

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)
include_directories(${MINDBW_INCLUDE_DIRS})
include_directories(${YGODATA_INCLUDE_DIRS})
include_directories(${YGODATA-C_INCLUDE_DIRS})
include_directories(${YGODECK_INCLUDE_DIRS})
include_directories(${ZEPHYR_INCLUDE_DIRS})

################################
# Build targets
################################
add_library(ygodeck-c
    ${SRC}
    ${INCLUDES})

target_link_libraries(ygodeck-c
    ${MINDBW_LIBRARIES}
    ${YGODATA_LIBRARIES}
    ${YGODATA-C_LIBRARIES}
    ${YGODECK_LIBRARIES}
    ${ZEPHYR_LIBRARIES})

################################
# Generate database file
################################

make_directory(${CMAKE_CURRENT_BINARY_DIR}/test)
add_custom_command(TARGET ygodeck-c POST_BUILD
    COMMAND rm ARGS -f ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    COMMAND ${YGODECK_PATH}/load-db
    ARGS ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    ${MINDBW_PATH}/sqlite3-execute
    ${YGODECK_DATA}
)

################################
# Export library
################################

export_project(
    NAME ygodeck-c
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS ygodeck-c
    VERSION ${C_DECK_VERSION}
)

################################
# Unit tests
################################
add_unit_tests(NAME ygodeck-c)
