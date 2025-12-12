# Timestamp when the status was sampled
builtin_interfaces/Time stamp

# True if the serial link to grblHAL is up and responding
bool connected

# High-level grblHAL state string (e.g. "Idle", "Run", "Alarm")
string state

# Optional machine state detail (planner, override info, etc.)
string machine_state

# Feed rate in mm/min reported by grblHAL (0 if unknown)
float32 feed_rate

# Spindle RPM or PWM-equivalent value
float32 spindle_rpm

# Latest base/pivot joint estimates derived from grbl feedback (radians)
float32 base_rotation
float32 arm_pivot

# True if grblHAL reported an alarm condition
bool alarm

# Free-form text describing the last error (empty if none)
string last_error
