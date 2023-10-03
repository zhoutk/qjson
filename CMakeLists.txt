cmake_minimum_required(VERSION 3.8)
file(READ "version" ver)
string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})
message("version: ${ver_major}.${ver_minor}.${ver_patch}")
project(qjson VERSION ${ver_major}.${ver_minor}.${ver_patch})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

message(test-env-var-QTDIR -----------> $ENV{QTDIR})
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") 
	add_definitions(-D__WIN__)
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
	add_compile_options(/wd26451 /wd26812 /wd4305 /wd4244 /wd6031 /wd26495 /wd26439)

    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(UNIX)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-D__DEBUG__)
	message(STATUS "Debug mode ...")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Release mode...")
endif()

#打开全局moc
set(CMAKE_AUTOMOC ON)

message("Looking for Qt...")
find_package(Qt5 REQUIRED Widgets)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

find_package(Qt5Core)

include_directories($ENV{QTDIR}/include)
include_directories($ENV{QTDIR}/include/QtCore)

FILE(GLOB_RECURSE SRC_FILES "./src/*.cpp") 
FILE(GLOB_RECURSE HEAD_FILES "./src/*.h") 

source_group("Head Files" FILES ${HEAD_FILES} )

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Core)

##--------------------- Build unit tests -----------------------------------##
option(CMAKE_QJSON_ENABLE_TEST "Whether to enable unit tests" ON)
if (CMAKE_QJSON_ENABLE_TEST)
    message(STATUS "Unit tests enabled")
    enable_testing()
    add_subdirectory(thirds/googletest-1.14.0 EXCLUDE_FROM_ALL)
    include_directories(thirds/googletest-1.14.0/googletest/include)

    #add_executable(test_test tests/test_test.cpp)
    #target_link_libraries(test_test gtest gtest_main)
    #add_test(NAME test_test COMMAND test_test)

    add_executable(test_object tests/test_object.cpp)
    target_link_libraries(test_object gtest gtest_main Qt5::Core)
    add_test(NAME test_object COMMAND test_object)

    #add_executable(test_array tests/test_array.cpp)
    #target_link_libraries(test_array gtest gtest_main)
    #add_test(NAME test_array COMMAND test_array)

    #add_executable(test_parse tests/test_parse.cpp)
    #target_link_libraries(test_parse gtest gtest_main)
    #add_test(NAME test_parse COMMAND test_parse)

endif()