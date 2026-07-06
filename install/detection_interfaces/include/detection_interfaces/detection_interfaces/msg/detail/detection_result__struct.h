// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from detection_interfaces:msg/DetectionResult.idl
// generated code does not contain a copyright notice

#ifndef DETECTION_INTERFACES__MSG__DETAIL__DETECTION_RESULT__STRUCT_H_
#define DETECTION_INTERFACES__MSG__DETAIL__DETECTION_RESULT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'name'
#include "rosidl_runtime_c/string.h"
// Member 'timestamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in msg/DetectionResult in the package detection_interfaces.
/**
  * 单个检测结果
 */
typedef struct detection_interfaces__msg__DetectionResult
{
  /// 物品名称 (如 fist, lance, hand)
  rosidl_runtime_c__String name;
  /// 左上角 x (0~1, 归一化到图像宽度)
  float x1;
  /// 左上角 y (0~1, 归一化到图像高度)
  float y1;
  /// 右下角 x (0~1)
  float x2;
  /// 右下角 y (0~1)
  float y2;
  /// 置信度
  float confidence;
  /// 深度值 (毫米)
  float depth_mm;
  /// 检测时间戳
  builtin_interfaces__msg__Time timestamp;
} detection_interfaces__msg__DetectionResult;

// Struct for a sequence of detection_interfaces__msg__DetectionResult.
typedef struct detection_interfaces__msg__DetectionResult__Sequence
{
  detection_interfaces__msg__DetectionResult * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} detection_interfaces__msg__DetectionResult__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DETECTION_INTERFACES__MSG__DETAIL__DETECTION_RESULT__STRUCT_H_
