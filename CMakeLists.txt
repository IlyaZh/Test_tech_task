# CMakeList.txt: проект CMake для multi_threads_file_hash; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.12)

project ("multi_threads_file_hash")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_C_COMPILER gcc)

#set(BOOST_ROOT "D:/Development/libs/boost_1_78_0/")
set(BOOST_ROOT "E:/Libs/boost_1_78_0/")
 
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

find_package (Boost REQUIRED COMPONENTS iostreams)

include_directories (multi_threads_file_hash ${Boost_INCLUDE_DIR})
add_executable (multi_threads_file_hash 
    "main.cpp"
    "cl_parser.h"
    "cl_parser.cpp"
    "file_handler.h"
    "file_handler.cpp"
    "config.h"
    "hasher.cpp"
    "hasher.h"
    "thread_handler.cpp"
    "thread_handler.h")
target_link_libraries (multi_threads_file_hash ${Boost_LIBRARIES})

#if (CMAKE_VERSION VERSION_GREATER 3.12)
  #set_property(TARGET multi_threads_file_hash PROPERTY CXX_STANDARD 20)
#endif()

