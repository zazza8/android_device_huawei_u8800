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

#ifndef ANDROID_APS_SENSOR_H
#define ANDROID_APS_SENSOR_H
#include <linux/input/aps-12d.h>

/*****************************************************************************/
#define APS_12D_NAME            "aps-12d"
#define APS_12D_DEVICE          "/dev/" APS_12D_NAME

/* All the light don't reach the sensor.
 * U8800 has darkened glass hiding the sensor. */
#define GLASS_MULTIPLIER        2.77f

/* Increase/decrease range if needed.
 * This should only be modified based on the light ADC values. */
#define RANGE_DEC_THRESHOLD     10
#define RANGE_INC_THRESHOLD     4000

#define PROX_SAMPLES		3

/* Values after converting the range. */
#define PROX_3CM_HIGH_THRESHOLD 40
#define PROX_3CM_LOW_THRESHOLD  25

/*****************************************************************************/

/*****************************************************************************/

#endif  // ANDROID_APS_SENSOR_H
