#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description() -> LaunchDescription:
    use_sim_time = LaunchConfiguration('use_sim_time')
    use_gui = LaunchConfiguration('use_joint_state_publisher_gui')
    use_rviz = LaunchConfiguration('use_rviz')
    model_file = LaunchConfiguration('model')
    rviz_config = LaunchConfiguration('rviz_config')
    default_model = PathJoinSubstitution(
        [FindPackageShare('robot_arm_description'), 'urdf', 'robot_arm.urdf.xacro'],
    )
    default_rviz = PathJoinSubstitution(
        [FindPackageShare('robot_arm_description'), 'rviz', 'robot_arm.rviz'],
    )

    robot_description = ParameterValue(
        Command([FindExecutable(name='xacro'), ' ', model_file]),
        value_type=str,
    )

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time', default_value='false', description='Use simulation time if true'
    )
    declare_use_gui = DeclareLaunchArgument(
        'use_joint_state_publisher_gui',
        default_value='true',
        description='Launch the joint_state_publisher GUI slider',
    )
    declare_use_rviz = DeclareLaunchArgument(
        'use_rviz', default_value='true', description='Launch RViz with the arm view'
    )
    declare_model = DeclareLaunchArgument(
        'model', default_value=default_model, description='Absolute path to the arm Xacro'
    )
    declare_rviz_config = DeclareLaunchArgument(
        'rviz_config', default_value=default_rviz, description='RViz configuration file'
    )

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'use_sim_time': use_sim_time, 'robot_description': robot_description}],
        output='screen',
    )

    joint_state_publisher = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        condition=UnlessCondition(use_gui),
    )

    joint_state_publisher_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        condition=IfCondition(use_gui),
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_config],
        output='screen',
        condition=IfCondition(use_rviz),
    )

    return LaunchDescription(
        [
            declare_use_sim_time,
            declare_use_gui,
            declare_use_rviz,
            declare_model,
            declare_rviz_config,
            robot_state_publisher,
            joint_state_publisher,
            joint_state_publisher_gui,
            rviz_node,
        ]
    )
