/*
 * Copyright (C) 2012 The Android Open-Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define PROX_DEBUG 0

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "ProximitySensor.h"

/*****************************************************************************/

ProximitySensor::ProximitySensor()
    : SensorBase(APS_12D_DEVICE, APS_12D_NAME),
      mInputReader(4), mEnabled(0), mHasPendingEvent(false),
      mSampleCounter(0)
{
    ALOGD_IF(PROX_DEBUG, "ProximitySensor: Initializing...");

    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_P;
    mPendingEvent.type = SENSOR_TYPE_PROXIMITY;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    open_device();

    if (ioctl(dev_fd, APS_IOCTL_GET_PROXIMITY_ENABLE, &mEnabled))
        ALOGE("ProximitySensor: Failed to get status.");

    if (ioctl(dev_fd, APS_IOCTL_GET_SETTINGS, &settings))
        ALOGE("ProximitySensor: Failed to get settings.");

    if (!mEnabled)
        close_device();
}

ProximitySensor::~ProximitySensor() {
    close_device();
}

int ProximitySensor::enable(int32_t handle, int enabled)
{
    ALOGD_IF(PROX_DEBUG, "ProximitySensor: enable %d %d", handle, enabled);

    bool _enabled = enabled ? true : false;

    if (mEnabled == _enabled)
        return 0;

    if (_enabled)
        open_device();

    if (ioctl(dev_fd, APS_IOCTL_SET_PROXIMITY_ENABLE, &_enabled))
        ALOGE("ProximitySensor: Failed to set proximity enable.");

    mEnabled = _enabled;

    return 0;
}

int ProximitySensor::setDelay(int32_t handle, int64_t ns)
{
    ALOGD_IF(PROX_DEBUG, "ProximitySensor: setDelay %d %lld", handle, ns);

    if (!mEnabled)
        return 0;

    if (ioctl(dev_fd, APS_IOCTL_SET_PROXIMITY_DELAY, &ns))
        ALOGE("ProximitySensor: Failed to set proximity delay.");

    return 0;
}

bool ProximitySensor::hasPendingEvents() const
{
    return mHasPendingEvent;
}

int ProximitySensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent)
    {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event))
    {
        int type = event->type;
        if (type == EV_ABS)
        {
            if (event->code == EVENT_TYPE_PROXIMITY)
                mPendingEvent.distance = getProxValue(event->value);
        }
        else if (type == EV_SYN)
        {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled)
            {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("ProximitySensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}

float ProximitySensor::getProxValue(uint16_t adc_count)
{
    float correct_adc = getCorrectADC(adc_count) * GLASS_MULTIPLIER;
    float distance = 1.0f;

    /* Proximity sensing algorithm described in Intersil datasheet.
     * This algorithm has low and high thresholds, which are used to figure out
     * whether the object is near or far. */
    if (correct_adc >= PROX_3CM_HIGH_THRESHOLD)
        distance = 0.0f;
    else if (correct_adc <= PROX_3CM_LOW_THRESHOLD)
        distance = 1.0f;

    /* Compare data with previously saved. */
    int i;
    for (i = 0; i < PROX_SAMPLES; i++) {
        if (mSamples[i] != distance)
            break;
    }

    float return_distance;

    /* Only return distance as close when PROX_SAMPLES data matches. */
    if (i == PROX_SAMPLES && distance == 0.0f)
        return_distance = distance;
    else
        return_distance = 1.0f;

    /* Save data for future processing. */
    mSamples[mSampleCounter] = distance;
    mSampleCounter++;

    if (mSampleCounter == PROX_SAMPLES)
        mSampleCounter = 0;

    return return_distance;
}

float ProximitySensor::getCorrectADC(uint16_t adc_count)
{
    float countValue;

    /* Read the settings. */
    if (ioctl(dev_fd, APS_IOCTL_GET_SETTINGS, &settings))
    {
        ALOGE("ProximitySensor: Failed to get settings.");
        return -EINVAL;
    }

    switch(settings.range)
    {
        case APS_12D_RANGE_0P24_TO_1000:
            countValue = 0.24f;
            break;
        case APS_12D_RANGE_0P96_TO_4000:
            countValue = 0.96f;
            break;
        case APS_12D_RANGE_3P84_TO_16000:
            countValue = 3.84f;
            break;
        case APS_12D_RANGE_15P36_TO_64000:
            countValue = 15.36f;
            break;
        default:
            countValue = 0.24f;
            break;
    }

    return adc_count * countValue;
}
