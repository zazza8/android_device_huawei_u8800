/*
 * Copyright (c) 2013, Rudolf Tammekivi.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

package com.cyanogenmod.settings.device;

public class VibratorSettings {

    private static final String VOLTAGE_LEVEL_FILE_PATH = "/sys/class/timed_output/vibrator/voltage_level";

    public static int DEF_VALUE;
    public static int MIN_VALUE;
    public static int MAX_VALUE;

    public static boolean isVoltageSupported() {
        return Utils.fileExists(VOLTAGE_LEVEL_FILE_PATH);
    }

    public static void writeVoltage(int voltage) {
        Utils.writeValue(VOLTAGE_LEVEL_FILE_PATH, Integer.toString(voltage));
    }

    /**
     * Convert vibrator strength to percent
     */
    public static int strengthToPercent(int strength) {
        double maxValue = MAX_VALUE;
        double minValue = MIN_VALUE;

        double percent = (strength - minValue) * (100 / (maxValue - minValue));

        if (percent > 100)
            percent = 100;
        else if (percent < 0)
            percent = 0;

        return (int) percent;
    }

    /**
     * Convert percent to vibrator strength
     */
    public static int percentToStrength(int percent) {
        int strength = Math.round((((MAX_VALUE - MIN_VALUE) * percent) / 100) + MIN_VALUE);

        if (strength > MAX_VALUE)
            strength = MAX_VALUE;
        else if (strength < MIN_VALUE)
            strength = MIN_VALUE;

        return strength;
    }
}
