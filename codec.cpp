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

#include "codec.h"

Encoder::Encoder() : context_(::initBcg729EncoderChannel(0)) {

}

Encoder::~Encoder() {
    ::closeBcg729EncoderChannel(context_);
}

void Encoder::process(int16_t input[], int inputLength, uint8_t output[], int *outputLength) {
    if (inputLength < 80) {
        *outputLength = 0;
        return;
    }
    uint8_t length;
    ::bcg729Encoder(context_, input, output, &length);
    *outputLength = length;
}

Decoder::Decoder() : context_(initBcg729DecoderChannel()) {

}

Decoder::~Decoder() {
    closeBcg729DecoderChannel(context_);
}

void Decoder::process(uint8_t input[], int inputLength, int16_t output[], int *outputLength) {
    if (inputLength < 10) {
        *outputLength = 0;
        return;
    }
    bcg729Decoder(context_, input, 10, 0, 0, 0, output);
    *outputLength = 80;
}
