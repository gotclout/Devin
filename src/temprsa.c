#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string.h>

/**
 * Decodes rf with private key and writes output to wf
 *
 * @param const char* rf, is the read file for decoding
 * @param const char* wf, is the write file for deconding
 * @param RSA** key, is the private key for decrypting
 */
int RsaDec(const char* rf, const char* wf, RSA** key)
{
  int r   = 0,               //return code
      v   = 0,               //decrypt value
      len = 0;               //length read/write
  unsigned char* rb;         //read buf
  unsigned char* wb;         //write buf
  FILE *i = fopen(rf, "rb"), //read file
       *o = fopen(wf, "wb"); //write file

  if(i && o)
  {
    fseek(i, 0, SEEK_END);
    len = ftell(i);
    fseek(i, 0, SEEK_SET);
    rb = (unsigned char*) malloc(256);
    wb = (unsigned char*) malloc(256);
    printf("RSA decrypting in file %s to out file %s\n", rf, wf);
    while(feof(i) == 0)
    {
      memset(rb, 0, 256);
      memset(wb, 0, 256);
      len = fread(rb, 1, 256, i);
      v   = RSA_private_decrypt(len, rb, wb, *key, RSA_PKCS1_PADDING);
      if(v != -1) fwrite(wb, 1, v, o);
      else if(!feof(i))
      {
        ERR_print_errors_fp(stderr);
        r = 2;
      }
    }
    free(rb);
    free(wb);
    fclose(i);
    fclose(o);
  }
  else
  {
    printf("I/O error\n");
    if(!i && rf) printf("open %s failed\n", rf);
    if(!o && wf) printf("open %s failed\n", wf);
    r = 1;
  }

  return r;
}

/**
 * Encodes file rf with key and writes output to wf
 *
 * @param const char* rf, is the read file for decoding
 * @param const char* wf, is the write file for deconding
 * @param RSA** key, is the private key for decrypting
 */
int RsaEnc(const char* rf, const char* wf, RSA** key)
{
  int r   = 0,               //return code
      v   = 0,               //decrypt value
      len = 0;               //length read/write
  unsigned char* rb;         //read buf
  unsigned char* wb;         //write buf
  FILE *i = fopen(rf, "rb"), //read file
       *o = fopen(wf, "wb"); //write file

  if(i && o)
  {
    fseek(i, 0, SEEK_END);
    len = ftell(i);
    fseek(i, 0, SEEK_SET);
    rb = (unsigned char*) malloc(256);
    wb = (unsigned char*) malloc(256);
    printf("RSA encrypting in file %s to out file %s\n", rf, wf);
    while(feof(i) == 0)
    {
      memset(rb, 0, 256);
      memset(wb, 0, 256);
      len = fread(rb, 1, 128, i);
      v = RSA_public_encrypt(len, rb, wb, *key, RSA_PKCS1_PADDING);
      if(v != -1) fwrite(wb, 1, 256, o);
      else if(!feof(i))
      {
        ERR_print_errors_fp(stderr);
        r = 2;
      }
    }

    free(rb);
    free(wb);
    fclose(i);
    fclose(o);
  }
  else
  {
    printf("I/O error\n");
    if(!i && rf) printf("open %s failed\n", rf);
    if(!o && wf) printf("open %s failed\n", wf);
    r = 1;
  }

  return r;
}

/**
 * Encodes and decodes text file using RSA pki where
 *
 * @param int argc is the number of arguments and
 * @param argv contains infile argv[1] outfile argv[2]
 */
int main(int argc, char** argv)
{
  RSA *key = 0;
  int    r = 0;

  static const char rnd_seed[] =
    "string to make the random number generator think it has entropy";

  if(argc == 3)
  {
    RAND_seed(rnd_seed, sizeof rnd_seed);

    if( (key = RSA_generate_key(2048,3,NULL,NULL)) == NULL)
    {
      printf("\nerror generating key\n");
    }
    else
    {
      if(RsaEnc(argv[1], argv[2], &key))
      {
        printf("RsaEnc error\n");
      }
      else
      {
        char decout[strlen(argv[2]) + 3];
        memset(decout, 0, strlen(argv[2]) + 3);
        strcat(decout, argv[2]);
        strcat(decout, ".d");
        if(RsaDec(argv[2], decout, &key))
        {
          printf("RsaDec error\n");
        }
      }
      RSA_free(key);
    }
  }
  else
  {
    printf("Usage: prog name, encode file name, outfile name\n");
  }

  return 0;
}

