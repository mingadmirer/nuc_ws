// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from detection_interfaces:msg/DetectionResult.idl
// generated code does not contain a copyright notice
#include "detection_interfaces/msg/detail/detection_result__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `name`
#include "rosidl_runtime_c/string_functions.h"
// Member `timestamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
detection_interfaces__msg__DetectionResult__init(detection_interfaces__msg__DetectionResult * msg)
{
  if (!msg) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__init(&msg->name)) {
    detection_interfaces__msg__DetectionResult__fini(msg);
    return false;
  }
  // x1
  // y1
  // x2
  // y2
  // confidence
  // depth_mm
  // timestamp
  if (!builtin_interfaces__msg__Time__init(&msg->timestamp)) {
    detection_interfaces__msg__DetectionResult__fini(msg);
    return false;
  }
  return true;
}

void
detection_interfaces__msg__DetectionResult__fini(detection_interfaces__msg__DetectionResult * msg)
{
  if (!msg) {
    return;
  }
  // name
  rosidl_runtime_c__String__fini(&msg->name);
  // x1
  // y1
  // x2
  // y2
  // confidence
  // depth_mm
  // timestamp
  builtin_interfaces__msg__Time__fini(&msg->timestamp);
}

bool
detection_interfaces__msg__DetectionResult__are_equal(const detection_interfaces__msg__DetectionResult * lhs, const detection_interfaces__msg__DetectionResult * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->name), &(rhs->name)))
  {
    return false;
  }
  // x1
  if (lhs->x1 != rhs->x1) {
    return false;
  }
  // y1
  if (lhs->y1 != rhs->y1) {
    return false;
  }
  // x2
  if (lhs->x2 != rhs->x2) {
    return false;
  }
  // y2
  if (lhs->y2 != rhs->y2) {
    return false;
  }
  // confidence
  if (lhs->confidence != rhs->confidence) {
    return false;
  }
  // depth_mm
  if (lhs->depth_mm != rhs->depth_mm) {
    return false;
  }
  // timestamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->timestamp), &(rhs->timestamp)))
  {
    return false;
  }
  return true;
}

bool
detection_interfaces__msg__DetectionResult__copy(
  const detection_interfaces__msg__DetectionResult * input,
  detection_interfaces__msg__DetectionResult * output)
{
  if (!input || !output) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__copy(
      &(input->name), &(output->name)))
  {
    return false;
  }
  // x1
  output->x1 = input->x1;
  // y1
  output->y1 = input->y1;
  // x2
  output->x2 = input->x2;
  // y2
  output->y2 = input->y2;
  // confidence
  output->confidence = input->confidence;
  // depth_mm
  output->depth_mm = input->depth_mm;
  // timestamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->timestamp), &(output->timestamp)))
  {
    return false;
  }
  return true;
}

detection_interfaces__msg__DetectionResult *
detection_interfaces__msg__DetectionResult__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  detection_interfaces__msg__DetectionResult * msg = (detection_interfaces__msg__DetectionResult *)allocator.allocate(sizeof(detection_interfaces__msg__DetectionResult), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(detection_interfaces__msg__DetectionResult));
  bool success = detection_interfaces__msg__DetectionResult__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
detection_interfaces__msg__DetectionResult__destroy(detection_interfaces__msg__DetectionResult * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    detection_interfaces__msg__DetectionResult__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
detection_interfaces__msg__DetectionResult__Sequence__init(detection_interfaces__msg__DetectionResult__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  detection_interfaces__msg__DetectionResult * data = NULL;

  if (size) {
    data = (detection_interfaces__msg__DetectionResult *)allocator.zero_allocate(size, sizeof(detection_interfaces__msg__DetectionResult), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = detection_interfaces__msg__DetectionResult__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        detection_interfaces__msg__DetectionResult__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
detection_interfaces__msg__DetectionResult__Sequence__fini(detection_interfaces__msg__DetectionResult__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      detection_interfaces__msg__DetectionResult__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

detection_interfaces__msg__DetectionResult__Sequence *
detection_interfaces__msg__DetectionResult__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  detection_interfaces__msg__DetectionResult__Sequence * array = (detection_interfaces__msg__DetectionResult__Sequence *)allocator.allocate(sizeof(detection_interfaces__msg__DetectionResult__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = detection_interfaces__msg__DetectionResult__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
detection_interfaces__msg__DetectionResult__Sequence__destroy(detection_interfaces__msg__DetectionResult__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    detection_interfaces__msg__DetectionResult__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
detection_interfaces__msg__DetectionResult__Sequence__are_equal(const detection_interfaces__msg__DetectionResult__Sequence * lhs, const detection_interfaces__msg__DetectionResult__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!detection_interfaces__msg__DetectionResult__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
detection_interfaces__msg__DetectionResult__Sequence__copy(
  const detection_interfaces__msg__DetectionResult__Sequence * input,
  detection_interfaces__msg__DetectionResult__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(detection_interfaces__msg__DetectionResult);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    detection_interfaces__msg__DetectionResult * data =
      (detection_interfaces__msg__DetectionResult *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!detection_interfaces__msg__DetectionResult__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          detection_interfaces__msg__DetectionResult__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!detection_interfaces__msg__DetectionResult__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
