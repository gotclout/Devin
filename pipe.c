#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * Reverse a cstring in place
 */
char** reverseinplace(char** str, int i)
{
  int sz;
  char* c;
  c   = *str;
  sz = strlen(c);

  for(i = 0 ; i < sz / 2 ; ++i)
  {
    c[sz] = c[i];
    c[i] = c[sz - 1 - i];
    c[sz - 1 - i] = c[sz];
  }

  c[sz] = 0;
  
  return str;
}

/**
  * main
  */
int main(int argc, char *argv[])
{
  int pfd[2];   //pipe descriptors
  pid_t cpid; //child id
  char* rev;   //reversed string arg
  int sz;          //string arg len
  int rsz;        //pipe read result
  
  assert(argc == 2);
  
  sz = strlen(argv[1]);
  char buff[sz + 1];
  memset(buff, 0, sz + 1);
  
  if (pipe(pfd) == -1)
  { 
    perror("pipe"); 
    exit(EXIT_FAILURE); 
  }
   
  cpid = fork();  //create child
  if (cpid == -1) 
  { 
    perror("fork"); 
    exit(EXIT_FAILURE); 
  }

  //child proc
  if (cpid == 0)
  {    
    /* Close unused write end */
    close(pfd[1]); 
    
    /* Child reads from pipe */
    rsz = read(pfd[0],buff, sz);             //read
    rev = reverseinplace(&buff, 0);    //reverse
    write(STDOUT_FILENO, rev, sz);//print
    write(STDOUT_FILENO, "\n", 1);
    close(pfd[0]);                                   //close read
    _exit(EXIT_SUCCESS);
  } 
  else
  {        
    /* Parent writes argv[1] to pipe */
    close(pfd[0]);
    
     /* Close unused read end */
    write(pfd[1], argv[1], strlen(argv[1]));
    close(pfd[1]);
    
    /* Reader will see EOF 
     *  Parent waits for child
     */
    wait(NULL);
    exit(EXIT_SUCCESS);
  }
}
