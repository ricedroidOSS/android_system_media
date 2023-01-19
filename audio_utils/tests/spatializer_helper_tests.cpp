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

#include <audio_utils/SpatializerHelper.h>
#include <gtest/gtest.h>

using namespace android;
using android::audio_utils::SpatializerHelper;

TEST(SpatializerHelper, binaural) {
  spatialization_mode_t spatializationMode = SPATIALIZATION_MODE_TRANSAURAL;

  // headset is binaural.
  ASSERT_EQ(OK, SpatializerHelper::spatializationModeForDeviceMask(
      AUDIO_DEVICE_OUT_WIRED_HEADSET, &spatializationMode));
  ASSERT_EQ(SPATIALIZATION_MODE_BINAURAL, spatializationMode);

  // headphone is binaural.
  spatializationMode = SPATIALIZATION_MODE_TRANSAURAL;
  ASSERT_EQ(OK, SpatializerHelper::spatializationModeForDeviceMask(
      AUDIO_DEVICE_OUT_WIRED_HEADPHONE, &spatializationMode));
  ASSERT_EQ(SPATIALIZATION_MODE_BINAURAL, spatializationMode);

  // binaural for 2 ORed masks (lowest bit is binaural, second bit not valid).
  spatializationMode = SPATIALIZATION_MODE_TRANSAURAL;
  ASSERT_EQ(OK, SpatializerHelper::spatializationModeForDeviceMask(
      (audio_devices_t)(AUDIO_DEVICE_OUT_WIRED_HEADPHONE | AUDIO_DEVICE_OUT_REMOTE_SUBMIX),
      &spatializationMode));
  ASSERT_EQ(SPATIALIZATION_MODE_BINAURAL, spatializationMode);
}

TEST(SpatializerHelper, transaural) {
  spatialization_mode_t spatializationMode = SPATIALIZATION_MODE_BINAURAL;

  // speaker is transaural.
  ASSERT_EQ(OK, SpatializerHelper::spatializationModeForDeviceMask(
      AUDIO_DEVICE_OUT_SPEAKER, &spatializationMode));
  ASSERT_EQ(SPATIALIZATION_MODE_TRANSAURAL, spatializationMode);

  // speaker and headphone (higher bit) both present, use the lowest bit (currently).
  ASSERT_EQ(OK, SpatializerHelper::spatializationModeForDeviceMask(
       (audio_devices_t)(AUDIO_DEVICE_OUT_SPEAKER | AUDIO_DEVICE_OUT_WIRED_HEADPHONE),
       &spatializationMode));
  ASSERT_EQ(SPATIALIZATION_MODE_TRANSAURAL, spatializationMode);
}

TEST(SpatializerHelper, invalid) {
  spatialization_mode_t spatializationMode = SPATIALIZATION_MODE_TRANSAURAL;

  // remote submix not supported (currently).
  ASSERT_EQ(BAD_VALUE, SpatializerHelper::spatializationModeForDeviceMask(
      AUDIO_DEVICE_OUT_REMOTE_SUBMIX, &spatializationMode));

  // nullptr destination invalid.
  ASSERT_EQ(BAD_VALUE, SpatializerHelper::spatializationModeForDeviceMask(
      AUDIO_DEVICE_OUT_WIRED_HEADSET, nullptr));
}
