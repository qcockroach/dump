#ifndef AES_H
#define AES_H

#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <string.h>
#include <string>
#include "CipherInterface.h"
#include <ctype.h>

using namespace std;

/**
 * Implement a AES cipher
 */
class AES: public CipherInterface
{
	/* The public members */
	public:

	/**
 	* The default constructor
 	*/
	AES() {}

	/**
 	* Sets the key to use
 	* @param key - the key to use
 	* @return - True if the key is valid and False otherwise
 	*/
	virtual bool setKey(const unsigned char* key);

	/**
 	* Encrypts a plaintext string
 	* @param plainText - the plaintext string
 	* @return - the encrypted ciphertext string
 	*/
	virtual unsigned char* encrypt(const unsigned char* plainText);
	virtual unsigned char* decrypt(const unsigned char* cipherText);

	/* The protected members */
	protected:
		// Functions to convert AES key
		unsigned char twoCharToHexByte(const unsigned char* twoChars);
		unsigned char charToHex(const char& character);
	//TODO: declare the AES key
		AES_KEY AESKey;
};

#endif
