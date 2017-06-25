cmake_minimum_required(VERSION 3.8)
project(urdl LANGUAGES CXX VERSION 0.1.0)
set(TARGET_NAME Urdl)
set(EXPORT_NAME Urdl)
set(NAMESPACE Asio)

find_package(ACM REQUIRED NO_MODULE)
include(FindAPackage)
find_a_package(Asio 1.10.8 REQUIRED)

add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME} INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)
target_link_libraries(${TARGET_NAME} INTERFACE Asio::AsioSSL)
target_sources(${TARGET_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/urdl>
        $<INSTALL_INTERFACE:include/urdl>)
target_compile_definitions(${TARGET_NAME} INTERFACE ASIO_HEADER_ONLY ASIO_STANDALONE)
target_compile_features(${TARGET_NAME} INTERFACE cxx_std_14)

if (NOT DEFINED DISABLE_SSL)
    find_a_package(OpenSSL REQUIRED)
    target_link_libraries(${TARGET_NAME} INTERFACE OpenSSL::SSL)
else()
    target_compile_definitions(${TARGET_NAME} INTERFACE DISABLE_SSL)
endif()

install(DIRECTORY include/urdl DESTINATION include)
include(ExportTarget)


option(BUILD_EXAMPLES "Build the C++11 example applications")
option(BUILD_TESTS "Build and run the unit tests" ON)

if (BUILD_EXAMPLES OR BUILD_TESTS)
find_package(Boost 1.58 REQUIRED COMPONENTS coroutine system filesystem unit_test_framework)
endif()

if (BUILD_EXAMPLES)
add_executable(get1 example/get1.cpp)
target_link_libraries(get1 ${TARGET_NAME})
add_executable(get2 example/get2.cpp)
target_link_libraries(get2 ${TARGET_NAME})
add_executable(multiget1 example/multiget1.cpp)
target_link_libraries(multiget1 ${TARGET_NAME})
add_executable(multiget2 example/multiget2.cpp)
target_link_libraries(multiget2 ${TARGET_NAME} Boost::boost Boost::coroutine)
endif ()

if (BUILD_TESTS)
enable_testing()
add_executable(test_istream
        test/istream.cpp)
add_executable(test_istreambuf
        test/istreambuf.cpp)
add_executable(test_read_stream
        test/read_stream.cpp)
add_executable(test_option_set
        test/option_set.cpp)
add_executable(test_url
        test/url.cpp)


target_link_libraries(test_istream
        ${TARGET_NAME} Boost::boost Boost::filesystem
        Boost::system Boost::unit_test_framework)
target_link_libraries(test_istreambuf
        ${TARGET_NAME} Boost::boost Boost::filesystem
        Boost::system Boost::unit_test_framework)
target_link_libraries(test_read_stream
        ${TARGET_NAME} Boost::boost Boost::filesystem
        Boost::system Boost::unit_test_framework)
target_link_libraries(test_option_set
        ${TARGET_NAME} Boost::boost Boost::filesystem
        Boost::system Boost::unit_test_framework)
target_link_libraries(test_url
        ${TARGET_NAME} Boost::boost Boost::filesystem
        Boost::system Boost::unit_test_framework)
add_test(NAME test_istrem COMMAND test_istream)
add_test(NAME test_istrembuf COMMAND test_istreambuf)
add_test(NAME test_read_stream COMMAND test_read_stream)
add_test(NAME test_option_set COMMAND test_option_set)
add_test(NAME test_url COMMAND test_url)
endif()
