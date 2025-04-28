cmake_minimum_required(VERSION 3.30)
project(wang_chongxi_2024_25310619)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(HOMEBREW_PREFIX "/opt/homebrew")
    else()
        set(HOMEBREW_PREFIX "/usr/local")
    endif()
endif()

include_directories(${HOMEBREW_PREFIX}/include)
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/users)
include_directories(${CMAKE_SOURCE_DIR}/books)
include_directories(${CMAKE_SOURCE_DIR}/records)
include_directories(/usr/local/opt/openssl@3/include)

find_library(FTXUI_SCREEN_LIB NAMES ftxui-screen HINTS ${HOMEBREW_PREFIX}/lib)
find_library(FTXUI_DOM_LIB NAMES ftxui-dom HINTS ${HOMEBREW_PREFIX}/lib)
find_library(FTXUI_COMPONENT_LIB NAMES ftxui-component HINTS ${HOMEBREW_PREFIX}/lib)

if(NOT FTXUI_SCREEN_LIB OR NOT FTXUI_DOM_LIB OR NOT FTXUI_COMPONENT_LIB)
    message(FATAL_ERROR "FTXUI libraries not found. Please ensure FTXUI is installed via Homebrew.")
endif()

add_executable(wang_chongxi_2024_25310619
    Main.cpp
    users/User.cpp
    users/User.h
        books/book.cpp
        books/book.h
        pages/Pages.cpp
        pages/Pages.h
        records/record.cpp
        records/record.h
)


target_link_libraries(wang_chongxi_2024_25310619
        /usr/local/opt/openssl@3/lib/libcrypto.dylib
        ${FTXUI_SCREEN_LIB}
        ${FTXUI_DOM_LIB}
        ${FTXUI_COMPONENT_LIB}
)
