#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <serial/serial.h>
#include <thread>
#include <mutex>
#include <regex>
#include <chrono>

class GrblController : public rclcpp::Node
{
public:
    GrblController() : Node("grbl_controller")
    {
        // Declare parameters
        this->declare_parameter("serial_port", "/dev/ttyACM0");
        this->declare_parameter("baud_rate", 115200);
        this->declare_parameter("timeout_ms", 1000);
        this->declare_parameter("status_rate_hz", 10.0);
        this->declare_parameter("heartbeat_rate_hz", 2.0);
        
        // Get parameters
        std::string port = this->get_parameter("serial_port").as_string();
        int baud = this->get_parameter("baud_rate").as_int();
        int timeout = this->get_parameter("timeout_ms").as_int();
        double status_rate = this->get_parameter("status_rate_hz").as_double();
        double heartbeat_rate = this->get_parameter("heartbeat_rate_hz").as_double();
        
        // Initialize joint names
        joint_names_ = {"base_joint", "shoulder_joint", "elbow_joint", 
                       "linear1_joint", "linear2_joint", "end_effector_joint"};
        
        // Initialize joint positions
        current_joint_positions_.resize(joint_names_.size(), 0.0);
        
        // Initialize serial connection
        try {
            serial_port_ = std::make_unique<serial::Serial>(port, baud, 
                serial::Timeout::simpleTimeout(timeout));
            
            if (serial_port_->isOpen()) {
                RCLCPP_INFO(this->get_logger(), "Serial connection established on %s", port.c_str());
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to open serial port %s", port.c_str());
                return;
            }
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Serial connection error: %s", e.what());
            return;
        }
        
        // Initialize publishers
        joint_state_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
            "joint_states", 10);
        grbl_status_pub_ = this->create_publisher<std_msgs::msg::String>(
            "grbl_status", 10);
        grbl_alarm_pub_ = this->create_publisher<std_msgs::msg::Bool>(
            "grbl_alarm", 10);
        
        // Initialize subscribers  
        trajectory_sub_ = this->create_subscription<trajectory_msgs::msg::JointTrajectory>(
            "joint_trajectory", 10,
            std::bind(&GrblController::trajectory_callback, this, std::placeholders::_1));
        
        gcode_sub_ = this->create_subscription<std_msgs::msg::String>(
            "gcode_command", 10,
            std::bind(&GrblController::gcode_callback, this, std::placeholders::_1));
        
        // Initialize services
        home_service_ = this->create_service<std_srvs::srv::Trigger>(
            "home_robot", std::bind(&GrblController::home_service_callback, this,
            std::placeholders::_1, std::placeholders::_2));
        
        emergency_stop_service_ = this->create_service<std_srvs::srv::Trigger>(
            "emergency_stop", std::bind(&GrblController::emergency_stop_callback, this,
            std::placeholders::_1, std::placeholders::_2));
        
        reset_service_ = this->create_service<std_srvs::srv::Trigger>(
            "reset_grbl", std::bind(&GrblController::reset_grbl_callback, this,
            std::placeholders::_1, std::placeholders::_2));
        
        // Initialize timers
        status_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(static_cast<int>(1000.0 / status_rate)),
            std::bind(&GrblController::status_timer_callback, this));
        
        heartbeat_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(static_cast<int>(1000.0 / heartbeat_rate)),
            std::bind(&GrblController::heartbeat_timer_callback, this));
        
        // Start serial reader thread
        serial_reader_thread_ = std::thread(&GrblController::serial_reader_thread, this);
        
        // Initialize grblHAL
        initialize_grbl();
        
        RCLCPP_INFO(this->get_logger(), "grblHAL Controller initialized successfully");
    }
    
    ~GrblController()
    {
        // Stop threads and close serial port
        shutdown_requested_ = true;
        if (serial_reader_thread_.joinable()) {
            serial_reader_thread_.join();
        }
        if (serial_port_ && serial_port_->isOpen()) {
            serial_port_->close();
        }
    }

private:
    // ROS2 interfaces
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr grbl_status_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr grbl_alarm_pub_;
    
    rclcpp::Subscription<trajectory_msgs::msg::JointTrajectory>::SharedPtr trajectory_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gcode_sub_;
    
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr home_service_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr emergency_stop_service_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr reset_service_;
    
    rclcpp::TimerBase::SharedPtr status_timer_;
    rclcpp::TimerBase::SharedPtr heartbeat_timer_;
    
    // Serial communication
    std::unique_ptr<serial::Serial> serial_port_;
    std::thread serial_reader_thread_;
    std::mutex serial_mutex_;
    bool shutdown_requested_ = false;
    
    // Robot state
    std::vector<std::string> joint_names_;
    std::vector<double> current_joint_positions_;
    std::string current_grbl_state_ = "Unknown";
    bool grbl_alarm_state_ = false;
    rclcpp::Time last_status_time_;
    
    void initialize_grbl()
    {
        // Send initialization commands to grblHAL
        std::vector<std::string> init_commands = {
            "\r\n\r\n",  // Wake up grblHAL
            "$X",         // Unlock if alarmed
            "G21",        // Set units to millimeters
            "G90",        // Absolute positioning
            "G94"         // Feed rate in units per minute
        };
        
        for (const auto& cmd : init_commands) {
            send_gcode_command(cmd);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        RCLCPP_INFO(this->get_logger(), "grblHAL initialization commands sent");
    }
    
    void trajectory_callback(const trajectory_msgs::msg::JointTrajectory::SharedPtr msg)
    {
        if (msg->points.empty()) {
            RCLCPP_WARN(this->get_logger(), "Received empty trajectory");
            return;
        }
        
        RCLCPP_INFO(this->get_logger(), "Received trajectory with %zu points", msg->points.size());
        
        // Convert trajectory points to G-code commands
        for (const auto& point : msg->points) {
            if (point.positions.size() != joint_names_.size()) {
                RCLCPP_ERROR(this->get_logger(), "Joint position size mismatch: expected %zu, got %zu",
                           joint_names_.size(), point.positions.size());
                continue;
            }
            
            // Convert joint positions to G-code
            std::string gcode = joints_to_gcode(point.positions, 1000.0);  // Default feedrate
            send_gcode_command(gcode);
        }
    }
    
    void gcode_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received G-code: %s", msg->data.c_str());
        send_gcode_command(msg->data);
    }
    
    std::string joints_to_gcode(const std::vector<double>& positions, double feedrate)
    {
        if (positions.size() != 6) {
            RCLCPP_ERROR(this->get_logger(), "Invalid joint positions size: %zu", positions.size());
            return "";
        }
        
        // Format: G1 X<base> Y<shoulder> Z<elbow> A<linear1> B<linear2> C<end_eff> F<feedrate>
        std::ostringstream gcode;
        gcode << std::fixed << std::setprecision(3);
        gcode << "G1 X" << positions[0] 
              << " Y" << positions[1]
              << " Z" << positions[2] 
              << " A" << positions[3]
              << " B" << positions[4]
              << " C" << positions[5]
              << " F" << feedrate;
        
        return gcode.str();
    }
    
    void send_gcode_command(const std::string& command)
    {
        std::lock_guard<std::mutex> lock(serial_mutex_);
        try {
            if (serial_port_ && serial_port_->isOpen()) {
                serial_port_->write(command + "\n");
                serial_port_->flush();
                RCLCPP_DEBUG(this->get_logger(), "Sent G-code: %s", command.c_str());
            } else {
                RCLCPP_ERROR(this->get_logger(), "Serial port not available");
            }
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Error sending G-code: %s", e.what());
        }
    }
    
    void serial_reader_thread()
    {
        std::string buffer;
        
        while (!shutdown_requested_) {
            try {
                if (serial_port_ && serial_port_->isOpen() && serial_port_->available()) {
                    std::string data = serial_port_->read(serial_port_->available());
                    buffer += data;
                    
                    // Process complete lines
                    size_t pos;
                    while ((pos = buffer.find('\n')) != std::string::npos) {
                        std::string line = buffer.substr(0, pos);
                        buffer.erase(0, pos + 1);
                        
                        // Remove carriage return
                        if (!line.empty() && line.back() == '\r') {
                            line.pop_back();
                        }
                        
                        process_grbl_response(line);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } catch (const std::exception& e) {
                RCLCPP_ERROR(this->get_logger(), "Serial reader error: %s", e.what());
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
    
    void process_grbl_response(const std::string& response)
    {
        RCLCPP_DEBUG(this->get_logger(), "Received from grblHAL: %s", response.c_str());
        
        // Publish raw status
        auto status_msg = std_msgs::msg::String();
        status_msg.data = response;
        grbl_status_pub_->publish(status_msg);
        
        // Parse status reports: <Idle|MPos:0.000,0.000,0.000,0.000,0.000,0.000|FS:0,0>
        if (response.front() == '<' && response.back() == '>') {
            parse_status_report(response);
        }
        // Parse alarm states: ALARM:1
        else if (response.find("ALARM:") == 0) {
            grbl_alarm_state_ = true;
            auto alarm_msg = std_msgs::msg::Bool();
            alarm_msg.data = true;
            grbl_alarm_pub_->publish(alarm_msg);
            RCLCPP_WARN(this->get_logger(), "grblHAL ALARM: %s", response.c_str());
        }
        // Parse error messages: error:1
        else if (response.find("error:") == 0) {
            RCLCPP_ERROR(this->get_logger(), "grblHAL ERROR: %s", response.c_str());
        }
        // Parse ok responses
        else if (response == "ok") {
            RCLCPP_DEBUG(this->get_logger(), "Command acknowledged");
        }
    }
    
    void parse_status_report(const std::string& status)
    {
        last_status_time_ = this->now();
        
        // Extract state
        std::regex state_regex("^<([^|]+)");
        std::smatch state_match;
        if (std::regex_search(status, state_match, state_regex)) {
            current_grbl_state_ = state_match[1].str();
        }
        
        // Extract machine positions
        std::regex pos_regex("MPos:([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+)");
        std::smatch pos_match;
        if (std::regex_search(status, pos_match, pos_regex)) {
            for (size_t i = 0; i < 6 && i < current_joint_positions_.size(); ++i) {
                current_joint_positions_[i] = std::stod(pos_match[i + 1].str());
            }
            
            // Clear alarm state if we're getting position updates
            if (grbl_alarm_state_) {
                grbl_alarm_state_ = false;
                auto alarm_msg = std_msgs::msg::Bool();
                alarm_msg.data = false;
                grbl_alarm_pub_->publish(alarm_msg);
            }
        }
    }
    
    void status_timer_callback()
    {
        // Request status from grblHAL
        send_gcode_command("?");
        
        // Publish joint states
        publish_joint_states();
        
        // Check for communication timeout
        auto now = this->now();
        if (last_status_time_.nanoseconds() > 0) {
            auto timeout_duration = std::chrono::seconds(2);  // 2 second timeout
            if ((now - last_status_time_) > rclcpp::Duration(timeout_duration)) {
                RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                                    "grblHAL communication timeout");
            }
        }
    }
    
    void heartbeat_timer_callback()
    {
        // Send heartbeat to keep connection alive
        send_gcode_command("G4 P0.001");  // Dwell for 1ms - minimal command
    }
    
    void publish_joint_states()
    {
        auto joint_state_msg = sensor_msgs::msg::JointState();
        joint_state_msg.header.stamp = this->now();
        joint_state_msg.name = joint_names_;
        joint_state_msg.position = current_joint_positions_;
        
        // For now, velocity and effort are not available from grblHAL
        joint_state_msg.velocity.resize(joint_names_.size(), 0.0);
        joint_state_msg.effort.resize(joint_names_.size(), 0.0);
        
        joint_state_pub_->publish(joint_state_msg);
    }
    
    bool home_service_callback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                              std::shared_ptr<std_srvs::srv::Trigger::Response> response)
    {
        (void)request;  // Unused parameter
        
        RCLCPP_INFO(this->get_logger(), "Homing robot...");
        send_gcode_command("$H");
        
        response->success = true;
        response->message = "Homing cycle initiated";
        return true;
    }
    
    bool emergency_stop_callback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                                std::shared_ptr<std_srvs::srv::Trigger::Response> response)
    {
        (void)request;  // Unused parameter
        
        RCLCPP_WARN(this->get_logger(), "EMERGENCY STOP activated!");
        
        // Send emergency stop command
        send_gcode_command("M13");  // Custom emergency stop
        send_gcode_command("!");    // Real-time feed hold
        
        response->success = true;
        response->message = "Emergency stop activated";
        return true;
    }
    
    bool reset_grbl_callback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                           std::shared_ptr<std_srvs::srv::Trigger::Response> response)
    {
        (void)request;  // Unused parameter
        
        RCLCPP_INFO(this->get_logger(), "Resetting grblHAL...");
        
        // Send soft reset
        {
            std::lock_guard<std::mutex> lock(serial_mutex_);
            if (serial_port_ && serial_port_->isOpen()) {
                serial_port_->write("\x18");  // Ctrl+X soft reset
                serial_port_->flush();
            }
        }
        
        // Wait a moment and reinitialize
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        initialize_grbl();
        
        response->success = true;
        response->message = "grblHAL reset completed";
        return true;
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    
    try {
        auto node = std::make_shared<GrblController>();
        rclcpp::spin(node);
    } catch (const std::exception& e) {
        RCLCPP_ERROR(rclcpp::get_logger("grbl_controller"), "Exception: %s", e.what());
    }
    
    rclcpp::shutdown();
    return 0;
}