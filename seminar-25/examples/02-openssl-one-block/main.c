/*
 * REFERENCES:
 *  - https://wiki.openssl.org/index.php/Libcrypto_API
 *  - https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 *  - https://github.com/openssl/openssl
 *  - https://www.openssl.org/docs/man1.1.1/
 */

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

enum {
  SIZE = 128
};

typedef unsigned char byte;

int
encrypt(byte* plaintext,
        int plaintext_len,
        byte* key,
        byte* iv,
        byte* ciphertext);

int
decrypt(byte* ciphertext,
        int ciphertext_len,
        byte* key,
        byte* iv,
        byte* plaintext);

void
handle_errors(void);

int
main(int argc, char** argv){
    /*
     * Set up the key and iv. Do I need to say to not hard
     * code these in a real application? :-)
     */
    /* A 256 bit key */
    byte* key = (byte *)"01234567890123456789012345678901";
    /* A 128 bit IV */
    byte* iv = (byte *)"0123456789012345";

    /* Message to be encrypted */
    byte* plaintext = (byte *)"The quick brown fox jumps over the lazy dog";

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    byte ciphertext[SIZE];

    /* Buffer for the decrypted text */
    byte decryptedtext[SIZE];

    int decryptedtext_len;
    int ciphertext_len;

    /* Encrypt the plaintext */
    ciphertext_len = encrypt(
        /* byte* plaintext   = */ plaintext,
        /* int plaintext_len = */ strlen ((char *)plaintext),
        /* byte* key         = */ key,
        /* byte* iv          = */ iv,
        /* byte* ciphertext  = */ ciphertext
    );

    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(
        /* byte* ciphertext   = */ ciphertext,
        /* int ciphertext_len = */ ciphertext_len,
        /* byte* key          = */ key,
        /* byte* iv           = */ iv,
        /* byte* plaintext    = */ decryptedtext
    );

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    printf("Decrypted text is:\n%s\n", decryptedtext);

    return 0;
}

int
encrypt(byte* plaintext,
        int plaintext_len,
        byte* key,
        byte* iv,
        byte* ciphertext){

    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        handle_errors();
    }

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handle_errors();
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        handle_errors();
    }
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        handle_errors();
    }
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int
decrypt(byte* ciphertext,
        int ciphertext_len,
        byte* key,
        byte* iv,
        byte* plaintext){

    EVP_CIPHER_CTX* ctx;

    int len;
    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        handle_errors();
    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handle_errors();
    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        handle_errors();
    }
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        handle_errors();
    }
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void
handle_errors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}