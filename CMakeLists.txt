cmake_minimum_required(VERSION 3.2)
set(exe_name lab1)
project(${exe_name})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# find_package(Threads REQUIRED)
# find_package(OpenCV REQUIRED)
# find_package( Boost 1.50 COMPONENTS system REQUIRED)

# include_directories(${Boost_INCLUDE_DIR})

set(${exe_name}_deps
    # ${CMAKE_THREAD_LIBS_INIT} 
    # ${OpenCV_LIBS} 
    # ${Boost_LIBRARIES}
    -larmadillo
    )

set(${exe_name}_srcs 
    lab1.cpp
    )

source_group("" FILES ${${exe_name}_srcs})

add_executable(${exe_name} ${${exe_name}_srcs} )
target_link_libraries( ${exe_name} ${${exe_name}_deps} )
