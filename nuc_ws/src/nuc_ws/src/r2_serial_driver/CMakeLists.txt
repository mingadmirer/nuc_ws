# r2_serial_driver — 纯子库，随 r2_mission 一同编译
# 不包含 package.xml，非独立 colcon 包

add_library(r2_serial_driver_core STATIC
  src/serial_interface.cpp
  src/command_encoder.cpp
)
target_include_directories(r2_serial_driver_core PUBLIC src)

# 安装到 r2_mission 的安装目录
install(TARGETS r2_serial_driver_core
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
