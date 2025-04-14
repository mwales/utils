#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "webapi.h"

void printErrno()
{
	fprintf(stderr, "   errno = %d = %s\n", errno, strerror(errno));
}

void freeUrlDecoded(UrlDecoded* freeMe)
{
	free(freeMe->protocol);
	free(freeMe->webHost);
	free(freeMe->filePath);
	free(freeMe);
}

UrlDecoded* decodeUrl(char* url, bool* success)
{
	char* endOfProtocol = strstr(url, ":");
	if (!endOfProtocol)
	{
		fprintf(stderr, "URL %s has no colon to end protocol\n", url);
		goto fail;
	}

	// Confirm that :// is next
	int protocolLen = endOfProtocol - url;

	int urlLen = strlen(url);

	if (protocolLen + 2 > urlLen)
	{
		fprintf(stderr, "URL %s is too short after protocol\n", url);
		goto fail;
	}

	if ( (url[protocolLen+1] != '/') || (url[protocolLen+2] != '/') )
	{
		fprintf(stderr, "URL %s missing the // after protocol\n", url);
		goto fail;
	}

	char* startOfHost = url + protocolLen + 3;
	char* endOfHost = strstr(startOfHost, "/");
	char* filePath;
	int hostLen;

	UrlDecoded* retVal = (UrlDecoded*) malloc( sizeof(UrlDecoded) );

	if (endOfHost)
	{
		// We found a /
		hostLen = endOfHost - startOfHost;
		filePath = startOfHost + hostLen;

		printf("Host len = %d\n", hostLen);


	}
	else
	{
		// We didn't find a /
		hostLen = strlen(startOfHost);

	}

	retVal->portNumber = 80;

	retVal->protocol = (char*) malloc(protocolLen + 1);
	memset(retVal->protocol, 0, protocolLen + 1);
	memcpy(retVal->protocol, url, protocolLen);

	retVal->webHost = malloc(hostLen + 1);
	memset(retVal->webHost, 0, hostLen + 1);
	memcpy(retVal->webHost, url + protocolLen + 3, hostLen);

	retVal->filePath = (char*) malloc( strlen(filePath) + 1);
	strcpy(retVal->filePath, filePath);

	*success = true;
	return retVal;

fail:
	*success = false;
	return NULL;

}

int httpRequest(char* url, int port, char* responseBuffer, int* respBufferLen)
{

	bool success = false;
	UrlDecoded* du = decodeUrl(url, &success);

	if ( (!du) || (!success) )
	{
		fprintf(stderr, "Web request failed, couldn't decode URL %s\n", url);
		return 1;
	}

	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo* res;
	if (getaddrinfo(du->webHost, "80", &hints, &res) != 0)
	{
		fprintf(stderr, "Error looking up host %s.\n", du->webHost);
		printErrno();
		freeUrlDecoded(du);
		return 3;
	}


	int clientFd;
	struct addrinfo* rp;
	for(rp = res; rp != NULL; rp = rp->ai_next)
	{

		clientFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (clientFd == -1)
		{
			fprintf(stderr, "Error creating socket\n");
			printErrno();
			continue;
		}
		else
		{
			printf("A socket was created successfully!\n");
		}


		// Lets try connecting
		if (connect(clientFd, rp->ai_addr, rp->ai_addrlen))
		{
			fprintf(stderr, "Error connecing on the socket\n");
			printErrno();
			freeUrlDecoded(du);
			return 4;
		}
		else
		{
			// success
			printf("Connect success\n");
			break;
		}
	}

	if (clientFd == -1)
	{
		// We exhausted all attempts
		fprintf(stderr, "Error creating socket, and now we all possible sockets to try....\n");
		printErrno();
		freeUrlDecoded(du);
		return 5;
	}

	freeaddrinfo(res);

	printf("Connect finished\n");

	char request[2048];
	snprintf(request, sizeof(request),"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", 
	         du->filePath, du->webHost);

	freeUrlDecoded(du);

	fprintf(stderr,"Http request created!\n%s\n", request);

	ssize_t bytesSent = send(clientFd, request, strlen(request), 0);

	fprintf(stderr, "Bytes sent %ld\n", bytesSent);

	if (bytesSent == -1)
	{
		fprintf(stderr, "Error sending the reuqest\n");
		printErrno();
		return 6;
	}

	fprintf(stderr, "Request bytes sent successfully = %ld\n", bytesSent);

	ssize_t bytesRxTotal = 0;
	ssize_t br = 0;
	while(bytesRxTotal < *respBufferLen)
	{
		br = read(clientFd, responseBuffer + bytesRxTotal, *respBufferLen - bytesRxTotal);
		fprintf(stderr, "Received %ld bytes\n", br);

		if (br == 0)
		{
			break;
		}

		bytesRxTotal += br;
	}

	*respBufferLen = bytesRxTotal;
	printf("Bytes total received %ld\n", bytesRxTotal);

	return 0;
}


