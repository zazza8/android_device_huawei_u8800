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

import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;

public class Utils {
    private static final String TAG = "DeviceSettings_Utils";

    public static boolean fileExists(String filename) {
        return new File(filename).exists();
    }

    public static void writeValue(String filename, String value) {
        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(new File(filename), false);
            fos.write(value.getBytes());
            fos.flush();
        } catch (Exception ex) {
            Log.w(TAG, "Exception: " + ex);
        } finally {
            if (fos != null) {
                try {
                    Log.w(TAG, "file " + filename + ": " + value);
                    fos.close();
                } catch (Exception ex) {
                    Log.w(TAG, "Exception: " + ex);
                }
            }
        }
    }
}
