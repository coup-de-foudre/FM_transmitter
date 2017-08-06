/*
    fm_transmitter - use Raspberry Pi as FM transmitter

    Copyright (c) 2015, Marcin Kondej
    All rights reserved.

    See https://github.com/markondej/fm_transmitter

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be
    used to endorse or promote products derived from this software without specific
    prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
    SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
    TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
    WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ALSA_READER_H
#define ALSA_READER_H

#include <vector>
#include <fcntl.h>
#include <alsa/asoundlib.h>
#include <boost/lockfree/queue.hpp>

#include "abstract_reader.h"
#include "audio_format.h"
#include "error_reporter.h"

#define MAX_STREAM_SIZE 2097152
#define STREAM_SAMPLE_RATE 44100
#define STREAM_BITS_PER_SAMPLE 16
#define STREAM_CHANNELS 2
#define ALSA_FRAME_BUFFER_LENGTH 1024

using std::vector;

class AlsaReader : public AbstractReader {
 public:
        virtual ~AlsaReader();
        pthread_t thread;
        vector<float>* getFrames(unsigned frameCount, bool &forceStop);
        bool getFrames(vector<float>* &result);
        AudioFormat* getFormat();
        void stop(bool block);
        void reset();
        static AlsaReader* getInstance(string alsaDevice);

 private:
        AlsaReader(string alsaDevice);
        static void* read(void* params);
        static int setParams(snd_pcm_t* &);
        static bool doStop, isDataAccess, isReading;
        static string alsaDevice_;
        static boost::lockfree::queue<std::vector<float>*> queue;
};

#endif // ALSA_READER_H
