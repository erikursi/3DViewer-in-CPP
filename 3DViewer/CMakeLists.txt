cmake_minimum_required(VERSION 3.5)

# App
project(3DViewer VERSION 0.1 LANGUAGES C CXX)

execute_process(COMMAND qmake -query QT_INSTALL_PREFIX
                OUTPUT_VARIABLE QT_INSTALL_PREFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE)

                

set(CMAKE_PREFIX_PATH ${QT_INSTALL_PREFIX})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Debug by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(SRC_DIR "${CMAKE_SOURCE_DIR}/sources")
set(TESTS_DIR "${CMAKE_SOURCE_DIR}/tests")
set(SAMPLES_DIR "${CMAKE_SOURCE_DIR}/samples")

set(MODELS_DIR "${SRC_DIR}/model")
set(CONTROLLERS_DIR "${SRC_DIR}/controller")
set(VIEWS_DIR "${SRC_DIR}/view")
set(SHARED_DIR "${SRC_DIR}/shared")
set(FORMS_DIR "${SRC_DIR}/forms")
set(LIB_DIR "${SRC_DIR}/lib")


set(RESOURCES_DIR "${SRC_DIR}/resources")
set(SHADERS_DIR "${RESOURCES_DIR}/shaders")
set(STYLES_DIR "${RESOURCES_DIR}/styles")


set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

set(PROJECT_DIRS
    "${MODELS_DIR}"
    "${VIEWS_DIR}"
    "${CONTROLLERS_DIR}"
    "${SHARED_DIR}"
    "${FORMS_DIR}"
    "${SHADERS_DIR}"
    "${STYLES_DIR}"
)

set(CXX_FLAGS -Wall -Werror -Wextra -Wpedantic -g -Wno-error=deprecated-declarations)
set(GCOVR_FLAGS --coverage -fprofile-arcs -ftest-coverage -O0 -fPIC) 
set(GTEST_FLAGS "")
set(OPEN "")

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
  SET(GTEST_FLAGS -lgtest_main -lgtest -lpthread -lm)
  SET(OPEN xdg-open)
else()
  SET(GTEST_FLAGS -lgtest_main -lgtest)
  SET(OPEN open)
endif()

set(SOURCE_FILES "")
set(HEADERS "")
set(FORMS "")
set(SHADERS "")
set(RESOURCES "")
set(STYLES "")

foreach(dir ${PROJECT_DIRS})
    file(GLOB_RECURSE FILES "${dir}/*.cpp" "${dir}/*.cc")
    list(APPEND SOURCE_FILES ${FILES})

    file(GLOB_RECURSE FILES "${dir}/*.h" "${dir}/*.hpp" "${dir}/*.tpp")
    list(APPEND HEADERS ${FILES})

    file(GLOB_RECURSE FILES "${dir}/*.ui")
    list(APPEND FORMS ${FILES})

    file(GLOB_RECURSE FILES "${dir}/*.vs" "${dir}/*.fs" "${dir}/*.frag" "${dir}/*.vert")
    list(APPEND SHADERS ${FILES})

    file(GLOB_RECURSE FILES "${dir}/*.qss")
    list(APPEND STYLES ${FILES})
endforeach()


set(PROJECT_SOURCES
    ${SOURCE_FILES}
    ${HEADERS}
    ${FORMS}
    ${SHADERS}
    ${STYLES}
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/resources.qrc
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

add_subdirectory("${LIB_DIR}/QtGifImage")

# add_library(SOURCE_REPORT_OBJS OBJECT ${SOURCE_FILES} ${HEADERS})
# target_compile_options(SOURCE_REPORT_OBJS PRIVATE ${CXX_FLAGS} ${GCOVR_FLAGS})

# add_library(SOURCE_OBJS OBJECT ${SOURCE_FILES} ${HEADERS})
# target_compile_options(SOURCE_OBJS PRIVATE ${CXX_FLAGS})

# add_library(TEST_SOURCE_OBJS OBJECT ${TEST_SOURCE_FILES} ${TEST_HEADERS})
# target_compile_options(TEST_SOURCE_OBJS  PRIVATE ${CXX_FLAGS})


add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/resources.qrc
)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR} ${PROJECT_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
    GIFLIB
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Tests
set(TEST_SOURCES "")
file(GLOB_RECURSE TEST_SOURCE_FILES "${TESTS_DIR}/*.cpp" "${TESTS_DIR}/*.c")
file(GLOB_RECURSE TEST_HEADERS "${TESTS_DIR}/*.h" "${TESTS_DIR}/*.hpp")
list(APPEND TEST_SOURCES
    ${SOURCE_FILES}
    ${HEADERS}
    ${TEST_SOURCE_FILES}
    ${TEST_HEADERS}
)

# Check for pre-installed GoogleTest
find_package(GTest QUIET)

if(GTest_FOUND)
    message(STATUS "Using pre-installed GoogleTest")
else()
    include(FetchContent)
    message(STATUS "Fetching GoogleTest from github")
    FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "main"
    )
    FetchContent_MakeAvailable(googletest)
endif()


# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(GoogleTest)
add_executable(tests
    ${TEST_SOURCES}
)

target_compile_options(tests PRIVATE ${CXX_FLAGS} ${GTEST_FLAGS})
target_compile_definitions(tests PRIVATE SAMPLES_DIR="${SAMPLES_DIR}")
target_include_directories(tests PRIVATE ${SRC_DIR} ${TESTS_DIR} ${PROJECT_DIRS})
gtest_discover_tests(tests)
target_link_libraries(tests PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
    GIFLIB
    GTest::gtest_main
)

# Coverage
add_executable(test_cov
    ${TEST_SOURCES}
)
target_compile_options(test_cov PRIVATE ${CXX_FLAGS} ${GCOVR_FLAGS} ${GTEST_FLAGS})
target_compile_definitions(test_cov PRIVATE SAMPLES_DIR="${SAMPLES_DIR}")
target_link_options(test_cov PRIVATE ${GCOVR_FLAGS})
target_include_directories(test_cov PRIVATE ${SRC_DIR} ${TESTS_DIR} ${PROJECT_DIRS})
gtest_discover_tests(test_cov)

target_link_libraries(test_cov PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
    GIFLIB
    GTest::gtest_main
)


# Static analysic

find_program(CPPCHECK cppcheck)

if(CPPCHECK)
  message(STATUS "cppcheck found: ${CPPCHECK}")
  list(
    APPEND CPPCHECK_ARGS
    "--enable=all"
    "--inconclusive"
    "--quiet"
    "--language=c++"
    "--std=c++17"
    "--suppress=missingIncludeSystem"
    "--suppress=unusedFunction"
    "--suppress=missingInclude"
    "--suppress=unusedStructMember"
    "--suppress=variableScope"
    "--suppress=uninitMemberVar"
    "--suppress=*:${LIB_DIR}/*"
  )
  add_custom_target(
    cppcheck
    COMMAND ${CPPCHECK} ${CPPCHECK_ARGS} ${SOURCE_FILES} ${HEADERS} ${TEST_SOURCE_FILES} ${TEST_HEADERS}
  )
else()
  message(STATUS "cppcheck not found")
endif()

# Add clang-format
find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
  message(STATUS "clang-format found: ${CLANG_FORMAT}")
  list(
    APPEND CHECK_ARGS
    "-n"
    "-style=Google"
    "--verbose"
  )
  list(
    APPEND FORMAT_ARGS
    "-i"
    "-style=Google"
    "--verbose"
  )
  add_custom_target(
    check_style
    COMMAND ${CLANG_FORMAT} ${CHECK_ARGS} ${HEADERS} ${SOURCE_FILES} ${TEST_SOURCE_FILES} ${TEST_HEADERS}
  )
  add_custom_target(
    format_style
    COMMAND ${CLANG_FORMAT} ${FORMAT_ARGS} ${HEADERS} ${SOURCE_FILES} ${TEST_SOURCE_FILES} ${TEST_HEADERS}
  )
else()
  message(STATUS "clang-format not found")
endif()

# Add coverage
# # Find the lcov tool
# find_program(LCOV lcov)
# find_program(GENHTML genhtml)

# if(LCOV)
#   message(STATUS "lcov found: ${LCOV}")
#   # Add a custom target for generating coverage
#   # add_custom_target(coverage
#   #   COMMAND ${CMAKE_COMMAND} -E env LCOV_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
#   #   COMMAND lcov --capture --directory . --output-file coverage.info
#   #   COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info # Remove external libraries
#   #   COMMAND lcov --list coverage.info
#   #   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#   #   COMMENT "Generating code coverage"
#   # )
#   add_custom_target(coverage
#         COMMAND ${LCOV} --capture --directory . --output-file coverage.info
#         COMMAND ${LCOV} --remove coverage.info 'tests/*' --output-file coverage.info # Exclude directories
#         # if(GENHTML)
#         #   COMMAND ${GENHTML} coverage.info --output-directory coverage
#         # else()
#           COMMAND ${LCOV} --list coverage.info
#         # endif()
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         COMMENT "Generating code coverage"
#     )
#   # Ensure tests are run before generating coverage
#   add_dependencies(coverage tests)
# else()
#   message(WARNING "lcov tool not found. Coverage will not be generated.")
# endif()


# Find the gcovr tool
find_program(GCOVR_PATH gcovr)

if(GCOVR_PATH)
    # Add a custom target for generating coverage
    add_custom_target(coverage
        ./test_cov
        COMMAND ${GCOVR_PATH} -r .. -e ../3DViewer/tests/ --html --html-details -o coverage.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${OPEN} coverage.html
        COMMENT "Generating code coverage with gcovr"
    )
    add_dependencies(coverage test_cov)
else()
    message(WARNING "gcovr tool not found. Coverage will not be generated.")
endif()


install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)