#include <iostream>
#include <string.h>
#include "easycrypt.h"

#include "../hexdump/hexdump.h"

// g++ test.cpp easycrypt.cpp ../hexdump/hexdump.cpp -l crypto -o test

int main(int argc, char** argv)
{
   std::cout << "MD5 size = " << getChecksumLength(CS_MD5) << std::endl;

   unsigned char const * const ALPHABET_STRING = (unsigned char * const) "abcdefghijklmnopqrstuvwxyz";

   std::cout << "Plaintext:" << std::endl;
   hexDump(ALPHABET_STRING, strlen( (char*) ALPHABET_STRING), stdout, true);

   std::cout << "MD5 Sum:" << std::endl;
   unsigned char* csBuffer = new unsigned char[getChecksumLength(CS_MD5)];
   computeChecksum(ALPHABET_STRING,
                   strlen( (char*) ALPHABET_STRING),
                   CS_MD5,
                   csBuffer);
   hexDump(csBuffer, getChecksumLength(CS_MD5));
   delete[] csBuffer;

   std::cout << "SHA1 Sum:" << std::endl;
   csBuffer = new unsigned char[getChecksumLength(CS_SHA1)];
   computeChecksum(ALPHABET_STRING,
                   strlen( (char*) ALPHABET_STRING),
                   CS_SHA1,
                   csBuffer);
   hexDump(csBuffer, getChecksumLength(CS_SHA1));
   delete[] csBuffer;

   std::cout << "SHA 256 Sum:" << std::endl;
   csBuffer = new unsigned char[getChecksumLength(CS_SHA256)];
   computeChecksum(ALPHABET_STRING,
                   strlen( (char*) ALPHABET_STRING),
                   CS_SHA256,
                   csBuffer);
   hexDump(csBuffer, getChecksumLength(CS_SHA256));
   delete[] csBuffer;

   std::cout << "SHA 512 Sum:" << std::endl;
   csBuffer = new unsigned char[getChecksumLength(CS_SHA512)];
   computeChecksum(ALPHABET_STRING,
                   strlen( (char*) ALPHABET_STRING),
                   CS_SHA512,
                   csBuffer);
   hexDump(csBuffer, getChecksumLength(CS_SHA512));
   delete[] csBuffer;



   return 0;
}
