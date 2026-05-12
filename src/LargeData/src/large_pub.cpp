#include <chrono>
#include <memory>
#include <vector>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"

using namespace std::chrono_literals;

class LargeDataPublisher : public rclcpp::Node
{
public:
    LargeDataPublisher()
        : Node("large_data_publisher")
    {
        publisher_ = this->create_publisher<std_msgs::msg::ByteMultiArray>(
            "large_data",
            10);

        timer_ = this->create_wall_timer(
            1000ms,
            std::bind(&LargeDataPublisher::publish_data, this));

        // 50 MB packet
        packet_size_ = 50 * 1024 * 1024;

        RCLCPP_INFO(this->get_logger(),
                    "Publisher started. Sending %zu MB packets",
                    packet_size_ / (1024 * 1024));
    }

private:
    void publish_data()
    {
        auto msg = std_msgs::msg::ByteMultiArray();

        auto start = std::chrono::high_resolution_clock::now();

        msg.data.resize(packet_size_);

        // Fill with random bytes
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_int_distribution<uint8_t> dist(0, 255);

        for (size_t i = 0; i < packet_size_; ++i)
        {
            msg.data[i] = dist(rng);
        }

        publisher_->publish(msg);

        auto end = std::chrono::high_resolution_clock::now();

        double elapsed =
            std::chrono::duration<double>(end - start).count();

        RCLCPP_INFO(
            this->get_logger(),
            "Sent %.2f MB in %.3f seconds",
            packet_size_ / (1024.0 * 1024.0),
            elapsed);
    }

    rclcpp::Publisher<std_msgs::msg::ByteMultiArray>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    size_t packet_size_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<LargeDataPublisher>());

    rclcpp::shutdown();

    return 0;
}