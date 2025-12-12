import math
from typing import List

import rclpy
from rclpy.node import Node
from rclpy.duration import Duration

from trajectory_msgs.msg import JointTrajectory, JointTrajectoryPoint


class SweepPublisher(Node):
    """Publishes a repeating trajectory that excites both joints."""

    def __init__(self) -> None:
        super().__init__('test_arm_sweep')

        self.declare_parameter('base_period', 8.0)
        self.declare_parameter('pivot_period', 6.0)
        self.declare_parameter('publish_rate_hz', 2.0)
        self.declare_parameter('base_amplitude_deg', 90.0)
        self.declare_parameter('pivot_min_deg', 25.0)
        self.declare_parameter('pivot_max_deg', 130.0)

        self._base_period = float(self.get_parameter('base_period').value)
        self._pivot_period = float(self.get_parameter('pivot_period').value)
        self._publish_rate = float(self.get_parameter('publish_rate_hz').value)
        self._base_amp = math.radians(float(self.get_parameter('base_amplitude_deg').value))

        pivot_min = math.radians(float(self.get_parameter('pivot_min_deg').value))
        pivot_max = math.radians(float(self.get_parameter('pivot_max_deg').value))
        self._pivot_mid = 0.5 * (pivot_min + pivot_max)
        self._pivot_amp = 0.5 * (pivot_max - pivot_min)

        self._pub = self.create_publisher(JointTrajectory, 'command/joint_trajectory', 10)

        if self._publish_rate <= 0.0:
            self.get_logger().warn('publish_rate_hz <= 0, defaulting to 1 Hz')
            self._publish_rate = 1.0

        self._start_time = self.get_clock().now()
        self._timer = self.create_timer(1.0 / self._publish_rate, self._timer_callback)
        self.get_logger().info(
            'SweepPublisher ready: base_amp=%.2f rad pivot_mid=%.2f rad' %
            (self._base_amp, self._pivot_mid)
        )

    def _timer_callback(self) -> None:
        now = self.get_clock().now()
        elapsed = (now - self._start_time).nanoseconds * 1e-9

        base_angle = self._base_amp * math.sin(2.0 * math.pi * elapsed / self._base_period)
        pivot_angle = self._pivot_mid + self._pivot_amp * math.sin(
            2.0 * math.pi * elapsed / self._pivot_period + math.pi / 2.0
        )

        msg = JointTrajectory()
        msg.header.stamp = now.to_msg()
        msg.joint_names = ['base_rotation', 'arm_pivot']
        point = JointTrajectoryPoint()
        point.positions = self._clamp_angles([base_angle, pivot_angle])
        point.time_from_start = Duration(seconds=1.0 / self._publish_rate).to_msg()
        msg.points = [point]

        self._pub.publish(msg)

    def _clamp_angles(self, angles: List[float]) -> List[float]:
        """Clamp pivot angle to the allowed mechanical window."""
        base, pivot = angles
        pivot_clamped = max(min(pivot, self._pivot_mid + self._pivot_amp), self._pivot_mid - self._pivot_amp)
        return [base, pivot_clamped]


def main() -> None:
    rclpy.init()
    node = SweepPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Shutting down sweep publisher')
    finally:
        node.destroy_node()
        rclpy.shutdown()
