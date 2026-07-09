"""一键启动：崇武探幽 v1 蓝队"""

import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    pkg_share = get_package_share_directory('r2_mission')
    params_path = os.path.join(pkg_share, 'config', 'params.yaml')

    return LaunchDescription([
        Node(
            package='r2_mission',
            executable='blue_mission_node',
            name='blue_mission',
            output='screen',
            arguments=['--ros-args', '--params-file', params_path, '--log-level', 'info'],
        ),
    ])
