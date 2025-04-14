#include <stdio.h>
#include <string.h>

#include "webapi.h"

#define BIG_BUFFER_LEN (64 * 1024)

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s http_url\n", argv[0]);
		return 1;
	}

	char bigBuf[BIG_BUFFER_LEN];
	memset(bigBuf, 0, BIG_BUFFER_LEN);

	bool success = 0;
	printf("Challing URL decoder on %s\n", argv[1]);

	UrlDecoded* decodedUrl = decodeUrl(argv[1], &success);

	printf("Success = %s\n", success ? "true" : "false");
	if (success)
	{
		printf("Protocol = %s\n", decodedUrl->protocol);
		printf("Port = %d\n", decodedUrl->portNumber);
		printf("Web Host = %s\n", decodedUrl->webHost);
		printf("Filepath = %s\n", decodedUrl->filePath);
		freeUrlDecoded(decodedUrl);
	}

	int bufLenAndBytesRx = BIG_BUFFER_LEN;
	int errCode = httpRequest(argv[1], 80, bigBuf, &bufLenAndBytesRx);

	printf("Error code = %d\n", errCode);
	printf("Bytes asked for / received = %d\n", bufLenAndBytesRx);
	printf("Response:\n");
	printf("%s\n", bigBuf);

	return 0;
}
