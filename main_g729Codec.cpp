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

//#define LOG_NDEBUG 0
#define LOG_TAG "g729Codec"
#include <cutils/log.h>
#include <cutils/sockets.h>
#include <sys/socket.h>
#include <stdio.h>
#include <Vector.h>
#include <List.h>
#include "client.h"

using namespace android;

int prepareServerSocket() {
    int serverSocket = android_get_control_socket("g729");
    if (listen(serverSocket, 5) == -1) {
        ALOGE("listen: %s", strerror(errno));
        return -1;
    }
    return serverSocket;
}

int listenServerSocket(int serverSocket) {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(serverSocket, &set);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int result = select(serverSocket + 1, &set, NULL, NULL, &timeout);
    if (result == 0) {
        return 0;
    } else if (result < 0) {
        ALOGE("Critical error was occurred in server socket");
        return result;
    }
    int socket = accept(serverSocket, NULL, 0);
    if (socket <= 0) {
        return 0;
    }
    return socket;
}

sp<Client> createClient(int socket) {
    sp<Client> client = new Client(socket);
    client->run();
    return client;
}

void cleanupGarbage(List< sp<Client> > &clients) {
    for (List< sp<Client> >::iterator it = clients.begin(); it != clients.end(); ) {
        if (it->get() == NULL || !it->get()->valid()) {
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}

int main() {
    List< sp<Client> > clients;

    signal(SIGPIPE, SIG_IGN);
    int serverSocket = prepareServerSocket();
    LOG_ALWAYS_FATAL_IF(serverSocket < 0, "Cannot found server socket");
    while (true) {
        int socket = listenServerSocket(serverSocket);
        LOG_ALWAYS_FATAL_IF(socket < 0, "Server socket was died. Restart deamon automatically");
        if (socket == 0) continue;
        clients.push_back(createClient(socket));
        cleanupGarbage(clients);
    }
    clients.clear();
    return 1;
}
