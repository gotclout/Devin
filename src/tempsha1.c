#include <openssl/sha.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

/**
 * Generates a sha1 digest for
 *
 * @param rf read file with message to be digested
 * @param wf write file with digested message
 * @param c is the context used for generating the digest
 */
int Sha1Digest(const char* rf, const char* wf, SHA_CTX* c)
{
  int   r = 0,
      len = 0;

  unsigned char *wb,
                *rb;

  FILE *i = fopen(rf, "rb"),
       *o = fopen(wf, "wb");

  if(i && o)
  {
    printf("Creating Sha1 Digest for in message %s to out message %s\n", rf, wf);
    fseek(i, 0, SEEK_END);
    len = ftell(i);
    fseek(i, 0, SEEK_SET);
    printf("Length: %i\n", len);
    rb = (unsigned char*) malloc(20);
    wb = (unsigned char*) malloc(20);
    int rbs = 0, wbs = 0, len = 0;

    while(feof(i) == 0)
    {
      memset(rb, 0, 20);
      memset(wb, 0, 20);
      fread(rb, 1, 20, i);
      rbs += strlen(rb);
      SHA1_Update(c, rb, sizeof(rb));
      SHA1_Final(wb, c);
      wbs += fwrite(wb, 1, 20, o);
    }

    free(rb);
    free(wb);
    fclose(i);
    fclose(o);
  }
  else
  {
    printf("I/O error\n");
    r = 1;
  }
  return r;
}

/**
 * Initialize a SHA1 context for encoding message in file
 * argv[1] to output file argv[2]
 */
int main(int argc, char** argv )
{
  SHA_CTX c;
  if(argc == 3)
  {
    SHA1_Init(&c);
    if(Sha1Digest(argv[1], argv[2], &c) != 0)
    {
      printf("digest error\n");
    }
  }
  else
  {
    printf("usage: prog name, message file name, digest file name");
  }

  return 0;
}

