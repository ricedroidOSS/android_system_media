/*
 * Copyright (C) 2023 The Android Open Source Project
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

#pragma once

#include <system/audio.h>
#include <system/audio_effects/effect_spatializer.h>
#include <utils/Errors.h> // status_t

namespace android::audio_utils {

/*
 * SpatializerHelper provides some useful audio methods
 * for implementing Spatialization.
 */
class SpatializerHelper {
public:

    /**
     * Converts a device mask to a spatialization mode.
     *
     * \param deviceMask         is a device mask, though typically a single device
     * \param spatializationMode is the chosen spatializationMode.
     *
     * \return OK on success or BAD_VALUE on failure.
     */

    static status_t spatializationModeForDeviceMask(
            audio_devices_t deviceMask, spatialization_mode_t *spatializationMode);

};

} // namespace android::audio_utils
