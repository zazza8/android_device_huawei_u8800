#!/sbin/sh

# Disable frambuffer alignment
echo 0 > /sys/module/msm_fb/parameters/align_buffer

# Set brightnes for LCD and buttons
echo 127 > /sys/class/leds/lcd-backlight/brightness
echo 127 > /sys/class/leds/button-backlight/brightness
