#!/usr/bin/env python3

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    # Get the package directory
    pkg_share = get_package_share_directory('robot_arm_description')
    
    # Declare launch arguments
    use_sim_time = LaunchConfiguration('use_sim_time', default='false')
    use_rviz = LaunchConfiguration('use_rviz', default='true')
    use_joint_state_publisher_gui = LaunchConfiguration('use_joint_state_publisher_gui', default='true')
    
    # URDF file path
    urdf_file = os.path.join(pkg_share, 'urdf', 'robot_arm.urdf.xacro')
    
    # Robot state publisher node
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'robot_description': LaunchConfiguration('robot_description')
        }]
    )
    
    # Joint state publisher node
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        output='screen'
    )
    
    # Joint state publisher GUI node
    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        output='screen',
        condition=LaunchConfiguration('use_joint_state_publisher_gui')
    )
    
    # RViz node
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        condition=LaunchConfiguration('use_rviz'),
        arguments=['-d', os.path.join(pkg_share, 'rviz', 'robot_arm.rviz')]
    )
    
    # URDF loader node
    urdf_loader_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='urdf_loader',
        output='screen',
        parameters=[{
            'robot_description': LaunchConfiguration('robot_description')
        }]
    )
    
    # Launch arguments
    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulation (Gazebo) clock if true'
    )
    
    declare_use_rviz = DeclareLaunchArgument(
        'use_rviz',
        default_value='true',
        description='Launch RViz if true'
    )
    
    declare_use_joint_state_publisher_gui = DeclareLaunchArgument(
        'use_joint_state_publisher_gui',
        default_value='true',
        description='Launch joint state publisher GUI if true'
    )
    
    # Load URDF
    with open(urdf_file, 'r') as file:
        robot_description_content = file.read()
    
    declare_robot_description = DeclareLaunchArgument(
        'robot_description',
        default_value=robot_description_content,
        description='Robot description'
    )
    
    return LaunchDescription([
        declare_use_sim_time,
        declare_use_rviz,
        declare_use_joint_state_publisher_gui,
        declare_robot_description,
        robot_state_publisher_node,
        joint_state_publisher_node,
        joint_state_publisher_gui_node,
        rviz_node
    ])