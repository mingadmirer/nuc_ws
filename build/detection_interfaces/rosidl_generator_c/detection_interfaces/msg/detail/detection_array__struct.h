// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from detection_interfaces:msg/DetectionArray.idl
// generated code does not contain a copyright notice

#ifndef DETECTION_INTERFACES__MSG__DETAIL__DETECTION_ARRAY__STRUCT_H_
#define DETECTION_INTERFACES__MSG__DETAIL__DETECTION_ARRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'detections'
#include "detection_interfaces/msg/detail/detection_result__struct.h"

/// Struct defined in msg/DetectionArray in the package detection_interfaces.
/**
  * 一帧内的所有检测结果
 */
typedef struct detection_interfaces__msg__DetectionArray
{
  /// 帧头 (时间戳 + frame_id)
  std_msgs__msg__Header header;
  /// 检测结果数组
  detection_interfaces__msg__DetectionResult__Sequence detections;
} detection_interfaces__msg__DetectionArray;

// Struct for a sequence of detection_interfaces__msg__DetectionArray.
typedef struct detection_interfaces__msg__DetectionArray__Sequence
{
  detection_interfaces__msg__DetectionArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} detection_interfaces__msg__DetectionArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DETECTION_INTERFACES__MSG__DETAIL__DETECTION_ARRAY__STRUCT_H_
