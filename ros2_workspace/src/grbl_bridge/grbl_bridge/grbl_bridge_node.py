import math
import threading
import time
from typing import Dict, List, Optional

import rclpy
from rclpy.node import Node

from robot_arm_msgs.msg import GrblCommand, GrblStatus
from std_srvs.srv import Trigger
from trajectory_msgs.msg import JointTrajectory, JointTrajectoryPoint

try:
    import serial  # type: ignore
except ImportError:  # pragma: no cover - optional dependency at dev time
    serial = None


AXES = ['X', 'Y', 'Z', 'A', 'B', 'C']


class GrblBridge(Node):
    """Bidirectional ROS 2 ↔︎ grblHAL bridge with heartbeat and basic parsing."""

    def __init__(self) -> None:
        super().__init__('grbl_bridge')

        self.declare_parameter('port', '/dev/ttyAMA0')
        self.declare_parameter('baudrate', 115200)
        self.declare_parameter('timeout', 0.1)
        self.declare_parameter('heartbeat_interval', 0.5)
        self.declare_parameter('heartbeat_command', '?')
        self.declare_parameter('base_axis', 'X')
        self.declare_parameter('pivot_axis', 'Y')
        self.declare_parameter('incoming_degrees', True)
        self.declare_parameter('output_in_degrees', True)
        self.declare_parameter('default_feed_rate', 500.0)
        self.declare_parameter('status_frame_id', 'base_link')

        self._port = str(self.get_parameter('port').value)
        self._baudrate = int(self.get_parameter('baudrate').value)
        self._timeout = float(self.get_parameter('timeout').value)
        self._heartbeat_interval = float(self.get_parameter('heartbeat_interval').value)
        self._heartbeat_command = str(self.get_parameter('heartbeat_command').value)
        self._base_axis = str(self.get_parameter('base_axis').value).upper()
        self._pivot_axis = str(self.get_parameter('pivot_axis').value).upper()
        self._incoming_degrees = bool(self.get_parameter('incoming_degrees').value)
        self._output_degrees = bool(self.get_parameter('output_in_degrees').value)
        self._default_feed = float(self.get_parameter('default_feed_rate').value)
        self._status_frame_id = str(self.get_parameter('status_frame_id').value)

        self._serial: Optional['serial.Serial'] = None
        self._serial_lock = threading.Lock()
        self._reader_thread: Optional[threading.Thread] = None
        self._stop_event = threading.Event()
        self._last_connect_log = 0.0
        self._last_status = GrblStatus()
        self._warned_missing_serial = False

        self._status_pub = self.create_publisher(GrblStatus, 'status/grbl', 10)
        self._command_sub = self.create_subscription(
            GrblCommand, 'command/grbl', self._command_callback, 50
        )
        self._trajectory_sub = self.create_subscription(
            JointTrajectory, 'command/joint_trajectory', self._trajectory_callback, 10
        )
        self._reset_srv = self.create_service(Trigger, 'grbl/reset', self._handle_reset)

        self._heartbeat_timer = self.create_timer(self._heartbeat_interval, self._send_heartbeat)
        self._connection_timer = self.create_timer(1.0, self._ensure_connection)

    # ------------------------------------------------------------------ Lifecycle
    def destroy_node(self) -> bool:
        self._stop_reader()
        return super().destroy_node()

    # ------------------------------------------------------------------ ROS Callbacks
    def _command_callback(self, msg: GrblCommand) -> None:
        line = msg.gcode.strip()
        if not line:
            return
        self._send_line(line)

    def _trajectory_callback(self, msg: JointTrajectory) -> None:
        if not msg.points:
            return
        for point in msg.points:
            command = self._point_to_gcode(msg.joint_names, point)
            if command:
                self._send_line(command)

    def _handle_reset(self, request: Trigger.Request, response: Trigger.Response) -> Trigger.Response:
        if not self._serial:
            response.success = False
            response.message = 'Serial link not established'
            return response
        self.get_logger().warn('Sending grblHAL reset (Ctrl-X)')
        try:
            with self._serial_lock:
                self._serial.write(b'\x18')
                self._serial.flush()
            response.success = True
            response.message = 'Reset command sent'
        except Exception as exc:  # pragma: no cover
            response.success = False
            response.message = f'Failed to send reset: {exc}'
        return response

    # ------------------------------------------------------------------ Serial handling
    def _ensure_connection(self) -> None:
        if self._serial:
            return
        if serial is None:
            if not self._warned_missing_serial:
                self.get_logger().error(
                    'pyserial is not installed. Install python3-serial before running the bridge.'
                )
                self._warned_missing_serial = True
            return
        now = time.time()
        if now - self._last_connect_log < 5.0:
            return
        self._last_connect_log = now
        try:
            self.get_logger().info(f'Opening {self._port} @ {self._baudrate} baud')
            self._serial = serial.Serial(self._port, self._baudrate, timeout=self._timeout)
            self._stop_event.clear()
            self._reader_thread = threading.Thread(target=self._reader_loop, daemon=True)
            self._reader_thread.start()
            self.get_logger().info('grblHAL connection established')
        except Exception as exc:  # pragma: no cover
            self._serial = None
            self.get_logger().error(f'Failed to open serial port {self._port}: {exc}')

    def _stop_reader(self) -> None:
        self._stop_event.set()
        if self._reader_thread and self._reader_thread.is_alive():
            self._reader_thread.join(timeout=1.0)
        if self._serial:
            try:
                self._serial.close()
            except Exception:  # pragma: no cover
                pass
        self._serial = None
        self._reader_thread = None

    def _reader_loop(self) -> None:
        assert self._serial is not None
        while not self._stop_event.is_set():
            try:
                raw = self._serial.readline()
                if not raw:
                    continue
                line = raw.decode('utf-8', errors='ignore').strip()
                if line:
                    self._handle_line(line)
            except Exception as exc:  # pragma: no cover
                self.get_logger().error(f'Serial reader error: {exc}')
                break
        self._stop_reader()

    def _send_line(self, line: str) -> None:
        if not self._serial:
            self.get_logger().debug('Serial not ready, dropping command: %s', line)
            return
        try:
            payload = (line + '\n').encode('ascii')
            with self._serial_lock:
                self._serial.write(payload)
                self._serial.flush()
        except Exception as exc:  # pragma: no cover
            self.get_logger().error(f'Failed to write to serial: {exc}')
            self._stop_reader()

    def _send_heartbeat(self) -> None:
        if not self._serial or not self._heartbeat_command:
            return
        self._send_line(self._heartbeat_command)

    # ------------------------------------------------------------------ Parsing utilities
    def _handle_line(self, line: str) -> None:
        if line.startswith('<') and line.endswith('>'):
            status = self._parse_status(line)
            if status:
                self._last_status = status
                self._status_pub.publish(status)
        elif line.lower().startswith('error'):
            self.get_logger().error(f'grblHAL error: {line}')
            self._last_status.last_error = line
            self._status_pub.publish(self._last_status)
        elif 'ALARM' in line.upper():
            self.get_logger().error(f'grblHAL alarm: {line}')
            self._last_status.alarm = True
            self._last_status.last_error = line
            self._status_pub.publish(self._last_status)
        else:
            self.get_logger().debug('grblHAL: %s', line)

    def _parse_status(self, frame: str) -> Optional[GrblStatus]:
        body = frame.strip('<>').strip()
        if not body:
            return None
        parts = body.split('|')
        status = GrblStatus()
        status.stamp = self.get_clock().now().to_msg()
        status.connected = self._serial is not None
        status.state = parts[0]
        status.machine_state = '|'.join(parts[1:])
        status.frame_id = self._status_frame_id if hasattr(status, 'frame_id') else ''
        status.alarm = status.state.lower().startswith('alarm')

        for part in parts[1:]:
            if part.startswith('MPos:'):
                axis_values = self._parse_axis_values(part[5:])
                status.base_rotation = self._convert_incoming(axis_values.get(self._base_axis))
                status.arm_pivot = self._convert_incoming(axis_values.get(self._pivot_axis))
            elif part.startswith('FS:'):
                feed, spindle = self._parse_feed_spindle(part[3:])
                status.feed_rate = feed
                status.spindle_rpm = spindle

        return status

    @staticmethod
    def _parse_axis_values(payload: str) -> Dict[str, float]:
        values = {}
        try:
            floats = [float(v) for v in payload.split(',')]
        except ValueError:
            return values
        for axis, value in zip(AXES, floats):
            values[axis] = value
        return values

    @staticmethod
    def _parse_feed_spindle(payload: str) -> (float, float):
        try:
            feed_str, spindle_str = payload.split(',')
            return float(feed_str), float(spindle_str)
        except ValueError:
            return 0.0, 0.0

    def _convert_incoming(self, value: Optional[float]) -> float:
        if value is None:
            return 0.0
        if self._incoming_degrees:
            return math.radians(value)
        return value

    def _point_to_gcode(self, joint_names: List[str], point: JointTrajectoryPoint) -> Optional[str]:
        joint_map = {name: point.positions[idx] for idx, name in enumerate(joint_names)}

        segments: List[str] = ['G1']
        appended = False

        base_value = joint_map.get('base_rotation')
        if base_value is not None:
            segments.append(self._format_axis(self._base_axis, base_value))
            appended = True

        pivot_value = joint_map.get('arm_pivot')
        if pivot_value is not None:
            segments.append(self._format_axis(self._pivot_axis, pivot_value))
            appended = True

        if not appended:
            return None

        feed = self._default_feed
        if point.velocities:
            feed = max(point.velocities[0], 0.1) * 60.0
        segments.append(f'F{feed:.1f}')
        return ' '.join(segments)

    def _format_axis(self, axis: str, value: float) -> str:
        if self._output_degrees:
            value = math.degrees(value)
        return f'{axis}{value:.3f}'


def main() -> None:
    rclpy.init()
    node = GrblBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('grbl_bridge interrupted, shutting down')
    finally:
        node.destroy_node()
        rclpy.shutdown()
