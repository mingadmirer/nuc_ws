#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "detection_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__detection_interfaces__msg__DetectionResult() -> *const std::ffi::c_void;
}

#[link(name = "detection_interfaces__rosidl_generator_c")]
extern "C" {
    fn detection_interfaces__msg__DetectionResult__init(msg: *mut DetectionResult) -> bool;
    fn detection_interfaces__msg__DetectionResult__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<DetectionResult>, size: usize) -> bool;
    fn detection_interfaces__msg__DetectionResult__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<DetectionResult>);
    fn detection_interfaces__msg__DetectionResult__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<DetectionResult>, out_seq: *mut rosidl_runtime_rs::Sequence<DetectionResult>) -> bool;
}

// Corresponds to detection_interfaces__msg__DetectionResult
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 单个检测结果

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct DetectionResult {
    /// 物品名称 (如 fist, lance, hand)
    pub name: rosidl_runtime_rs::String,

    /// 左上角 x (0~1, 归一化到图像宽度)
    pub x1: f32,

    /// 左上角 y (0~1, 归一化到图像高度)
    pub y1: f32,

    /// 右下角 x (0~1)
    pub x2: f32,

    /// 右下角 y (0~1)
    pub y2: f32,

    /// 置信度
    pub confidence: f32,

    /// 深度值 (毫米)
    pub depth_mm: f32,

    /// 检测时间戳
    pub timestamp: builtin_interfaces::msg::rmw::Time,

}



impl Default for DetectionResult {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !detection_interfaces__msg__DetectionResult__init(&mut msg as *mut _) {
        panic!("Call to detection_interfaces__msg__DetectionResult__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for DetectionResult {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { detection_interfaces__msg__DetectionResult__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { detection_interfaces__msg__DetectionResult__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { detection_interfaces__msg__DetectionResult__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for DetectionResult {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for DetectionResult where Self: Sized {
  const TYPE_NAME: &'static str = "detection_interfaces/msg/DetectionResult";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__detection_interfaces__msg__DetectionResult() }
  }
}


#[link(name = "detection_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__detection_interfaces__msg__DetectionArray() -> *const std::ffi::c_void;
}

#[link(name = "detection_interfaces__rosidl_generator_c")]
extern "C" {
    fn detection_interfaces__msg__DetectionArray__init(msg: *mut DetectionArray) -> bool;
    fn detection_interfaces__msg__DetectionArray__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<DetectionArray>, size: usize) -> bool;
    fn detection_interfaces__msg__DetectionArray__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<DetectionArray>);
    fn detection_interfaces__msg__DetectionArray__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<DetectionArray>, out_seq: *mut rosidl_runtime_rs::Sequence<DetectionArray>) -> bool;
}

// Corresponds to detection_interfaces__msg__DetectionArray
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 一帧内的所有检测结果

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct DetectionArray {
    /// 帧头 (时间戳 + frame_id)
    pub header: std_msgs::msg::rmw::Header,

    /// 检测结果数组
    pub detections: rosidl_runtime_rs::Sequence<super::super::msg::rmw::DetectionResult>,

}



impl Default for DetectionArray {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !detection_interfaces__msg__DetectionArray__init(&mut msg as *mut _) {
        panic!("Call to detection_interfaces__msg__DetectionArray__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for DetectionArray {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { detection_interfaces__msg__DetectionArray__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { detection_interfaces__msg__DetectionArray__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { detection_interfaces__msg__DetectionArray__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for DetectionArray {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for DetectionArray where Self: Sized {
  const TYPE_NAME: &'static str = "detection_interfaces/msg/DetectionArray";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__detection_interfaces__msg__DetectionArray() }
  }
}


