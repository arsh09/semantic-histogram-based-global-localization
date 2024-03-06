cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)
set(DCMAKE_BUILD_TYPE Release)

project(mapAlignment)

set (Pangolin_DIR "/usr/local/lib/cmake/Pangolin/PangolinConfig.cmake")

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)


# generate library 
add_library(${PROJECT_NAME}
    # source files 
    src/matcher.cpp
    src/neighborhood.cpp
    src/pointCloudMapping.cpp
    src/registration.cpp
    src/toolfile.cpp
    # src/utils.cpp
)

add_dependencies(${PROJECT_NAME} 
  ${${PROJECT_NAME}_EXPORTED_TARGETS} 
  ${PCL_LIBRARIES} 
  ${Pangolin_LIBRARIES} 
  ${OpenCV_LIBS} 
  ${EIGEN3_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}
  ${${PROJECT_NAME}_EXPORTED_TARGETS} 
  ${PCL_LIBRARIES} 
  ${Pangolin_LIBRARIES} 
  ${OpenCV_LIBS} 
  ${EIGEN3_LIBRARIES}
)


# generate examples 
add_executable(example_1 src/main.cpp)
target_link_libraries(example_1 ${PROJECT_NAME} ${PCL_LIBRARIES} ${Pangolin_LIBRARIES} ${OpenCV_LIBS} ${EIGEN3_LIBRARIES})
add_dependencies(example_1 ${PROJECT_NAME})

# generate examples 
add_executable(example_2 src/main_airsim_xview.cpp)
target_link_libraries(example_2 ${PROJECT_NAME} ${PCL_LIBRARIES} ${Pangolin_LIBRARIES} ${OpenCV_LIBS} ${EIGEN3_LIBRARIES})
add_dependencies(example_2 ${PROJECT_NAME})

