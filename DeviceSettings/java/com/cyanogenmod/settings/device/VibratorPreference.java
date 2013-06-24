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
import android.content.SharedPreferences;
import android.os.Vibrator;
import android.preference.SeekBarDialogPreference;
import android.util.AttributeSet;
import android.view.View;
import android.widget.SeekBar;

/**
 * Created by rudolf on 6/24/13.
 */
public class VibratorPreference extends SeekBarDialogPreference implements SeekBar.OnSeekBarChangeListener {

    SeekBar mSeekBar;

    public VibratorPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected void onBindDialogView(View view) {
        super.onBindDialogView(view);

        mSeekBar = getSeekBar(view);
        mSeekBar.setOnSeekBarChangeListener(this);

        int percent = VibratorSettings.strengthToPercent(getSharedPreferences().getInt(DeviceSettings.VIBRATOR_INTENSITY, VibratorSettings.DEF_VALUE));
        mSeekBar.setProgress(percent);
    }

    @Override
    protected void onDialogClosed(boolean positiveResult) {
        super.onDialogClosed(positiveResult);

        int voltage = VibratorSettings.percentToStrength(mSeekBar.getProgress());
        SharedPreferences prefs = getSharedPreferences();

        // Save off new value.
        if (positiveResult) {
            SharedPreferences.Editor editor = prefs.edit();
            editor.putInt(DeviceSettings.VIBRATOR_INTENSITY, voltage);
            editor.commit();
        // Restore old value if we did not modify.
        } else {
            VibratorSettings.writeVoltage(prefs.getInt(DeviceSettings.VIBRATOR_INTENSITY, VibratorSettings.DEF_VALUE));
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        VibratorSettings.writeVoltage(VibratorSettings.percentToStrength(seekBar.getProgress()));

        Vibrator vib = (Vibrator) getContext().getSystemService(Context.VIBRATOR_SERVICE);
        vib.vibrate(200);
    }
}
