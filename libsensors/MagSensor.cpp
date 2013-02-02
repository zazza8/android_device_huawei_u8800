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

#define MAG_DEBUG 0

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "MagSensor.h"

/*****************************************************************************/

MagSensor::MagSensor()
    : SensorBase(LSM303DLH_MAG_DEVICE, LSM303DLH_MAG_NAME),
      mInputReader(4), mEnabled(0), mHasPendingEvent(false)
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: Initializing...");

    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_M;
    mPendingEvent.type = SENSOR_TYPE_MAGNETIC_FIELD;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    open_device();

    int enabled;
    if (ioctl(dev_fd, LSM303DLH_MAG_IOCTL_GET_ENABLE, &enabled))
        ALOGE("MagSensor: Failed to get status.");

    mEnabled = enabled ? true : false;

    if (!mEnabled)
        close_device();
}

MagSensor::~MagSensor() {
    close_device();
}

int MagSensor::enable(int32_t handle, int enabled)
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: enable %d %d", handle, enabled);

    bool _enabled = enabled ? true : false;

    if (mEnabled == _enabled)
        return 0;

    if (_enabled)
        open_device();

    if (ioctl(dev_fd, LSM303DLH_MAG_IOCTL_SET_ENABLE, &enabled))
        ALOGE("MagSensor: Failed to set enable.");

    mEnabled = _enabled;

    if (!_enabled)
        close_device();

    return 0;
}

int MagSensor::setDelay(int32_t handle, int64_t ns)
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: setDelay %d %lld", handle, ns);

    if (!mEnabled)
        return 0;

    int ms = ns / 1000000;

    if (ioctl(dev_fd, LSM303DLH_MAG_IOCTL_SET_DELAY, &ms))
        ALOGE("MagSensor: Failed to set delay.");

    return 0;
}

bool MagSensor::hasPendingEvents() const
{
    return mHasPendingEvent;
}

int MagSensor::readEvents(sensors_event_t* data, int count)
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
            if (event->code == EVENT_TYPE_MAGV_X)
                mPendingEvent.magnetic.x = event->value * CONVERT_M;
            else if (event->code == EVENT_TYPE_MAGV_Y)
                mPendingEvent.magnetic.y = event->value * CONVERT_M;
            else if (event->code == EVENT_TYPE_MAGV_Z)
                mPendingEvent.magnetic.z = event->value * CONVERT_M;
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
            ALOGE("MagSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}
