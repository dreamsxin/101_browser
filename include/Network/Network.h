/*
* Copyright 2011 Wolfgang Keller
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef _NetworkWin_Network_h
#define _NetworkWin_Network_h

#ifdef __cplusplus
extern "C" {
#endif

#include "MiniStdlib/declspec.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket, AF_INET etc.
#include <arpa/inet.h>  // inet_addr
#include <unistd.h>     // close
#endif

#ifdef _WIN32
typedef SOCKET socket_t;
typedef SOCKADDR sockaddr_t;
typedef SOCKADDR_IN sockaddr_in_t;
typedef int socklen_t; // for use in recvfrom
#else
typedef int socket_t;
typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR   (-1)
#define closesocket close
#endif

DLLEXPORT int startupNetwork();
DLLEXPORT int cleanupNetwork();

#ifdef __cplusplus
}
#endif

#endif
