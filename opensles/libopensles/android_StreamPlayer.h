/*
 * Copyright (C) 2010 The Android Open Source Project
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

// FIXME move to mediaplayer.h
enum media_player_stream_origin {
    MEDIA_PLAYER_STREAM_ORIGIN_INVALID           = 0,
    MEDIA_PLAYER_STREAM_ORIGIN_FILE              = 1 << 0,
    MEDIA_PLAYER_STREAM_ORIGIN_TRANSPORT_STREAM  = 1 << 1
};

typedef struct StreamPlayback_Parameters_struct {
    int streamType;
    int sessionId;
} StreamPlayback_Parameters;

/*
 * Called with a lock on the CAudioPlayer mObject
 */
extern void android_StreamPlayer_realize_lApObj(CAudioPlayer *ap);

extern void android_StreamPlayer_destroy(CAudioPlayer *ap);

namespace android {

class StreamPlayer
{
public:
    StreamPlayer(StreamPlayback_Parameters* params);
    virtual ~StreamPlayer();

    void setStream(SLuint32 streamOrigin);

protected:
    sp<MediaPlayer> mMediaPlayer;

private:

};

} // namespace android
