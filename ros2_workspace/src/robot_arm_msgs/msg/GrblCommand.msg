# Timestamp assigned by ROS 2 side
builtin_interfaces/Time stamp

# Raw G-code or grblHAL real-time command to forward over UART
string gcode

# If true the bridge will wait for "ok"/error
bool expect_ok

# When true the command should be flushed immediately (vs buffered)
bool flush
