cmake_minimum_required(VERSION 3.8)
project(r2_mission)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(detection_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

# r2_serial_driver 库（命令编码 + 串口）
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../r2_serial_driver ${CMAKE_BINARY_DIR}/r2_serial_driver)

add_executable(r2_mission_node
  src/mission_node.cpp
)
target_include_directories(r2_mission_node PRIVATE
  src
  ${CMAKE_CURRENT_SOURCE_DIR}/../r2_serial_driver/src
)
target_link_libraries(r2_mission_node r2_serial_driver_core)
ament_target_dependencies(r2_mission_node rclcpp detection_interfaces std_msgs)

add_executable(blue_mission_node
  src/bluemisson_node.cpp
)
target_include_directories(blue_mission_node PRIVATE
  src
  ${CMAKE_CURRENT_SOURCE_DIR}/../r2_serial_driver/src
)
target_link_libraries(blue_mission_node r2_serial_driver_core)
ament_target_dependencies(blue_mission_node rclcpp detection_interfaces std_msgs)

install(TARGETS r2_mission_node blue_mission_node
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/ DESTINATION share/${PROJECT_NAME}/launch)
install(DIRECTORY config/ DESTINATION share/${PROJECT_NAME}/config)

ament_package()
