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

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

public class USBSettings {

    private static final String USB_MODE_FILE_PATH = "/sys/devices/platform/msm_otg/mode";

    private static final Integer HOST_STATE = 0;
    private static final Integer PERIPHERAL_STATE = 1;

    private static final Integer USB_HOST_NOTIFICATION_ID = 0;
    public static final String USB_HOST_NOTIFICATION_INTENT =
            "com.cyanogenmod.settings.device.usb_host";

    public static boolean isModeSupported() {
        return Utils.fileExists(USB_MODE_FILE_PATH);
    }

    public static void writeMode(Context context, boolean host_on) {
        Utils.writeValue(USB_MODE_FILE_PATH, host_on ? HOST_STATE.toString() : PERIPHERAL_STATE.toString());

        NotificationManager notificationManager =
                (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

        if (host_on) {
            Intent intentActivity = new Intent(context, DeviceSettings.class);
            PendingIntent pActivityIntent = PendingIntent.getActivity(context, 0, intentActivity, 0);

            Intent intentTurnOff = new Intent(USB_HOST_NOTIFICATION_INTENT);
            PendingIntent pTurnoffIntent = PendingIntent.getBroadcast(context, 0, intentTurnOff, 0);

            Notification n = new Notification.Builder(context)
                    .setContentTitle(context.getString(R.string.not_usb_host_title))
                    .setContentText(context.getString(R.string.not_usb_host_text))
                    .setContentIntent(pTurnoffIntent)
                    .setSmallIcon(com.android.internal.R.drawable.stat_sys_data_usb)
                    .setAutoCancel(false)
                    .build();

            n.flags |= Notification.FLAG_ONGOING_EVENT;

            notificationManager.notify(USB_HOST_NOTIFICATION_ID, n);
        } else {
            notificationManager.cancel(USB_HOST_NOTIFICATION_ID);
        }
    }
}
