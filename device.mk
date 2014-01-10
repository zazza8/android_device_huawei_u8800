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
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml

# Init scripts
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/init.qcom.rc:root/init.qcom.rc \
	$(LOCAL_PATH)/init.qcom.usb.rc:root/init.qcom.usb.rc \
	$(LOCAL_PATH)/init.target.rc:root/init.target.rc \
	$(LOCAL_PATH)/ueventd.qcom.rc:root/ueventd.qcom.rc

# Recovery script
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/recovery/init.recovery.qcom.rc:root/init.recovery.qcom.rc \
	$(LOCAL_PATH)/recovery/postrecoveryboot.sh:recovery/root/sbin/postrecoveryboot.sh

# The fstab file
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/fstab.qcom:root/fstab.qcom

# Configs
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/audio_policy.conf:system/etc/audio_policy.conf \
	$(LOCAL_PATH)/configs/media_codecs.xml:system/etc/media_codecs.xml \
	$(LOCAL_PATH)/configs/media_profiles.xml:system/etc/media_profiles.xml \
	$(LOCAL_PATH)/configs/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
	$(LOCAL_PATH)/configs/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf

# Additional apps
PRODUCT_PACKAGES += \
	Torch

# Graphics
PRODUCT_PACKAGES += \
	libgenlock \
	libmemalloc \
	liboverlay \
	libqdutils \
	libtilerenderer \
	libI420colorconvert

# OMX
PRODUCT_PACKAGES += \
	libstagefrighthw \
	libmm-omxcore \
	libOmxCore \
	libOmxVdec \
	libOmxVenc

# Audio
PRODUCT_PACKAGES += \
	libaudio-resampler \
	libaudioutils \
	libdashplayer

# HAL
PRODUCT_PACKAGES += \
	audio.a2dp.default \
	audio.primary.msm7x30 \
	audio_policy.msm7x30 \
	camera.msm7x30 \
	copybit.msm7x30 \
	gralloc.msm7x30 \
	hwcomposer.msm7x30 \
	lights.msm7x30 \
	memtrack.msm7x30 \
	power.msm7x30 \
	sensors.u8800 \
	gps.u8800

# Filesystem management tools
PRODUCT_PACKAGES += \
	e2fsck

# Additional packages
PRODUCT_PACKAGES += \
	DeviceSettings

# Hardware MAC address tool
PRODUCT_PACKAGES += \
	hwmac

PRODUCT_PROPERTY_OVERRIDES += \
	ro.sf.lcd_density=240 \
	ro.config.low_ram=true

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
	lpa.decode=true \
	use.non-omx.mp3.decoder=true \
	ro.hs_intmic.supported=1

# Graphics
PRODUCT_PROPERTY_OVERRIDES += \
	debug.sf.hw=1 \
	debug.egl.hw=1 \
	debug.composition.type=gpu \
	persist.hwc.mdpcomp.enable=false \
	debug.mdpcomp.maxlayer=3 \
	debug.mdpcomp.logs=0 \
	debug.hwc.dynThreshold=1.9

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
	ro.telephony.ril_class=GBQualcommRIL

# Wi-Fi
PRODUCT_PROPERTY_OVERRIDES += \
	wifi.interface=wlan0

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
	ro.bt.bdaddr_path=/data/misc/bluedroid/bdaddr

# USB Interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mtp

# Recovery
PRODUCT_PROPERTY_OVERRIDES += \
	ro.cwm.forbid_format=/boot,/recovery
