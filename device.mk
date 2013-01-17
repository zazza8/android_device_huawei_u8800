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

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_eu_supl.mk)

# Include proprietary stuff
$(call inherit-product-if-exists, vendor/huawei/u8800/u8800-vendor.mk)

# Use standard dalvik heap sizes
$(call inherit-product, frameworks/native/build/phone-hdpi-512-dalvik-heap.mk)

# Include keyboards
$(call inherit-product-if-exists, device/huawei/u8800/keyboards/keyboards.mk)

# Include scripts
$(call inherit-product-if-exists, device/huawei/u8800/initscripts/initscripts.mk)

DEVICE_PACKAGE_OVERLAYS += device/huawei/u8800/overlay

# U8800 uses high-density artwork where available
PRODUCT_AAPT_CONFIG := normal hdpi
PRODUCT_AAPT_PREF_CONFIG := hdpi

# Init scripts
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/init.emmc.rc:root/init.emmc.rc \
	$(LOCAL_PATH)/init.qcom.rc:root/init.qcom.rc \
	$(LOCAL_PATH)/init.target.rc:root/init.target.rc \
	$(LOCAL_PATH)/ueventd.qcom.rc:root/ueventd.qcom.rc

# The fstab file
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/fstab.u8800:root/fstab.u8800 \
	$(LOCAL_PATH)/fstab.sdcard:root/fstab.sdcard \
	$(LOCAL_PATH)/vold.fstab:system/etc/vold.fstab

# Graphics
PRODUCT_PACKAGES += \
	libgenlock \
	libmemalloc \
	liboverlay \
	libqdutils \
	libtilerenderer

# HAL
PRODUCT_PACKAGES += \
	copybit.msm7x30 \
	gralloc.msm7x30 \
	hwcomposer.msm7x30 \
	power.qcom

# Filesystem management tools
PRODUCT_PACKAGES += \
	e2fsck

# Allow emulated sdcard on /data/media
PRODUCT_PROPERTY_OVERRIDES += \
	persist.fuse_sdcard=true

PRODUCT_PROPERTY_OVERRIDES += \
	ro.sf.lcd_density=240

# Graphics
PRODUCT_PROPERTY_OVERRIDES += \
	debug.composition.type=gpu \
	debug.egl.hw=1 \
	debug.sf.hw=1

PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=131072

# Workaround for MDP underrun issue with 4-layer composition
PRODUCT_PROPERTY_OVERRIDES += \
	debug.mdpcomp.maxlayer=3
