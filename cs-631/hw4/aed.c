#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/rand.h>
#include <openssl/evp.h>

#define BUF_SIZE    1024
#define KEY_SIZE    32
#define IV_SIZE     16
#define SALT_SIZE   8

int     flag_enc = -1;  /* init: -1, encryption: 1, decryption: 0 */
int     flag_print = 0;
char    *pass_file_path = NULL;

static void do_crypt(FILE *, FILE *, int, char *);
static char *get_passphrase(void);
static void usage(void);


int
main(int argc, char **argv)
{
    int ch;
    char    *passphrase;

    setprogname(argv[0]);

    while ((ch = getopt(argc, argv, "dehp:")) != -1) {
        switch(ch) {
            case 'd':
                flag_enc = 0;
                break;
            case 'e':
                flag_enc = 1;
                break;
            case 'h':
                flag_print = 1;
                break;
            case 'p':
                pass_file_path = optarg;
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (flag_print) {
        usage();
        exit(EXIT_SUCCESS);
    }

    if (flag_enc == -1) {
        usage();
        exit(EXIT_FAILURE);
    }

    passphrase = get_passphrase();
    do_crypt(stdin, stdout, flag_enc, passphrase);
    free(passphrase);

    exit(EXIT_SUCCESS);
}

/*
 * refer to http://linux.die.net/man/3/evp_cipherupdate
 */
static void
do_crypt(FILE *in, FILE *out, int enc, char *passphrase)
{
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char salt[SALT_SIZE];
    EVP_CIPHER_CTX ctx;
    unsigned char *inbuf, *outbuf;
    int inlen, outlen;
    size_t block_size;

    /* generate key, iv and salt */
    memset(key, 0, KEY_SIZE);
    memset(iv, 0, IV_SIZE);
    memset(salt, 0, SALT_SIZE);

    /* for encryption, salt is generated at random, read from the encrypted file when decryption */
    if (enc == 1) {
        if (RAND_bytes(salt, SALT_SIZE) != 1) {
            fprintf(stderr, "salt generation error.\n");
            exit(EXIT_FAILURE);
        }
        fwrite(salt, 1, SALT_SIZE, out);
    }
    else {
        inlen = fread(salt, 1, SALT_SIZE, in);
        if (inlen != SALT_SIZE) {
            fprintf(stderr, "cannot read salt from encrypted file.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, 
                (unsigned char *)passphrase, strlen(passphrase),
                1, key, iv) == 0) {
        fprintf(stderr, "EVP_BytesToKey error.\n");
        exit(EXIT_FAILURE);
    }

    /* init cipher context */
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, EVP_aes_256_cbc(), NULL, NULL, NULL, enc);
    EVP_CIPHER_CTX_set_key_length(&ctx, KEY_SIZE);
    EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, enc);
    
    block_size = EVP_CIPHER_CTX_block_size(&ctx);
    inbuf = (unsigned char *)malloc(BUF_SIZE);
    if (inbuf == NULL) {
        fprintf(stderr, "malloc error.\n");
        exit(EXIT_FAILURE);
    }
    outbuf = (unsigned char *)malloc(BUF_SIZE + block_size);
    if (outbuf == NULL) {
        fprintf(stderr, "malloc error.\n");
        exit(EXIT_FAILURE);
    }

    /* encypt or decrypt */
    while (1) {
        inlen = fread(inbuf, 1, BUF_SIZE, in);
        if (inlen <= 0) {
            break;
        }
        if (EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, inlen) != 1) {
            EVP_CIPHER_CTX_cleanup(&ctx);
            fprintf(stderr, "EVP_CipherUpdate error.\n");
            exit(EXIT_FAILURE);
        }
        fwrite(outbuf, 1, outlen, out);
    }

    if (EVP_CipherFinal_ex(&ctx, outbuf, &outlen) != 1) {
        EVP_CIPHER_CTX_cleanup(&ctx);
        fprintf(stderr, "EVP_CipherFinal_ex error.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_cleanup(&ctx);
    free(inbuf);
    free(outbuf);
}

static char *
get_passphrase(void)
{
    char        *passphrase, *pass_env;
    FILE        *pass_file;
    ssize_t     nread;
    size_t      pass_buf_size;

    pass_buf_size = BUF_SIZE;
    passphrase = malloc(sizeof(char *) * pass_buf_size);

    if (passphrase == NULL) {
        fprintf(stderr, "malloc error.\n");
        exit(EXIT_FAILURE);
    }

    if (pass_file_path != NULL) {
        pass_file = fopen(pass_file_path, "r");
        if (pass_file == NULL) {
            perror("fopen error");
            exit(EXIT_FAILURE);
        }
        nread = getline(&passphrase, &pass_buf_size, pass_file);

        if (nread == -1) {
            /* read error */
            if (ferror(pass_file) != 0) {
                perror("getline error");
                exit(EXIT_FAILURE);
            }
            /* end of file */
            else {
                fprintf(stderr, "the content of passin is empty.\n");
                exit(EXIT_FAILURE);
            }
        }
        fclose(pass_file);
    }
    else {
        pass_env = getenv("AED_PASS");
        if (pass_env == NULL) {
            fprintf(stderr, "input the passphrase through -p or AED_PASS environment variable.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(passphrase, pass_env);
    }

    return passphrase;
}

static void
usage(void)
{
    printf("Usage aed [-deh] -p passin\n \
    -d Perform decryption of the input stream.\n \
    -e Perform encryption of the input stream.\n \
    -h Print a short usage and exit.\n \
    -p passin \n \
        Use the first line of the file 'passin' \n \
        as the passphrase from which to derive the key material.\n \
        If this is not specified, use the value of the AED_PASS\n \
        environment variable.\n");
}
