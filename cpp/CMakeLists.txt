cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_compile_options(
        -std=gnu++17
        -Wall
)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

message(STATUS "Checking/installing dependencies")
file(COPY conanfile.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
execute_process(
        COMMAND conan install .
        -g cmake -g virtualenv
        -s build_type=Release
        --profile=${CMAKE_CURRENT_BINARY_DIR}/../../resources/build.profile
        --build=missing
        RESULT_VARIABLE CONAN_EXIT_CODE)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

project(krypto C CXX)

conan_check_compiler()
conan_define_targets()

message(STATUS "Updating RPATH settings")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_VERBOSE_MAKEFILE true)

include_directories(include)
include_directories(${CONAN_INCLUDE_DIRS})

file(GLOB_RECURSE KRYPTO_INCLUDES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE KRYPTO_SOURCES CONFIGURE_DEPENDS "src/krypto/*.cpp")

add_library(krypto STATIC ${KRYPTO_INCLUDES} ${KRYPTO_SOURCES})
target_link_libraries(krypto CONAN_PKG::openssl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::boost
        CONAN_PKG::tbb CONAN_PKG::zeromq CONAN_PKG::cppzmq CONAN_PKG::cryptopp
        CONAN_PKG::redis-plus-plus Threads::Threads)

add_executable(krypto-coinbase-mktdata src/apps/mktdata/coinbase_main.cpp)
target_link_libraries(krypto-coinbase-mktdata krypto)

add_executable(krypto-instruments-server src/apps/instruments/server_main.cpp)
target_link_libraries(krypto-instruments-server krypto)

add_executable(krypto-order-gateway src/apps/orders/order_gateway_main.cpp)
target_link_libraries(krypto-order-gateway krypto)

add_executable(krypto-mktdata-gateway src/apps/mktdata/mktdata_gateway_main.cpp)
target_link_libraries(krypto-mktdata-gateway krypto)

add_executable(krypto-ws-server src/apps/ws/ws_server_main.cpp)
target_link_libraries(krypto-ws-server krypto)

add_executable(krypto-sim-orders src/apps/orders/sim_main.cpp)
target_link_libraries(krypto-sim-orders krypto)

add_executable(krypto-coinbase-orders src/apps/orders/coinbase_main.cpp)
target_link_libraries(krypto-coinbase-orders krypto)

add_executable(krypto-pricing-server src/apps/pricing/krypto_pricing_server_main.cpp)
target_link_libraries(krypto-pricing-server krypto)

add_executable(krypto-mktdata-cache src/apps/mktdata/mktdata_cache_main.cpp)
target_link_libraries(krypto-mktdata-cache krypto)

# Unit tests
add_executable(krypto-test-auth src/tests/test_auth.cpp)
target_link_libraries(krypto-test-auth krypto)

add_executable(krypto-test-serialize src/tests/test_serialize.cpp)
target_link_libraries(krypto-test-serialize krypto)

# Examples

add_executable(mktdata-printer src/examples/mktdata_printer_main.cpp)
target_link_libraries(mktdata-printer krypto)

add_executable(query-instruments src/examples/query_instruments_main.cpp)
target_link_libraries(query-instruments krypto)

add_executable(config-loader src/examples/config_loader_main.cpp)
target_link_libraries(config-loader krypto)

add_executable(sim-order-client src/examples/sim_order_client_main.cpp)
target_link_libraries(sim-order-client krypto)

add_executable(theo-client src/examples/theo_printer_main.cpp)
target_link_libraries(theo-client krypto)

add_executable(coinbase-api-test src/examples/coinbase_api_main.cpp)
target_link_libraries(coinbase-api-test krypto)

add_executable(timer-test src/examples/timer_test_main.cpp)
target_link_libraries(timer-test krypto)

enable_testing()
add_test(NAME KryptoTestAuth COMMAND krypto-test-auth)
add_custom_target(run_unit_test ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS krypto-test-auth)