#include "easycrypt.h"
#include <openssl/aes.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

#ifdef EASY_CRYPT_DEBUG

#include <stdio.h>
#include "../hexdump/hexdump.h"

#define EC_DEBUG(...) printf(__VA_ARGS__)
#else
#define EC_DEBUG // Nothing
#endif


uint32_t getCbcCipherTextSize(uint32_t plaintextLength)
{
   uint32_t blocksRequired = plaintextLength / 16 + 1;
   return blocksRequired * 16;
}

unsigned char* cbcCrypt(unsigned char const * plaintext,
                        uint32_t plaintextLength,
                        int keyLength,
                        unsigned char const * key,
                        unsigned char const * iv)
{
   uint32_t ctSize = getCbcCipherTextSize(plaintextLength);
   int paddingByte = (ctSize - plaintextLength);

   if ( (keyLength != 128) && (keyLength != 192) && (keyLength != 256))
   {
      EC_DEBUG("Invalid key length of %d\n", keyLength);
      return NULL;
   }

   unsigned char ivCopy[0x10];
   memcpy(ivCopy, iv, 0x10);

   unsigned char* cipherText    = new unsigned char[ctSize];
   unsigned char* ptWithPadding = new unsigned char[ctSize];

   EC_DEBUG("\nEncrypting message of length %d, cipher text length = %d, padding byte = %d\n",
            plaintextLength, ctSize, paddingByte);

   int currentByte;
   for(currentByte = 0; currentByte < plaintextLength; currentByte++)
   {
       ptWithPadding[currentByte] = plaintext[currentByte];
   }

   while (currentByte < ctSize)
   {
       ptWithPadding[currentByte++] = paddingByte;
   }

   AES_KEY aesContext;
   AES_set_encrypt_key(key, keyLength, &aesContext);
   AES_cbc_encrypt(ptWithPadding, cipherText, ctSize, &aesContext, ivCopy, AES_ENCRYPT);

#ifdef EASY_CRYPT_DEBUG
    printf("Plaintext:\n");
    hexDump(ptWithPadding, ctSize);
    printf("Ciphertext:\n");
    hexDump(cipherText, ctSize);
#endif

   delete[] ptWithPadding;
   return cipherText;
}

unsigned char* cbcDecrypt(unsigned char const * ciphertext,
                          uint32_t ciphertextLength,
                          int keyLength,
                          unsigned char const * key,
                          unsigned char const * iv,
                          uint32_t* plaintextLength,
                          bool* paddingValid)
{
   if (ciphertextLength % 16 != 0)
   {
       EC_DEBUG("Ciphertext length of %d is invalid block size\n", ciphertextLength);
       return NULL;
   }

   if ( (keyLength != 128) && (keyLength != 192) && (keyLength != 256))
   {
      EC_DEBUG("Invalid key length of %d\n", keyLength);
      return NULL;
   }

   unsigned char* plaintext = new unsigned char[ciphertextLength];

   unsigned char ivCopy[0x10];
   memcpy(ivCopy, iv, 0x10);

   AES_KEY aesContext;
   AES_set_decrypt_key(key, keyLength, &aesContext);
   AES_cbc_encrypt(ciphertext, plaintext, ciphertextLength, &aesContext, ivCopy, AES_DECRYPT);

#ifdef EASY_CRYPT_DEBUG
   printf("\nDecrypting Ciphertext:\n");
   hexDump(ciphertext, ciphertextLength);

   printf("Plaintext:\n");
   hexDump(plaintext, ciphertextLength);
#endif

   // Truncate off the padding
   int rxPaddingByte = plaintext[ciphertextLength - 1];
   if (rxPaddingByte > 0x10)
   {
       EC_DEBUG("Plaintext had invalid padding byte(s)\n");
       *paddingValid = false;
       delete[] plaintext;
       return NULL;
   }

   for(uint32_t paddingIndex = ciphertextLength - rxPaddingByte;
       paddingIndex < ciphertextLength;
       paddingIndex++)
   {
      if (plaintext[paddingIndex] != rxPaddingByte)
      {
         EC_DEBUG("Plaintext padding bytes inconsistent at byte 0x%02x, value is 0x%02x\n", paddingIndex, plaintext[paddingIndex]);
         *paddingValid = false;
         delete[] plaintext;
         return NULL;
      }
   }

   *plaintextLength = ciphertextLength - rxPaddingByte;
   *paddingValid = true;
   return plaintext;
}

/**
 * I've left some of the unneccessary stuff in here as comments incase I want to use this as a
 * template in other code for CBC mode as well.  The EVP API is now supposed to be the preferred
 * API for encrypting with OpenSSL.
 */
unsigned char* ctrEncrypt(unsigned char const * plaintext,
                          uint32_t plaintextLength,
                          int keyLength,
                          unsigned char const * key,
                          unsigned char const * iv)
{
   if ( (keyLength != 128) && (keyLength != 192) && (keyLength != 256))
   {
      EC_DEBUG("Invalid key length of %d\n", keyLength);
      return NULL;
   }

   // The majority of this code came straight from the OpenSSL Wiki on EVP encryption
   EVP_CIPHER_CTX *ctx;
   unsigned char* ciphertext = new unsigned char[plaintextLength];

   // We don't really need to use this because we are using CTR mode, but the EVP methods require it
   int outlen;

   /* Create and initialise the context */
   ctx = EVP_CIPHER_CTX_new();
   if(!ctx)
   {
      EC_DEBUG("Error creating a new EVP contenxt\n");
      delete[] ciphertext;
      return NULL;
   }

   // Initialise the encryption operation. IMPORTANT - ensure you use a key and IV size
   // appropriate for your cipher. The modes can be found on the man page for
   // evp_encryptupdate

   int success;
   if (keyLength == 128)
   {
      success = EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);
   }
   else if (keyLength == 192)
   {
      success = EVP_EncryptInit_ex(ctx, EVP_aes_192_ctr(), NULL, key, iv);
   }
   else
   {
      // AES 256 is left
      success = EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);
   }

   if(1 != success)
   {

      EC_DEBUG("Error when calling EVP encypt init");
      EVP_CIPHER_CTX_free(ctx);
      delete[] ciphertext;
      return NULL;
   }

   // Provide the message to be encrypted, and obtain the encrypted output. EVP_EncryptUpdate
   // can be called multiple times if necessary.  This method gets a little weird when using
   // block ciphers that have to encrypt a whole block and you provide less than a block.
   if(1 != EVP_EncryptUpdate(ctx, ciphertext, &outlen, plaintext, plaintextLength))
   {
      EC_DEBUG("Error when calling EVP encypt update");
      EVP_CIPHER_CTX_free(ctx);
      delete[] ciphertext;
      return NULL;
   }

   // Finalise the encryption. Further ciphertext bytes may be written at this stage.  This
   // shouldn't do anything for CTR mode, but CBC mode would add padding bytes
   // EVP_EncryptFinal_ex(ctx, ciphertext + outlen, &outlen);

   /* Clean up */
   EVP_CIPHER_CTX_free(ctx);

   return ciphertext;
}


int getChecksumLength(enum ChecksumType t)
{
   // Pulled these number out of the documentation on openssl.org
   switch(t)
   {
      case CS_MD5:
         return 16;
      case CS_SHA1:
         return 20;
      case CS_SHA256:
         return 32;
      case CS_SHA512:
         return 64;
      default:
         EC_DEBUG("Checksum type of %d is invalid\n", t);
         return 0;
   }
}

void computeMd5Checksum(unsigned char const * const buffer,
                        uint32_t bufLength,
                        unsigned char* csOut)
{
   MD5_CTX csContext;
   MD5_Init(&csContext);
   MD5_Update(&csContext, buffer, bufLength);
   MD5_Final(csOut, &csContext);
}

void computeSha1Checksum(unsigned char const * const buffer,
                         uint32_t bufLength,
                         unsigned char* csOut)
{
   SHA_CTX csContext;
   SHA1_Init(&csContext);
   SHA1_Update(&csContext, buffer, bufLength);
   SHA1_Final(csOut, &csContext);
}

void computeSha256Checksum(unsigned char const * const buffer,
                        uint32_t bufLength,
                        unsigned char* csOut)
{
   SHA256_CTX csContext;
   SHA256_Init(&csContext);
   SHA256_Update(&csContext, buffer, bufLength);
   SHA256_Final(csOut, &csContext);
}

void computeSha512Checksum(unsigned char const * const buffer,
                        uint32_t bufLength,
                        unsigned char* csOut)
{
   SHA512_CTX csContext;
   SHA512_Init(&csContext);
   SHA512_Update(&csContext, buffer, bufLength);
   SHA512_Final(csOut, &csContext);
}

void computeChecksum(unsigned char const * const buffer,
                     uint32_t bufLength,
                     enum ChecksumType t,
                     unsigned char* csOut)
{
   switch(t)
   {
      case CS_MD5:
         computeMd5Checksum(buffer, bufLength, csOut);
         return;
      case CS_SHA1:
         computeSha1Checksum(buffer, bufLength, csOut);
         return;
      case CS_SHA256:
         computeSha256Checksum(buffer, bufLength, csOut);
         return;
      case CS_SHA512:
         computeSha512Checksum(buffer, bufLength, csOut);
         return;
      default:
         EC_DEBUG("Checksum type of %d is invalid\n", t);
         return;
   }
}
