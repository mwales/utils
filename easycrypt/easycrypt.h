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
 * Enrypts a single buffer of plaintext with AES in cipherblock chaining mode.
 *
 * @note Ciphertext in CBC mode will be longer than plaintext due to required padding
 *
 * @param plaintext Buffer of plaintext data
 * @param plaintextLength The length of plaintext data that is to be encrypted
 * @param keyLength Should be 128, 192, or 256
 * @param key AES key to encrypt data with
 * @param iv 16-bytes of data for the CBC IV
 * @return NULL on failure, or a buffer to ciphertext data.  The buffer should be delete[] after use
 */
unsigned char* cbcEncrypt(unsigned char const * plaintext,
                        uint32_t plaintextLength,
                        int keyLength,
                        unsigned char const * key,
                        unsigned char const * iv);

/**
 * Decrypts a single buffer of ciphertext with AES in cipherblock chaining mode
 *
 * @note Plaintext data will be slightly smaller than the ciphertext data
 *
 * @param ciphertext Buffer of ciphertext data
 * @param ciphertextLength The length of plaintext data that is to be encrypted
 * @param keyLength Should be 128, 192, or 256
 * @param key AES key to encrypt data with
 * @param iv 16-bytes of data for the CBC IV
 * @param[out] plaintextLength The length of the plaintext data (padding removed)
 * @param[out] paddingValid Returns true if the padding was valid3
 * @return NULL on failure, or a buffer to plaintext data.  The buffer should be delete[] after use
 */
unsigned char* cbcDecrypt(unsigned char const * ciphertext,
                          uint32_t ciphertextLength,
                          int keyLength,
                          unsigned char const * key,
                          unsigned char const * iv,
                          uint32_t* plaintextLength,
                          bool* paddingValid);

/**
 * Enrypts a single buffer of plaintext with AES in counter (CTR) mode.
 *
 * @note Never reuse the same IV / nonce
 *
 * @param plaintext Buffer of plaintext data
 * @param plaintextLength The length of plaintext data that is to be encrypted
 * @param keyLength Should be 128, 192, or 256
 * @param key AES key to encrypt data with
 * @param iv Nonce.  16-bytes for counter.  Caller needs to increment this once for each block of data
 *           encrypted
 * @return NULL on failure, or a buffer to ciphertext data.  The buffer should be delete[] after use
 */
unsigned char* ctrEncrypt(unsigned char const * plaintext,
                          uint32_t plaintextLength,
                          int keyLength,
                          unsigned char const * key,
                          unsigned char const * iv);

/**
 * Decrypts a single buffer of ciphertext with AES in counter mode
 *
 * @param ciphertext Buffer of ciphertext data
 * @param ciphertextLength The length of plaintext data that is to be encrypted
 * @param keyLength Should be 128, 192, or 256
 * @param key AES key to encrypt data with
 * @param iv 16-bytes of data for the counter nonce
 * @return NULL on failure, or a buffer to plaintext data.  The buffer should be delete[] after use
 */
unsigned char* ctrDecrypt(unsigned char const * ciphertext,
                          uint32_t ciphertextLength,
                          int keyLength,
                          unsigned char const * key,
                          unsigned char const * iv);

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
