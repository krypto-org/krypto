cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(GCC_VERSION 8.2)

add_compile_options(
        -std=gnu++17
        -Wall
)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

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
        include/krypto/types.h
        include/krypto/serialization/serialization_generated.h
        include/krypto/logger.h
        include/krypto/mktdata/coinbase/ws.h
        include/krypto/mktdata/coinbase/book_builder.h
        include/krypto/mktdata/convert.h
        include/krypto/mktdata/book.h
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
        include/krypto/http.h src/krypto/http.cpp)
target_link_libraries(krypto Threads::Threads)

add_library(crypto STATIC IMPORTED include/krypto/mktdata/coinbase/book_builder.h)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${CONAN_LIB_DIRS_OPENSSL}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${CONAN_LIB_DIRS_OPENSSL}/libssl.a)

add_executable(krypto-coinbase-mktdata src/apps/mktdata/coinbase/mktdata.cpp)
target_link_libraries(krypto-coinbase-mktdata krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq Threads::Threads)

set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}:")

find_library(GL GL /usr/lib/x86_64-linux-gnu)

add_executable(krypto-ui src/apps/ui/main.cpp src/apps/ui/imgui_impl_glfw.cpp src/apps/ui/imgui_impl_opengl3.cpp)
target_link_libraries(krypto-ui krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::glfw CONAN_PKG::glad CONAN_PKG::imgui Threads::Threads)

add_executable(krypto-instruments src/apps/instruments/main.cpp include/krypto/root_certificates.h)
target_link_libraries(krypto-instruments krypto CONAN_PKG::boost ssl crypto dl CONAN_PKG::bzip2 CONAN_PKG::zlib CONAN_PKG::tbb CONAN_PKG::zmq CONAN_PKG::cppzmq CONAN_PKG::glfw CONAN_PKG::glad CONAN_PKG::imgui Threads::Threads)