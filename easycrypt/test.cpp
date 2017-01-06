#include <iostream>
#include <string.h>
#include "easycrypt.h"

#include "../hexdump/hexdump.h"

// To compile
// g++ test.cpp easycrypt.cpp ../hexdump/hexdump.cpp -l crypto -o test

// To verify using command line openssl / hashing functiosns:
// md5sum data.txt
// sha1sum data.txt
// sha256sum data.txt
// sha512sum data.txt
// openssl enc -aes-128-cbc -in data.txt -out enc.txt -K 00020406080a0c0e10121416181a1c1e -iv 000102030405060708090a0b0c0d0e0f
// openssl enc -aes-192-cbc -in data.txt -out enc.txt -K 00020406080a0c0e10121416181a1c1e20222426282a2c2e -iv 000102030405060708090a0b0c0d0e0f
// openssl enc -aes-256-cbc -in data.txt -out enc.txt -K 00020406080a0c0e10121416181a1c1e20222426282a2c2e30323436383a3c3e -iv 000102030405060708090a0b0c0d0e0f


int main(int argc, char** argv)
{
   std::cout << "********************************************************************************" << std::endl;
   std::cout << "                                 Hashing Tests:" << std::endl;
   std::cout << "********************************************************************************" << std::endl;

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

   std::cout << "********************************************************************************" << std::endl;
   std::cout << "                               Encryption Tests:" << std::endl;
   std::cout << "********************************************************************************" << std::endl;

   int ctLength = getCbcCipherTextSize(strlen( (char*) ALPHABET_STRING));

   unsigned char iv[] = { 0x0, 0x1, 0x2, 0x3,
                         0x4, 0x5, 0x6, 0x7,
                         0x8, 0x9, 0xa, 0xb,
                         0xc, 0xd, 0xe, 0xf };

   unsigned char aes256Key[256 / 8];
   for(int i = 0; i < 256 / 8; i++)
   {
      aes256Key[i] = i * 2;
   }

   std::cout << "IV for all encryptions:" << std::endl;
   hexDump(iv, 0x10);

   for(int keySize = 128; keySize <= 256; keySize += 64)
   {
      std::cout << "AES " << keySize << " CBC cipher mode encyption test" << std::endl;
      std::cout << "Key:" << std::endl;
      hexDump(aes256Key, keySize / 8);

      unsigned char* ct = cbcCrypt(ALPHABET_STRING, strlen( (char*) ALPHABET_STRING), keySize, aes256Key, iv);

      std::cout << "CT:" << std::endl;
      hexDump(ct, ctLength);

      uint32_t ptLength = 0;
      bool paddingValid = false;
      unsigned char* verify = cbcDecrypt(ct, ctLength, keySize, aes256Key, iv, &ptLength, &paddingValid);

      std::cout << "Decryption verifiction with plaintext length = " << ptLength << ", and paddingValid=" << ( paddingValid ? "true" : "false") << std::endl;
      std::cout << "Verify Dump:" << std::endl;
      hexDump(verify, ptLength);

      std::cout << "Length compare = " << (ptLength == strlen((char*)ALPHABET_STRING) ? "VERIFIED" : "FAILED") << std::endl;
      std::cout << "Memory compare = " << (memcmp(verify, ALPHABET_STRING, ptLength) == 0 ? "VERIFIED" : "FAILED") << std::endl;

      delete[] ct;
      delete[] verify;
   }



   return 0;
}
