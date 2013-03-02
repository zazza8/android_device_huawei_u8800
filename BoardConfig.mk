#
# Copyright (C) 2012 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# WARNING: This line must come *before* including the proprietary
# variant, so that it gets overwritten by the parent (which goes
# against the traditional rules of inheritance).
# The proprietary variant sets USE_CAMERA_STUB := false, this way
# we use the camera stub when the vendor tree isn't present, and
# the true camera library when the vendor tree is available.
USE_CAMERA_STUB := true

# Inherit from the proprietary version if exists
-include vendor/huawei/u8800/BoardConfigVendor.mk

TARGET_SPECIFIC_HEADER_PATH := device/huawei/u8800/include

TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TAGET_ARCH_VARIANT_CPU := cortex-a8
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_BOARD_PLATFORM := msm7x30
TARGET_BOARD_PLATFORM_GPU := qcom-adreno200
TARGET_BOOTLOADER_BOARD_NAME := u8800

# Qualcomm Hardware
BOARD_USES_QCOM_HARDWARE := true
COMMON_GLOBAL_CFLAGS += -DQCOM_HARDWARE
TARGET_QCOM_AUDIO_VARIANT := caf
TARGET_QCOM_DISPLAY_VARIANT := caf

# Graphics
TARGET_HARDWARE_3D := false
TARGET_HAVE_HDMI_OUT := false
USE_OPENGL_RENDERER := true
TARGET_USES_ION := true
TARGET_USES_C2D_COMPOSITION := true
BOARD_EGL_CFG := device/huawei/u8800/egl.cfg

TARGET_PROVIDES_LIBLIGHT := true

# We don't build bootloader nor radio.
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# Radio class.
BOARD_RIL_CLASS := ../../../device/huawei/u8800/ril/

# Kernel
TARGET_NO_KERNEL := false
TARGET_KERNEL_SOURCE := kernel/huawei/u8800
TARGET_KERNEL_CONFIG := u8800_defconfig
BOARD_KERNEL_CMDLINE := console=ttyDCC0 androidboot.hardware=qcom
BOARD_KERNEL_BASE := 0x00200000
BOARD_KERNEL_PAGESIZE := 4096

# Filesystem
BOARD_WANTS_EMMC_BOOT := true

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 402653184 #384MB
BOARD_FLASH_BLOCK_SIZE := 262144 # (BOARD_KERNEL_PAGESIZE * 64)

# Recovery
BOARD_HAS_NO_SELECT_BUTTON := true

# Webkit
ENABLE_WEBGL := true

# Flags
TARGET_GLOBAL_CFLAGS += -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp
