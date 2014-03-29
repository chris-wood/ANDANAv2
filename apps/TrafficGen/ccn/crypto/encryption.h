//
//  encryption.h
//  namecrypto
//
//  Created by Paolo Gasti <pgasti@uci.edu> on 6/3/11.
//  Copyright 2011 Paolo Gasti. All rights reserved.
//

#ifndef __ndn_encryption__
#define __ndn_encryption__

#include <openssl/rsa.h>
#include <openssl/sha.h>

#include <ccn/charbuf.h>
#include <ccn/indexbuf.h>
#include <ccn/crypto/encryption.h>


#define KEYLEN 128/8 // symmetric cipher key length in bytes
#define IVLEN 128/8 // IV length in bits
#define MACKLEN 128/8 // mac key length in bits
#define NODE_KEYLEN 128/8 // Long term node key length
#define SESSION_KEYLEN 128/8 // length of a session key
#define SESSIONRAND_LENGTH 128/8 // length of the randomness provided by the node
#define SESSIONID_LENGTH IVLEN + SESSION_KEYLEN + MACLEN // length of a session identifier
#define MACLEN SHA256_DIGEST_LENGTH
#define PERLINKTAGSIZE 1


#define ERR_DECRYPTING_KEM      -1
#define ERR_DECRYPTING_DEM      -2
#define ERR_ALLOCATION_ERROR    -3
#define ERR_DECODING_CIPHERTEXT -4

#define NO_PER_LINK_ENCRYPTION          0
#define SYMMETRIC_PER_LINK_ENCRYPTION   1
#define ASYMMETRIC_PER_LINK_ENCRYPTION  2

struct ccn_pkey;


/*
  void
  ccn_crypto_name_asym_link_decrypt(struct ccn_pkey *privkey,
  unsigned char *encrypted,
  struct ccn_pkey **out_symkey,
  struct ccn_charbuf **out_decrypted,
  struct ccn_indexbuf **out_decrypted_comps);
*/

void
ccn_crypto_name_asym_encrypt(struct ccn_pkey *pubkey,
                             unsigned char *name,
                             const size_t length,
                             struct ccn_pkey *symkey,
                             unsigned char **out_name,
                             size_t *out_name_length);

void
ccn_crypto_name_asym_decrypt(struct ccn_pkey *privkey,
                             unsigned char *encrypted,
                             struct ccn_pkey **out_symkey,
                             struct ccn_charbuf **out_decrypted,
                             struct ccn_indexbuf **out_decrypted_comps);

void
ccn_crypto_name_sym_encrypt(struct ccn_pkey *sessionkey,
                            unsigned char *session_id,
                            unsigned char *name,
                            const size_t length,
                            struct ccn_pkey *symkey,
                            unsigned char **out_name,
                            size_t *out_name_length);

void
ccn_crypto_name_sym_decrypt(struct ccn_pkey *sessionkey,
                            unsigned char *encrypted,
                            size_t encrypted_length,
                            struct ccn_pkey **out_symkey,
                            struct ccn_charbuf **out_decrypted,
                            struct ccn_indexbuf **out_decrypted_comps);

void
ccn_crypto_content_encrypt(struct ccn_pkey *symkey,
                           unsigned char *content,
                           size_t length,
                           unsigned char **encrypted_content,
                           size_t *encrypted_length);

void
ccn_crypto_content_decrypt(struct ccn_pkey *symkey,
                           unsigned char *encrypted_content,
                           size_t length,
                           unsigned char **content,
                           size_t *content_length);


int decrypt_name(unsigned char * enc_name, RSA * key, unsigned char ** plaintext);
int encrypt_name(unsigned char * name, unsigned int name_length, RSA * key, unsigned char ** encrypted_name);
unsigned char * KDF(unsigned char * key, unsigned int keylen, char * s, unsigned int slen);
int getSessionKey(unsigned char * session_id, unsigned char ** session_key, unsigned char * node_key);
int createSession(unsigned char ** session_id, unsigned char ** key,unsigned char ** rand, unsigned char * user_provided_key, unsigned int userkey_len, unsigned char * node_key);
int encrypt_encode(unsigned char * name, unsigned int name_length, unsigned char * symmkey, unsigned int symmkey_length, RSA * key, unsigned char ** encrypted_name);
int decrypt_decode(char * encrypted_name, unsigned char ** symmkey, unsigned int * symmkey_length, RSA * key, unsigned char ** plaintext);
//int encrypt_name_for_node_B64(RSA * node_pubkey, unsigned char * privateName, int privateName_length, unsigned char * symmkey, unsigned int symmkey_length, unsigned char ** encryptedName);
//int decrypt_name_on_node_B64(char * ciphertext, RSA * node_pubkey, unsigned char ** symmkey, unsigned int * symmkey_length, unsigned char ** decryptedName);
int encrypt_name_for_node(RSA * node_pubkey, unsigned char * privateName, int privateName_length, unsigned char * symmkey, unsigned int symmkey_length, unsigned char ** encryptedName);
int decrypt_name_on_node(unsigned char * ciphertext, RSA * node_pubkey, unsigned char ** symmkey, unsigned int * symmkey_length, unsigned char ** decryptedName);
int session_encrypt_name_for_node(unsigned char * sessionkey, unsigned char * session_id, unsigned char * privateName, int privateName_length, unsigned char * symmkey, unsigned int symmkey_length, unsigned char ** encryptedName);
int session_decrypt_name_on_node(unsigned char * ciphertext, int ciphertext_length, unsigned char * node_key, unsigned char ** symmkey, unsigned int * symmkey_length, unsigned char ** decryptedName);
int symm_enc(unsigned char * plaintext, unsigned int plaintext_length, unsigned char * ciphertext, unsigned char * key);
int symm_enc_no_mac(unsigned char * plaintext, unsigned int plaintext_length, unsigned char * ciphertext, unsigned char * key);
int symm_dec_no_mac(unsigned char * ciphertext, unsigned int ciphertext_length, unsigned char * plaintext, unsigned char * key);
unsigned char * decrypt_data(unsigned char * ciphertext, unsigned int ciphertext_length, unsigned char * plaintext, unsigned int * len, unsigned char * key, unsigned int keylen);
unsigned char * encrypt_data(unsigned char * plaintext, unsigned int len, unsigned char * ciphertext, unsigned int * ciphertextlen, unsigned char * key, unsigned int keylen);
int session_encrypt_name_for_node_B64(unsigned char * sessionkey, unsigned char * session_id, unsigned char * privateName, int privateName_length, unsigned char * symmkey, unsigned int symmkey_length, unsigned char ** encryptedName);
int session_decrypt_name_on_node_B64(unsigned char * ciphertext, int ciphertext_length, unsigned char * node_key, unsigned char ** symmkey, unsigned int * symmkey_length, unsigned char ** decryptedName);
int verifyKeyFromNode(unsigned char * user_provided_key, unsigned int userkey_len, unsigned char * node_randomness, unsigned char * session_key);
#ifdef LINKENCRYPTION
int per_link_encrypt(RSA * node_pubkey, unsigned char * plaintext, int plaintext_length, unsigned char * symmkey, unsigned int symmkey_length, unsigned char ** encryptedName);

int per_link_decrypt(unsigned char * ciphertext, RSA * node_pubkey, unsigned char ** symmkey, unsigned int * symmkey_length, unsigned char ** decryptedName);

int is_per_link_encrypted(unsigned char * ciphertext);
#endif



#endif

