cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(GCC_VERSION 10.2)

add_compile_options(
        -std=gnu++17
        -Wall
)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

message(STATUS "Checking/installing dependencies")
file(COPY conanfile.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY build.profile DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
execute_process(
        COMMAND conan install .
        -g cmake -g virtualenv
        -s compiler=gcc -s compiler.version=${GCC_VERSION}
        -s build_type=Release
        --profile build.profile
        RESULT_VARIABLE CONAN_EXIT_CODE)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

set(CMAKE_C_COMPILER ${CONAN_BIN_DIRS_GCC}/gcc)
set(CMAKE_CXX_COMPILER ${CONAN_BIN_DIRS_GCC}/g++)

project(krypto C CXX)

conan_check_compiler()
conan_define_targets()

message(STATUS "Updating RPATH settings")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
foreach (x IN LISTS CONAN_LIB_DIRS)
    message(STATUS "ADDING ${x}")
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(DV_RPATH_OPTIONS "-Wl,--enable-new-dtags -Wl,-rpath,${x}")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}    ${DV_RPATH_OPTIONS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${DV_RPATH_OPTIONS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${DV_RPATH_OPTIONS}")
    message(STATUS "DONE ADDING ${x}")
endforeach (x)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_VERBOSE_MAKEFILE true)

include_directories(include)
include_directories(${CONAN_INCLUDE_DIRS})

file(GLOB_RECURSE KRYPTO_INCLUDES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE KRYPTO_SOURCES CONFIGURE_DEPENDS "src/krypto/*.cpp")


add_library(krypto STATIC ${KRYPTO_INCLUDES} ${KRYPTO_SOURCES})

target_link_libraries(krypto Threads::Threads)

add_library(crypto STATIC IMPORTED include/krypto/mktdata/coinbase/book_builder.h)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${CONAN_LIB_DIRS_OPENSSL}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${CONAN_LIB_DIRS_OPENSSL}/libssl.a)

set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}:")

find_library(GL GL /usr/lib/x86_64-linux-gnu)

add_executable(krypto-coinbase-mktdata src/apps/mktdata/coinbase_main.cpp)
target_link_libraries(krypto-coinbase-mktdata krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-instruments-server src/apps/instruments/server_main.cpp include/krypto/root_certificates.h src/krypto/utils/common.cpp)
target_link_libraries(krypto-instruments-server krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-order-gateway src/apps/orders/order_gateway_main.cpp)
target_link_libraries(krypto-order-gateway krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-mktdata-gateway src/apps/mktdata/mktdata_gateway_main.cpp)
target_link_libraries(krypto-mktdata-gateway krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-ws-server src/apps/ws/ws_server_main.cpp)
target_link_libraries(krypto-ws-server krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-sim-orders src/apps/orders/sim_main.cpp)
target_link_libraries(krypto-sim-orders krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-coinbase-orders src/apps/orders/coinbase_main.cpp)
target_link_libraries(krypto-coinbase-orders krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq  CONAN_PKG::cryptopp Threads::Threads)


add_executable(krypto-pricing-server src/apps/pricing/krypto_pricing_server_main.cpp)
target_link_libraries(krypto-pricing-server krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

# Unit tests
add_executable(krypto-test-auth src/tests/test_auth.cpp)
target_link_libraries(krypto-test-auth krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::cryptopp CONAN_PKG::catch2 Threads::Threads)

# Examples

add_executable(mktdata-printer src/examples/mktdata_printer_main.cpp)
target_link_libraries(mktdata-printer krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(query-instruments src/examples/query_instruments_main.cpp)
target_link_libraries(query-instruments krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(config-loader src/examples/config_loader_main.cpp include/krypto/utils/types.h)
target_link_libraries(config-loader krypto)

add_executable(sim-order-client src/examples/sim_order_client_main.cpp)
target_link_libraries(sim-order-client krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(theo-client src/examples/theo_printer_main.cpp)
target_link_libraries(theo-client krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(coinbase-api-test src/examples/coinbase_api_main.cpp)
target_link_libraries(coinbase-api-test krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::cryptopp Threads::Threads)

add_executable(timer-test src/examples/timer_test_main.cpp)
target_link_libraries(timer-test krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::cryptopp Threads::Threads)
