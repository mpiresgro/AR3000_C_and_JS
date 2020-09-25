#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../PK_Data_Extractor/data_extractor.c"

#ifndef ADDRESS
#define ADDRESS "127.0.0.1"
#endif

/**
 * PORT represents the server port we want to connect to.
 * Naturally, it must live in the range of [1,65k).
 */
#ifndef PORT
#define PORT 4000
#endif

#ifndef BUFFERSIZE
#define BUFFERSIZE 64
#endif

#ifndef TIMEOUT
#define TIMEOUT -1 //5000000
#endif

extern int start_tcp_client();