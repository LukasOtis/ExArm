cmake_minimum_required(VERSION 3.10)
project(robot_arm_description)

find_package(ament_cmake REQUIRED)
find_package(xacro REQUIRED)

install(DIRECTORY urdf rviz
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY resource
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
