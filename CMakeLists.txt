cmake_minimum_required(VERSION 2.8.3)
project(machine_learning_samples)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
)

file(GLOB_RECURSE dt_src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/dt/*.cpp", "${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp") 

message("dt source files: " ${dt_src_files})


add_definitions(-g)
add_executable(dt ${dt_src_files})                      

