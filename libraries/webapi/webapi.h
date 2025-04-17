#ifndef WEBAPI_H
#define WEBAPI_H

#include <stdbool.h>

typedef struct
{
	char* protocol; // "http"
	int portNumber;
	char* webHost;
	char* filePath;
} UrlDecoded;

void freeUrlDecoded(UrlDecoded* freeMe);

UrlDecoded* decodeUrl(char* url, bool* success);

int httpRequest(char* url, int port, char* responseBuffer, int* respBufferLen);
int httpsRequest(char* url, int port, char* responseBuffer, int* respBufferLen);


#endif
