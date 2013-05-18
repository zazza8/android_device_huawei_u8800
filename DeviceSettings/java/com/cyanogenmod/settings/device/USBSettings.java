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

public class USBSettings {

    private static final String USB_MODE_FILE_PATH = "/sys/devices/platform/msm_otg/mode";

    private static final Integer HOST_STATE = 0;
    private static final Integer PERIPHERAL_STATE = 1;

    public static boolean isModeSupported() {
        return Utils.fileExists(USB_MODE_FILE_PATH);
    }

    public static void writeMode(boolean host_on) {
        Utils.writeValue(USB_MODE_FILE_PATH, host_on ? HOST_STATE.toString() : PERIPHERAL_STATE.toString());
    }
}
