/*
 * Copyright (c) 2016-2020 HYUNDAI TELECOM Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __CODEC_H__
#define __CODEC_H__

extern "C" {
#include "bcg729/encoder.h"
#include "bcg729/decoder.h"
}
#include <RefBase.h>

class Encoder : public android::RefBase {
public:
    Encoder();
    ~Encoder();
    void process(int16_t input[], int inputLength, uint8_t output[], int *outputLength);
private:
    bcg729EncoderChannelContextStruct *context_;
};

class Decoder : public android::RefBase {
public:
    Decoder();
    ~Decoder();
    void process(uint8_t input[], int inputLength, int16_t output[], int *outputLength);
private:
    bcg729DecoderChannelContextStruct *context_;
};


#endif //__CODEC_H__