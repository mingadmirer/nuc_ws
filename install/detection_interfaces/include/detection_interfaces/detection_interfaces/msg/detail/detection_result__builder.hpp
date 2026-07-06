// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from detection_interfaces:msg/DetectionResult.idl
// generated code does not contain a copyright notice

#ifndef DETECTION_INTERFACES__MSG__DETAIL__DETECTION_RESULT__BUILDER_HPP_
#define DETECTION_INTERFACES__MSG__DETAIL__DETECTION_RESULT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "detection_interfaces/msg/detail/detection_result__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace detection_interfaces
{

namespace msg
{

namespace builder
{

class Init_DetectionResult_timestamp
{
public:
  explicit Init_DetectionResult_timestamp(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  ::detection_interfaces::msg::DetectionResult timestamp(::detection_interfaces::msg::DetectionResult::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_depth_mm
{
public:
  explicit Init_DetectionResult_depth_mm(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  Init_DetectionResult_timestamp depth_mm(::detection_interfaces::msg::DetectionResult::_depth_mm_type arg)
  {
    msg_.depth_mm = std::move(arg);
    return Init_DetectionResult_timestamp(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_confidence
{
public:
  explicit Init_DetectionResult_confidence(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  Init_DetectionResult_depth_mm confidence(::detection_interfaces::msg::DetectionResult::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return Init_DetectionResult_depth_mm(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_y2
{
public:
  explicit Init_DetectionResult_y2(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  Init_DetectionResult_confidence y2(::detection_interfaces::msg::DetectionResult::_y2_type arg)
  {
    msg_.y2 = std::move(arg);
    return Init_DetectionResult_confidence(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_x2
{
public:
  explicit Init_DetectionResult_x2(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  Init_DetectionResult_y2 x2(::detection_interfaces::msg::DetectionResult::_x2_type arg)
  {
    msg_.x2 = std::move(arg);
    return Init_DetectionResult_y2(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_y1
{
public:
  explicit Init_DetectionResult_y1(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  Init_DetectionResult_x2 y1(::detection_interfaces::msg::DetectionResult::_y1_type arg)
  {
    msg_.y1 = std::move(arg);
    return Init_DetectionResult_x2(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_x1
{
public:
  explicit Init_DetectionResult_x1(::detection_interfaces::msg::DetectionResult & msg)
  : msg_(msg)
  {}
  Init_DetectionResult_y1 x1(::detection_interfaces::msg::DetectionResult::_x1_type arg)
  {
    msg_.x1 = std::move(arg);
    return Init_DetectionResult_y1(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

class Init_DetectionResult_name
{
public:
  Init_DetectionResult_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_DetectionResult_x1 name(::detection_interfaces::msg::DetectionResult::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_DetectionResult_x1(msg_);
  }

private:
  ::detection_interfaces::msg::DetectionResult msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::detection_interfaces::msg::DetectionResult>()
{
  return detection_interfaces::msg::builder::Init_DetectionResult_name();
}

}  // namespace detection_interfaces

#endif  // DETECTION_INTERFACES__MSG__DETAIL__DETECTION_RESULT__BUILDER_HPP_
