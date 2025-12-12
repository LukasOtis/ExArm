#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare


def generate_launch_description() -> LaunchDescription:
    use_sim_time = LaunchConfiguration('use_sim_time')
    use_rviz = LaunchConfiguration('use_rviz')
    start_bridge = LaunchConfiguration('start_bridge')
    start_sweep = LaunchConfiguration('start_sweep')
    port = LaunchConfiguration('port')
    baudrate = LaunchConfiguration('baudrate')
    heartbeat_interval = LaunchConfiguration('heartbeat_interval')

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time', default_value='false', description='Use simulation time if true'
    )
    declare_use_rviz = DeclareLaunchArgument('use_rviz', default_value='true')
    declare_start_bridge = DeclareLaunchArgument('start_bridge', default_value='true')
    declare_start_sweep = DeclareLaunchArgument('start_sweep', default_value='true')
    declare_port = DeclareLaunchArgument('port', default_value='/dev/ttyAMA0')
    declare_baud = DeclareLaunchArgument('baudrate', default_value='115200')
    declare_heartbeat = DeclareLaunchArgument('heartbeat_interval', default_value='0.5')

    display_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare('robot_arm_bringup'), 'launch', 'display.launch.py'])
        ),
        launch_arguments={
            'use_sim_time': use_sim_time,
            'use_joint_state_publisher_gui': 'false',
            'use_rviz': use_rviz,
        }.items(),
    )

    grbl_bridge_node = Node(
        package='grbl_bridge',
        executable='grbl_bridge_node',
        name='grbl_bridge',
        parameters=[
            {
                'port': port,
                'baudrate': baudrate,
                'heartbeat_interval': heartbeat_interval,
            }
        ],
        condition=IfCondition(start_bridge),
        output='screen',
    )

    sweep_node = Node(
        package='robot_arm_bringup',
        executable='test_sweep',
        name='test_sweep',
        condition=IfCondition(start_sweep),
        output='screen',
    )

    return LaunchDescription(
        [
            declare_use_sim_time,
            declare_use_rviz,
            declare_start_bridge,
            declare_start_sweep,
            declare_port,
            declare_baud,
            declare_heartbeat,
            display_launch,
            grbl_bridge_node,
            sweep_node,
        ]
    )
