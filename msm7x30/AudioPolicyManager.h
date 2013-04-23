/*
 * Copyright (C) 2009 The Android Open Source Project
 * Copyright (c) 2009, 2011-2012, The Linux Foundation. All rights reserved.
 * Copyright (c) 2012-2013, The CyanogenMod Project
 * Not a Contribution, Apache license notifications and license are retained
 * for attribution purposes only.
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

#include <stdint.h>
#include <sys/types.h>
#include <cutils/config_utils.h>
#include <cutils/misc.h>
#include <utils/Timers.h>
#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <hardware_legacy/AudioPolicyManagerBase.h>

namespace android_audio_legacy {

class AudioPolicyManager: public AudioPolicyManagerBase
{

public:
                AudioPolicyManager(AudioPolicyClientInterface *clientInterface)
                : AudioPolicyManagerBase(clientInterface) {
                }

        virtual ~AudioPolicyManager() {}

        // AudioPolicyInterface
        virtual status_t setDeviceConnectionState(audio_devices_t device,
                                                          AudioSystem::device_connection_state state,
                                                          const char *device_address);
        virtual AudioSystem::device_connection_state getDeviceConnectionState(audio_devices_t device,
                                                                              const char *device_address);
        virtual audio_io_handle_t getInput(int inputSource,
                                            uint32_t samplingRate,
                                            uint32_t format,
                                            uint32_t channels,
                                            AudioSystem::audio_in_acoustics acoustics);

        // indicates to the audio policy manager that the input starts being used.
        virtual status_t startInput(audio_io_handle_t input);

        // return appropriate device for streams handled by the specified strategy according to current
        // phone state, connected devices...
        // if fromCache is true, the device is returned from mDeviceForStrategy[], otherwise it is determined
        // by current state (device connected, phone state, force use, a2dp output...)
        // This allows to:
        //  1 speed up process when the state is stable (when starting or stopping an output)
        //  2 access to either current device selection (fromCache == true) or
        // "future" device selection (fromCache == false) when called from a context
        //  where conditions are changing (setDeviceConnectionState(), setPhoneState()...) AND
        //  before updateDeviceForStrategy() is called.
        virtual status_t startOutput(audio_io_handle_t output, AudioSystem::stream_type stream, int session = 0);
        virtual status_t stopOutput(audio_io_handle_t output, AudioSystem::stream_type stream, int session = 0);
        virtual void setForceUse(AudioSystem::force_use usage, AudioSystem::forced_config config);

protected:

 virtual audio_devices_t getDeviceForStrategy(routing_strategy strategy, bool fromCache = true);
        // change the route of the specified output
        uint32_t setOutputDevice(audio_io_handle_t output, audio_devices_t device, bool force = false, int delayMs = 0);
        // check that volume change is permitted, compute and send new volume to audio hardware
        status_t checkAndSetVolume(int stream, int index, audio_io_handle_t output, audio_devices_t device, int delayMs = 0, bool force = false);
        // select input device corresponding to requested audio source
        virtual audio_devices_t getDeviceForInputSource(int inputSource);

        // mute/unmute strategies using an incompatible device combination
        // if muting, wait for the audio in pcm buffer to be drained before proceeding
        // if unmuting, unmute only after the specified delay
        // Returns the number of ms waited
        uint32_t  checkDeviceMuteStrategies(AudioOutputDescriptor *outputDesc,
                                            audio_devices_t prevDevice,
                                            uint32_t delayMs);

private:
        // updates device caching and output for streams that can influence the
        //    routing of notifications
        void handleNotificationRoutingForStream(AudioSystem::stream_type stream);
};
}; //namespace
