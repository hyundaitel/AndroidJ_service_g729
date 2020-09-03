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

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <threads.h>
#include <binder/Parcel.h> // Using parcel for serialization
#include "codec.h"
#include <RefBase.h>

class Client : public android::Thread {
public:
    Client(int socket);
    ~Client();

    bool valid();
    void stop();
private:
    int receive(android::Parcel &parcel);
    void send(android::Parcel &parcel);
    void decode(android::Parcel &parcel, android::Parcel &reply);
    void encode(android::Parcel &parcel, android::Parcel &reply);
    bool threadLoop();

    int socket_;
    android::sp<Decoder> decoder_;
    android::sp<Encoder> encoder_;
};

#endif //__CLIENT_H__