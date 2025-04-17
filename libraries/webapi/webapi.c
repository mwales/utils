#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// TLS support
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/error.h"

#include "webapi.h"

void printErrno()
{
	fprintf(stderr, "   errno = %d = %s\n", errno, strerror(errno));
}

void printMbedTlsError(int errorVal)
{
	char errorMsg[512];
	memset(errorMsg, 0, 512);
	mbedtls_strerror(errorVal, errorMsg, 512);
	fprintf(stderr, "mbed TLS error = 0x%x = %s\n", errorVal, errorMsg);
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

		// Set the filepath
		retVal->filePath = (char*) malloc( strlen(filePath) + 1);
		strcpy(retVal->filePath, filePath);
	}
	else
	{
		// We didn't find a /
		hostLen = strlen(startOfHost);

		retVal->filePath = (char*) malloc( 2 );
		strcpy(retVal->filePath, "/");
	}

	retVal->portNumber = 80;

	retVal->protocol = (char*) malloc(protocolLen + 1);
	memset(retVal->protocol, 0, protocolLen + 1);
	memcpy(retVal->protocol, url, protocolLen);

	retVal->webHost = malloc(hostLen + 1);
	memset(retVal->webHost, 0, hostLen + 1);
	memcpy(retVal->webHost, url + protocolLen + 3, hostLen);

	*success = true;
	return retVal;

fail:
	*success = false;
	return NULL;

}

void my_debug( void *ctx, int level,
               const char *file, int line, const char *str )
{
	((void) level);
	fprintf( (FILE *) ctx, "%s:%04d: %s", file, line, str );
	fflush(  (FILE *) ctx  );
}

int httpsRequest(char* url, int port, char* responseBuffer, int* respBufferLen)
{
	// TLS initialization
	mbedtls_net_context       server_fd;
	mbedtls_entropy_context   entropy;
	mbedtls_ctr_drbg_context  ctr_drbg;
	mbedtls_ssl_context       ssl;
	mbedtls_ssl_config        conf;
	mbedtls_x509_crt          cacert;
	char* pers = "https_request_lib_demo_test";

	mbedtls_net_init( &server_fd );
	mbedtls_ssl_init( &ssl );
	mbedtls_ssl_config_init( &conf );
	mbedtls_x509_crt_init( &cacert );
	mbedtls_ctr_drbg_init( &ctr_drbg );
	mbedtls_entropy_init( &entropy );

	int retVal = 0;
	
	int mbedRet = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, 
	                                     (const unsigned char *) pers, strlen( pers ) );
	if (mbedRet != 0)
	{
		fprintf(stderr, " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", mbedRet );
		printMbedTlsError(mbedRet);
		retVal = 10;
		goto exit;
	}

	bool success = false;
	UrlDecoded* du = decodeUrl(url, &success);

	if ( (!du) || (!success) )
	{
		fprintf(stderr, "Web request failed, couldn't decode URL %s\n", url);
		retVal = 1;
		goto exit;
	}

	if (strcmp(du->protocol, "https"))
	{
		fprintf(stderr, "httpsRequest called, but protocol %s isn't https\n", du->protocol);
		retVal = 2;
		goto exit;
	}

	mbedRet = mbedtls_ssl_config_defaults( &conf, MBEDTLS_SSL_IS_CLIENT,
	                                       MBEDTLS_SSL_TRANSPORT_STREAM,
	                                       MBEDTLS_SSL_PRESET_DEFAULT );
	if (mbedRet != 0)
	{
		fprintf(stderr, " failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", mbedRet );
		printMbedTlsError(mbedRet);
		retVal = 12;
		goto exit;
	}

	mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_OPTIONAL );

	// From chat gpt example
	mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);

	mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );

	mbedRet = mbedtls_ssl_setup(&ssl, &conf);
	if (mbedRet)
	{
		fprintf(stderr, "SSL setup error, retVal = %d\n", mbedRet);
		printMbedTlsError(mbedRet);
		retVal = 15;
		goto exit;
	}

	mbedRet =mbedtls_ssl_set_hostname( &ssl, du->webHost );
	if (mbedRet)
	{
		fprintf(stderr, " failed\n  ! mbedtls_ssl_set_hostname %s returned %d\n\n",
		        du->webHost, retVal );
		printMbedTlsError(mbedRet);
		retVal = 14;
		goto exit;
	}
	
	mbedRet = mbedtls_net_connect( &server_fd, du->webHost, "443", MBEDTLS_NET_PROTO_TCP);
	if (mbedRet)
	{
		fprintf(stderr, "mbedtls connect to %s:443 failed, ret code = %d\n",
		        du->webHost, mbedRet);
		printMbedTlsError(mbedRet);
		retVal = 11;
		goto exit;
	}
	
	printf("Socket connection complete\n");

	mbedtls_ssl_set_bio( &ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL );

	mbedRet = mbedtls_ssl_handshake(&ssl);
	if (mbedRet != 0)
	{
		fprintf(stderr, "Error performing the SSL handshake, retVal = %d\n", mbedRet);
		printMbedTlsError(mbedRet);
		retVal = 12;
		goto exit;
	}

	printf("SSL handshake finished\n");

	char request[2048];
	snprintf(request, sizeof(request),"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", 
	         du->filePath, du->webHost);

	freeUrlDecoded(du);

	fprintf(stderr,"Http request created!\n%s\n", request);

	ssize_t bytesSent = mbedtls_ssl_write( &ssl, request, strlen(request));

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
		br = mbedtls_ssl_read(&ssl, responseBuffer + bytesRxTotal, *respBufferLen - bytesRxTotal);
		fprintf(stderr, "Received %ld bytes\n", br);

		if (br == 0)
		{
			break;
		}

		bytesRxTotal += br;
	}

	*respBufferLen = bytesRxTotal;
	printf("Bytes total received %ld\n", bytesRxTotal);

exit:
	mbedtls_net_free( &server_fd );
	mbedtls_ssl_free( &ssl );
	mbedtls_ssl_config_free( &conf );
	mbedtls_ctr_drbg_free( &ctr_drbg );
	mbedtls_entropy_free( &entropy );

	return retVal;
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


