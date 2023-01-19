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

#define LOG_TAG "SpatializerHelper"
// #define LOG_NDEBUG 0

#include <audio_utils/SpatializerHelper.h>

#include <unordered_map>
#include <log/log.h>

namespace android::audio_utils {

/* static */
status_t SpatializerHelper::spatializationModeForDeviceMask(
        audio_devices_t deviceMask, spatialization_mode_t *spatializationMode)
{
    // This table must be kept in sync with
    // class AudioDeviceInfo in
    // AudioDeviceInfo.java
    enum class JavaAudioDeviceInfo {
        TYPE_UNKNOWN          = 0,
        TYPE_BUILTIN_EARPIECE = 1,
        TYPE_BUILTIN_SPEAKER  = 2,
        TYPE_WIRED_HEADSET    = 3,
        TYPE_WIRED_HEADPHONES = 4,
        TYPE_LINE_ANALOG      = 5,
        TYPE_LINE_DIGITAL     = 6,
        TYPE_BLUETOOTH_SCO    = 7,
        TYPE_BLUETOOTH_A2DP   = 8,
        TYPE_HDMI             = 9,
        TYPE_HDMI_ARC         = 10,
        TYPE_USB_DEVICE       = 11,
        TYPE_USB_ACCESSORY    = 12,
        TYPE_DOCK             = 13,
        TYPE_FM               = 14,
        TYPE_BUILTIN_MIC      = 15,
        TYPE_FM_TUNER         = 16,
        TYPE_TV_TUNER         = 17,
        TYPE_TELEPHONY        = 18,
        TYPE_AUX_LINE         = 19,
        TYPE_IP               = 20,
        TYPE_BUS              = 21,
        TYPE_USB_HEADSET      = 22,
        TYPE_HEARING_AID      = 23,
        TYPE_BUILTIN_SPEAKER_SAFE = 24,
        TYPE_REMOTE_SUBMIX    = 25,
        TYPE_BLE_HEADSET      = 26,
        TYPE_BLE_SPEAKER      = 27,
        TYPE_ECHO_REFERENCE   = 28,
        TYPE_HDMI_EARC        = 29,
        TYPE_BLE_BROADCAST    = 30,
        TYPE_DOCK_ANALOG      = 31,
    };

    // This table must be kept in sync with
    // AudioDeviceInfo.INT_TO_EXT_DEVICE_MAPPING in
    // AudioDeviceInfo.java
    static std::unordered_map<audio_devices_t, JavaAudioDeviceInfo> deviceToJavaDevice {
        {AUDIO_DEVICE_OUT_EARPIECE, JavaAudioDeviceInfo::TYPE_BUILTIN_EARPIECE},
        {AUDIO_DEVICE_OUT_SPEAKER, JavaAudioDeviceInfo::TYPE_BUILTIN_SPEAKER},
        {AUDIO_DEVICE_OUT_WIRED_HEADSET, JavaAudioDeviceInfo::TYPE_WIRED_HEADSET},
        {AUDIO_DEVICE_OUT_WIRED_HEADPHONE, JavaAudioDeviceInfo::TYPE_WIRED_HEADPHONES},
        {AUDIO_DEVICE_OUT_BLUETOOTH_SCO, JavaAudioDeviceInfo::TYPE_BLUETOOTH_SCO},
        {AUDIO_DEVICE_OUT_BLUETOOTH_SCO_HEADSET, JavaAudioDeviceInfo::TYPE_BLUETOOTH_SCO},
        {AUDIO_DEVICE_OUT_BLUETOOTH_SCO_CARKIT, JavaAudioDeviceInfo::TYPE_BLUETOOTH_SCO},
        {AUDIO_DEVICE_OUT_BLUETOOTH_A2DP, JavaAudioDeviceInfo::TYPE_BLUETOOTH_A2DP},
        {AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES, JavaAudioDeviceInfo::TYPE_BLUETOOTH_A2DP},
        {AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER, JavaAudioDeviceInfo::TYPE_BLUETOOTH_A2DP},
        {AUDIO_DEVICE_OUT_HDMI, JavaAudioDeviceInfo::TYPE_HDMI},
        {AUDIO_DEVICE_OUT_ANLG_DOCK_HEADSET, JavaAudioDeviceInfo::TYPE_DOCK_ANALOG},
        {AUDIO_DEVICE_OUT_DGTL_DOCK_HEADSET, JavaAudioDeviceInfo::TYPE_DOCK},
        {AUDIO_DEVICE_OUT_USB_ACCESSORY, JavaAudioDeviceInfo::TYPE_USB_ACCESSORY},
        {AUDIO_DEVICE_OUT_USB_DEVICE, JavaAudioDeviceInfo::TYPE_USB_DEVICE},
        {AUDIO_DEVICE_OUT_USB_HEADSET, JavaAudioDeviceInfo::TYPE_USB_HEADSET},
        {AUDIO_DEVICE_OUT_TELEPHONY_TX, JavaAudioDeviceInfo::TYPE_TELEPHONY},
        {AUDIO_DEVICE_OUT_LINE, JavaAudioDeviceInfo::TYPE_LINE_ANALOG},
        {AUDIO_DEVICE_OUT_HDMI_ARC, JavaAudioDeviceInfo::TYPE_HDMI_ARC},
        {AUDIO_DEVICE_OUT_HDMI_EARC, JavaAudioDeviceInfo::TYPE_HDMI_EARC},
        {AUDIO_DEVICE_OUT_SPDIF, JavaAudioDeviceInfo::TYPE_LINE_DIGITAL},
        {AUDIO_DEVICE_OUT_FM, JavaAudioDeviceInfo::TYPE_FM},
        {AUDIO_DEVICE_OUT_AUX_LINE, JavaAudioDeviceInfo::TYPE_AUX_LINE},
        {AUDIO_DEVICE_OUT_IP, JavaAudioDeviceInfo::TYPE_IP},
        {AUDIO_DEVICE_OUT_BUS, JavaAudioDeviceInfo::TYPE_BUS},
        {AUDIO_DEVICE_OUT_HEARING_AID, JavaAudioDeviceInfo::TYPE_HEARING_AID},
        {AUDIO_DEVICE_OUT_SPEAKER_SAFE, JavaAudioDeviceInfo::TYPE_BUILTIN_SPEAKER_SAFE},
        {AUDIO_DEVICE_OUT_REMOTE_SUBMIX, JavaAudioDeviceInfo::TYPE_REMOTE_SUBMIX},
        {AUDIO_DEVICE_OUT_BLE_HEADSET, JavaAudioDeviceInfo::TYPE_BLE_HEADSET},
        {AUDIO_DEVICE_OUT_BLE_SPEAKER, JavaAudioDeviceInfo::TYPE_BLE_SPEAKER},
        {AUDIO_DEVICE_OUT_BLE_BROADCAST, JavaAudioDeviceInfo::TYPE_BLE_BROADCAST},
    };

    // This table must be kept in sync with
    // SpatializerHelper.SPAT_MODE_FOR_DEVICE_TYPE
    // in SpatializerHelper.java
    static std::unordered_map<JavaAudioDeviceInfo, spatialization_mode_t> javaDeviceToMode {
        {JavaAudioDeviceInfo::TYPE_BUILTIN_SPEAKER, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_WIRED_HEADSET, SPATIALIZATION_MODE_BINAURAL},
        {JavaAudioDeviceInfo::TYPE_WIRED_HEADPHONES, SPATIALIZATION_MODE_BINAURAL},
        // assumption for A2DP: mostly headsets
        {JavaAudioDeviceInfo::TYPE_BLUETOOTH_A2DP, SPATIALIZATION_MODE_BINAURAL},
        {JavaAudioDeviceInfo::TYPE_DOCK, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_USB_ACCESSORY, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_USB_DEVICE, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_USB_HEADSET, SPATIALIZATION_MODE_BINAURAL},
        {JavaAudioDeviceInfo::TYPE_LINE_ANALOG, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_LINE_DIGITAL, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_AUX_LINE, SPATIALIZATION_MODE_TRANSAURAL},
        {JavaAudioDeviceInfo::TYPE_HEARING_AID, SPATIALIZATION_MODE_BINAURAL},
        {JavaAudioDeviceInfo::TYPE_BLE_HEADSET, SPATIALIZATION_MODE_BINAURAL},
        {JavaAudioDeviceInfo::TYPE_BLE_SPEAKER, SPATIALIZATION_MODE_TRANSAURAL},
        // assumption that BLE broadcast would be mostly consumed on headsets
        {JavaAudioDeviceInfo::TYPE_BLE_BROADCAST, SPATIALIZATION_MODE_BINAURAL},
    };

    if (spatializationMode == nullptr) return BAD_VALUE;

    // The logic here uses a two hash table lookup to emulate the Java Spatializer
    // assignment of spatialization mode to a device.
    //
    // audio_devices_t -> JavaAudioDeviceInfo::TYPE -> spatialization_mode_t
    //
    // It is possible to create a new hash table that concatenates
    // both tables together, but we consider this a unnecessary optimization at this time.

    auto javaDeviceIt = deviceToJavaDevice.find(deviceMask);
    if (javaDeviceIt == deviceToJavaDevice.end()) {
        // Note: deviceMask is a bitmask, so likely two devices have been ORed.
        // Current strategy is get lowest bit and retry, but that may not be
        // the best strategy (e.g. hamming distance search).
        ALOGW("%s: deviceMask %#x has more than one device, trying lowest bit",
                __func__, deviceMask);
        javaDeviceIt = deviceToJavaDevice.find((audio_devices_t)(deviceMask & -deviceMask));
    }
    if (javaDeviceIt == deviceToJavaDevice.end()) {
        ALOGW("%s: deviceMask %#x is invalid", __func__, deviceMask);
        return BAD_VALUE;
    }
    if (const auto modeIt = javaDeviceToMode.find(javaDeviceIt->second);
        modeIt !=  javaDeviceToMode.end()) {
        *spatializationMode = modeIt->second;
        return OK;
    }
    ALOGW("%s: no allowed spatialization mode for deviceMask %#x", __func__, deviceMask);
    return BAD_VALUE;
}

} // namespace android::audio_utils
