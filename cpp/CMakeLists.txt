cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(GCC_VERSION 8.2)

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

add_library(krypto STATIC
        include/krypto/serialization/serialization_generated.h
        include/krypto/logger.h
        include/krypto/mktdata/coinbase/ws.h
        include/krypto/mktdata/coinbase/book_builder.h
        include/krypto/mktdata/convert.h
        include/krypto/network/publisher.h
        include/krypto/network/subscriber.h
        include/krypto/network/proxy.h
        include/krypto/network/mktdata/incr.h
        include/krypto/network/mktdata/snap.h
        include/krypto/network/mktdata/top_of_book.h
        src/krypto/logger.cpp
        src/krypto/mktdata/coinbase/ws.cpp
        src/krypto/network/proxy.cpp
        src/krypto/network/mktdata/incr.cpp
        src/krypto/network/mktdata/snap.cpp
        src/krypto/network/mktdata/top_of_book.cpp
        include/krypto/http.h src/krypto/http.cpp
        include/krypto/config.h
        include/krypto/exchanges/factory.h
        include/krypto/exchanges/base.h
        include/krypto/utils/enum.h
        include/krypto/instruments/loader.h
        src/krypto/instruments/loader.cpp
        src/krypto/config.cpp
        include/krypto/network/rpc/client.h
        src/krypto/network/helper.cpp
        include/krypto/network/helper.h
        include/krypto/network/rpc/worker.h
        include/krypto/network/rpc/broker.h
        include/krypto/utils/message_types.h
        include/krypto/instruments/protocol.h
        src/krypto/instruments/client.cpp
        include/krypto/instruments/client.h
        include/krypto/instruments/server.h
        src/krypto/instruments/server.cpp
        include/krypto/utils/common.h
        src/krypto/utils/common.cpp
        include/krypto/exchanges/coinbase.h
        include/krypto/mktdata/coinbase/server.h
        include/krypto/mktdata/ws_server.h
        src/krypto/mktdata/ws_server.cpp
        include/krypto/utils/date_time.h
        src/krypto/utils/date_time.cpp
        include/krypto/mktdata/protocol.h
        src/krypto/mktdata/protocol.cpp
        src/krypto/instruments/protocol.cpp
        include/krypto/orders/sim/server.h
        include/krypto/orders/protocol.h
        src/krypto/orders/protocol.cpp
        src/krypto/orders/sim/server.cpp
        include/krypto/orders/client.h
        include/krypto/utils/id_gen.h
        src/krypto/orders/client.cpp)
target_link_libraries(krypto Threads::Threads)

add_library(crypto STATIC IMPORTED include/krypto/mktdata/coinbase/book_builder.h)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${CONAN_LIB_DIRS_OPENSSL}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${CONAN_LIB_DIRS_OPENSSL}/libssl.a)

add_executable(krypto-coinbase-mktdata src/apps/mktdata/coinbase_main.cpp)
target_link_libraries(krypto-coinbase-mktdata krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}:")

find_library(GL GL /usr/lib/x86_64-linux-gnu)

#add_executable(krypto-ui src/apps/ui/main.cpp src/apps/ui/imgui_impl_glfw.cpp src/apps/ui/imgui_impl_opengl3.cpp)
#target_link_libraries(krypto-ui krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::glfw CONAN_PKG::glad CONAN_PKG::imgui Threads::Threads)

# Apps

add_executable(imgui-demo src/apps/ui/demo.cpp src/apps/ui/imgui_impl_glfw.cpp src/apps/ui/imgui_impl_opengl3.cpp)
target_link_libraries(imgui-demo CONAN_PKG::glfw CONAN_PKG::glad CONAN_PKG::imgui Threads::Threads)

add_executable(krypto-instruments-server src/apps/instruments/server_main.cpp include/krypto/root_certificates.h src/krypto/utils/common.cpp)
target_link_libraries(krypto-instruments-server krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::glfw CONAN_PKG::glad CONAN_PKG::imgui Threads::Threads)

add_executable(krypto-broker src/apps/network/broker_main.cpp)
target_link_libraries(krypto-broker krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-proxy src/apps/network/proxy_main.cpp)
target_link_libraries(krypto-proxy krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-ws-server src/apps/ws/ws_server_main.cpp)
target_link_libraries(krypto-ws-server krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(krypto-sim-orders src/apps/orders/sim_main.cpp)
target_link_libraries(krypto-sim-orders krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)


# Examples

add_executable(mktdata-printer src/examples/mktdata_printer_main.cpp)
target_link_libraries(mktdata-printer krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(query-instruments src/examples/query_instruments_main.cpp)
target_link_libraries(query-instruments krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

add_executable(config-loader src/examples/config_loader_main.cpp include/krypto/utils/types.h)
target_link_libraries(config-loader krypto)

add_executable(sim-order-client src/examples/sim_order_client_main.cpp)
target_link_libraries(sim-order-client krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)
