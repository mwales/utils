#ifndef EASY_CRYPT_H
#define EASY_CRYPT_H

#include <stdint.h>

/**
 * Must have libssl-dev package installed
 *
 * To compile:  g++ easycrypt.cpp -lcrypto other-files
 *
 */

/**
 * Returns the size of the ciphertext (because the ciphertext will require at least 1-16 padding
 * bytes)
 *
 * @param plaintextLength The length of the plaintext
 * @return The length of the cipher text
 */
uint32_t getCbcCipherTextSize(uint32_t plaintextLength);

/**
 * Enrypts a single buffer of plaintext with AES in cipherblock chaining mode
 *
 * @param plaintext Buffer of plaintext data
 * @param plaintextLength The length of plaintext data that is to be encrypted
 * @param keyLength Should be 128, 192, or 256
 * @param key AES key to encrypt data with
 * @param iv 16-bytes of data for the CBC IV
 * @return NULL on failure, or a buffer to ciphertext data.  The buffer should be free() after use
 */
unsigned char* cbcCrypt(unsigned char * plaintext,
                        uint32_t plaintextLength,
                        int keyLength,
                        unsigned char * key,
                        unsigned char * iv);

/**
 * Decrypts a single buffer of ciphertext with AES in cipherblock chaining mode
 *
 * @param ciphertext Buffer of ciphertext data
 * @param ciphertextLength The length of plaintext data that is to be encrypted
 * @param keyLength Should be 128, 192, or 256
 * @param key AES key to encrypt data with
 * @param iv 16-bytes of data for the CBC IV
 * @param[out] paddingValid Returns true if the padding was valid3
 * @return NULL on failure, or a buffer to plaintext data.  The buffer should be free() after use
 */
unsigned char* cbcDecrypt(unsigned char * ciphertext,
                          uint32_t ciphertextLength,
                          int keyLength,
                          unsigned char * key,
                          unsigned char * iv,
                          bool* paddingValid);

enum ChecksumType {
   CS_MD5,
   CS_SHA1,
   CS_SHA256,
   CS_SHA512
};

/**
 * Returns the number of bytes that are required to store the checksum of given type
 */
int getChecksumLength(enum ChecksumType t);

/**
 * Computes checksum on the buffer provided and stores in csOut
 * @param buffer Data to compute checksum on
 * @param bufLength Length of data buffer
 * @param t Checksum type to compute
 * @param[out] csOut Checksum output (memory must be allocated before calling)
 */
void computeChecksum(unsigned char const * const buffer,
                     uint32_t bufLength,
                     enum ChecksumType t,
                     unsigned char* csOut);

#endif
