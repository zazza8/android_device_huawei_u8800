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

# Hardware-specific features
PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
	frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
	frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
	frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
	frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

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

# Configs
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/audio_policy.conf:system/etc/audio_policy.conf \
	$(LOCAL_PATH)/configs/media_codecs.xml:system/etc/media_codecs.xml \
	$(LOCAL_PATH)/configs/media_profiles.xml:system/etc/media_profiles.xml

# Graphics
PRODUCT_PACKAGES += \
	libgenlock \
	libmemalloc \
	liboverlay \
	libqdutils \
	libtilerenderer

# OMX
PRODUCT_PACKAGES += \
	libstagefrighthw \
	libmm-omxcore \
	libOmxCore \
	libOmxVdec \
	libOmxVenc

# Audio
PRODUCT_PACKAGES += \
	libaudioutils

# HAL
PRODUCT_PACKAGES += \
	audio.primary.msm7x30 \
	audio_policy.msm7x30 \
	copybit.msm7x30 \
	gralloc.msm7x30 \
	hwcomposer.msm7x30 \
	lights.msm7x30 \
	power.qcom \
	sensors.u8800 \
	gps.u8800

# Filesystem management tools
PRODUCT_PACKAGES += \
	e2fsck

# Additional packages
PRODUCT_PACKAGES += \
	DeviceSettings

# Allow emulated sdcard on /data/media
PRODUCT_PROPERTY_OVERRIDES += \
	persist.fuse_sdcard=true

PRODUCT_PROPERTY_OVERRIDES += \
	ro.sf.lcd_density=240

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
	lpa.decode=true \
	use.non-omx.mp3.decoder=true \
	ro.hs_intmic.supported=1

# Graphics
PRODUCT_PROPERTY_OVERRIDES += \
	debug.composition.type=gpu \
	debug.egl.hw=1 \
	debug.sf.hw=1

PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=131072

# RIL
PRODUCT_PROPERTY_OVERRIDES += \
	rild.libpath=/system/lib/libril-qc-1.so \
	rild.libargs=-d/dev/smd0 \
	persist.rild.nitz_plmn="" \
	persist.rild.nitz_long_ons_0="" \
	persist.rild.nitz_long_ons_1="" \
	persist.rild.nitz_long_ons_2="" \
	persist.rild.nitz_long_ons_3="" \
	persist.rild.nitz_short_ons_0="" \
	persist.rild.nitz_short_ons_1="" \
	persist.rild.nitz_short_ons_2="" \
	persist.rild.nitz_short_ons_3="" \
	ril.subscription.types=NV,RUIM \
	ro.use_data_netmgrd=true \
	ro.telephony.ril_class=GBQualcommRIL
