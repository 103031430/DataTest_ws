#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/byte_multi_array.hpp"

class LargeDataSubscriber : public rclcpp::Node
{
public:
    LargeDataSubscriber()
        : Node("large_data_subscriber")
    {
        subscription_ =
            this->create_subscription<std_msgs::msg::ByteMultiArray>(
                "large_data",
                10,
                std::bind(
                    &LargeDataSubscriber::topic_callback,
                    this,
                    std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Subscriber started");
    }

private:
    void topic_callback(
        const std_msgs::msg::ByteMultiArray::SharedPtr msg)
    {
        double size_mb =
            msg->data.size() / (1024.0 * 1024.0);

        RCLCPP_INFO(
            this->get_logger(),
            "Received packet: %.2f MB",
            size_mb);
    }

    rclcpp::Subscription<std_msgs::msg::ByteMultiArray>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<LargeDataSubscriber>());

    rclcpp::shutdown();

    return 0;
}