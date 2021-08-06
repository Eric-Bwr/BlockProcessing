#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#endif
#ifdef linux
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#endif

namespace Network {
#ifdef _WIN32
    typedef SOCKET SocketHandle;
#else
#define INVALID_SOCKET ~0
#define SOCKET_ERROR -1
#define closesocket(fd) close(fd)
#define WSAPoll(fds,nfds,tm) poll(fds,nfds,tm)
#define WSAGetLastError() errno
#define WSAEWOULDBLOCK 10035
#define FALSE 0
#define TRUE 1
    typedef int SocketHandle;
    typedef int BOOL;
    typedef pollfd WSAPOLLFD;
#endif
}