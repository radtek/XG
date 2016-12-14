/*
* Modified by Henry Rawas (henryr@schakra.com)
*  - make it compatible with Visual Studio builds
*  - added wstrtod to handle INF, NAN
*  - added gettimeofday routine
*  - modified rename to retry after failure
*/

#ifdef _WIN32

#include <process.h>
#include <stdlib.h>
#include <errno.h>
#ifndef FD_SETSIZE
#define FD_SETSIZE 16000
#endif
#include <winsock2.h>
#include <windows.h>
#include <signal.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <string.h>


/* Redefined here to avoid redis.h so it can be used in other projects */
#define REDIS_NOTUSED(V) ((void) V)
#define REDIS_THREAD_STACK_SIZE (1024*1024*4)


/* Winsock requires library initialization on startup  */
int w32initWinSock(void) 
{
    WSADATA t_wsa;
    WORD wVers;
    int iError;

    wVers  = MAKEWORD(2, 2);
    iError = WSAStartup(wVers, &t_wsa);

    if(iError != NO_ERROR || LOBYTE(t_wsa.wVersion) != 2 || HIBYTE(t_wsa.wVersion) != 2 ) {
        return 0; /* not done; check WSAGetLastError() for error number */
    };

    return 1;
}

/* BSD sockets compatibile replacement */
int replace_setsockopt(int socket, int level, int optname, const void *optval, int optlen)
{
	return (setsockopt)((SOCKET)socket, level, optname, (const char *)optval, optlen);
}

int strerror_r(int err, char* buf, size_t buflen) 
{
    int size = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL,
                            err,
                            0,
                            buf,
                            (DWORD)buflen,
                            NULL);
    if (size == 0) {
        char* strerr = strerror(err);
        if (strlen(strerr) >= buflen) {
            errno = ERANGE;
            return -1;
        }
        strcpy(buf, strerr);
    }
    if (size > 2 && buf[size - 2] == '\r') {
        /* remove extra CRLF */
        buf[size - 2] = '\0';
    }
    return 0;
}

#endif
