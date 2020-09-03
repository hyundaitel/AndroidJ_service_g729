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
#define LOG_TAG "G729Client"
//#define LOG_NDEBUG 0
#include <utils/Log.h>

#include "client.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

enum {
    COMMAND_INITAILIZE_DECODER = 0,
    COMMAND_INITAILIZE_ENCODER,
    COMMAND_DECODING,
    COMMAND_ENCODING,
    COMMAND_TERMINATE,
};

Client::Client(int socket) : Thread(false), socket_(socket), decoder_(), encoder_() {
    ALOGV("Create new client");
}

Client::~Client() {
    stop();
    ALOGV("Destroy client");
}

bool Client::valid() {
    return socket_ > 0;
}

void Client::stop() {
    // TODO: make it concurrently
    if (socket_ > 0) {
        close(socket_);
        socket_ = -1;
    }
    requestExit();
}

bool Client::threadLoop() {
    android::Parcel parcel;
    if (receive(parcel) < 0) {
        stop();
        return false;
    }
    android::Parcel reply;
    int command = parcel.readInt32();
    ALOGV("Command %d", command);
    switch (command) {
    case COMMAND_INITAILIZE_DECODER:
        decoder_ = new Decoder();
        reply.writeInt32(0);
        break;
    case COMMAND_INITAILIZE_ENCODER:
        encoder_ = new Encoder();
        reply.writeInt32(0);
        break;
    case COMMAND_DECODING:
        decode(parcel, reply);
        break;
    case COMMAND_ENCODING:
        encode(parcel, reply);
        break;
    case COMMAND_TERMINATE:
        decoder_.clear();
        encoder_.clear();
        stop();
        return false;
    default:
        stop();
        return false;
    }
    send(reply);
    return true;
}

int Client::receive(android::Parcel &parcel) {
    if (socket_ < 0) {
        return -1;
    }
    int length;
    int size = read(socket_, &length, sizeof(length));
    if (size < 0) {
        // Android 4 does not fully support exception in jni. pass the result throw return value
        return size;
    }
    uint8_t data[length];
    size = read(socket_, data, sizeof(uint8_t) * length);
    if (size < 0) {
        return size;
    }
    parcel.setData(data, length);
    return 0;
}

void Client::send(android::Parcel &parcel) {
    int size = parcel.dataSize();
    write(socket_, &size, sizeof(size));
    write(socket_, parcel.data(), parcel.dataSize());
}

void Client::decode(android::Parcel &parcel, android::Parcel &reply) {
    if (decoder_ == NULL) {
        return;
    }
    int length = parcel.readInt32();
    uint8_t input[length];
    parcel.read(input, sizeof(uint8_t) * length);
    int outputLength = 0;
    int16_t output[80];
    decoder_->process(input, length, output, &outputLength);
    reply.writeInt32((outputLength > 0) ? 0 : 1);
    reply.writeInt32(outputLength);
    reply.write(output, sizeof(int16_t) * outputLength);
}

void Client::encode(android::Parcel &parcel, android::Parcel &reply) {
    if (encoder_ == NULL) {
        reply.writeInt32(1);
        return;
    }
    int length = parcel.readInt32();
    int16_t input[length];
    parcel.read(input, sizeof(int16_t) * length);
    int outputLength = 0;
    uint8_t output[10];
    encoder_->process(input, length, output, &outputLength);
    reply.writeInt32((outputLength > 0) ? 0 : 1);
    reply.writeInt32(outputLength);
    reply.write(output, sizeof(uint8_t) * outputLength);
}
