#!/sbin/sh

# Disable frambuffer alignment
echo 0 > /sys/module/msm_fb/parameters/align_buffer

# Set brightnes for LCD and buttons
echo 127 > /sys/class/leds/lcd-backlight/brightness
echo 127 > /sys/class/leds/button-backlight/brightness

# Enable USB Mass Storage
echo 0 > /sys/class/android_usb/android0/enable
echo adb,mass_storage > /sys/class/android_usb/android0/functions
echo 1 > /sys/class/android_usb/android0/enable
