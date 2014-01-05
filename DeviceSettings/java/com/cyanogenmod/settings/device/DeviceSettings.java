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

import android.app.ActionBar;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.content.res.Resources;
import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.view.MenuItem;

/**
 * A {@link PreferenceActivity} that presents a set of application settings.
 * Settings are presented as a single list.
 * <p/>
 * See <a href="http://developer.android.com/design/patterns/settings.html">
 * Android Design: Settings</a> for design guidelines and the <a
 * href="http://developer.android.com/guide/topics/ui/settings.html">Settings
 * API Guide</a> for more information on developing a Settings UI.
 */
public class DeviceSettings extends PreferenceActivity
        implements OnSharedPreferenceChangeListener {

    public static final String USB_MODE_STATE = "usb_mode_state";
    public static final String AUDIO_INTERNALMIC_STATE = "audio_internalmic_state";

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        final ActionBar actionBar = getActionBar();
        actionBar.setDisplayHomeAsUpEnabled(true);

        // Add 'general' preferences.
        addPreferencesFromResource(R.xml.pref_general);

        PreferenceCategory fakeHeader;

        // Add 'usb' preferences, and a corresponding header.
        fakeHeader = new PreferenceCategory(this);
        fakeHeader.setTitle(R.string.pref_header_usb);
        getPreferenceScreen().addPreference(fakeHeader);
        addPreferencesFromResource(R.xml.pref_usb);
        findPreference(USB_MODE_STATE).setEnabled(USBSettings.isModeSupported());

        // Add 'audio' preferences, and a corresponding header.
        fakeHeader = new PreferenceCategory(this);
        fakeHeader.setTitle(R.string.pref_header_headset);
        getPreferenceScreen().addPreference(fakeHeader);
        addPreferencesFromResource(R.xml.pref_headset);
        findPreference(AUDIO_INTERNALMIC_STATE).setEnabled(AudioSettings.isInternalmicForcedSupported());
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            onBackPressed();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPause() {
        super.onPause();
        getPreferenceScreen().getSharedPreferences()
                .unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        getPreferenceScreen().getSharedPreferences()
                .registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String s) {
        if (s.equals(USB_MODE_STATE))
            USBSettings.writeMode(this, sharedPreferences.getBoolean(USB_MODE_STATE, false));
        else if (s.equals(AUDIO_INTERNALMIC_STATE))
            AudioSettings.writeInternalmicForced(this, sharedPreferences.getBoolean(AUDIO_INTERNALMIC_STATE, false));
    }
}
