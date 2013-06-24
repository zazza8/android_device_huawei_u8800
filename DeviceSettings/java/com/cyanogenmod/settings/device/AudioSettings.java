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

import android.content.Context;
import android.media.AudioManager;
import android.os.SystemProperties;
import android.widget.Toast;

public class AudioSettings {
    private static final String AUDIO_HS_INTMIC_SUPPORTED_PROP = "ro.hs_intmic.supported";
    private static final String AUDIO_HS_INTMIC_FORCED_PROP = "persist.service.audio.hs_intmic";

    private static final Integer DISABLED = 0;
    private static final Integer ENABLED = 1;

    public static boolean isInternalmicForcedSupported() {
        Integer value = SystemProperties.getInt(AUDIO_HS_INTMIC_SUPPORTED_PROP, DISABLED);

        if (value == DISABLED)
            return false;
        else if (value == ENABLED)
            return true;

        return false;
    }

    public static void writeInternalmicForced(Context context, boolean forced) {
        SystemProperties.set(AUDIO_HS_INTMIC_FORCED_PROP, forced ? ENABLED.toString() : DISABLED.toString());

        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);

        /* Need to reset the routing process. */
        if (audioManager.isWiredHeadsetOn()) {
            Toast.makeText(context, context.getResources().getString(R.string.toast_replug_headset),
                    Toast.LENGTH_SHORT).show();
        }
    }
}
