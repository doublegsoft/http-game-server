#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mongoose.h"

#define MAX_ARG_LEN       256

int main(int argc, char *argv[]) 
{
  
  // parse arguments
  int i = 1;
  char port[MAX_ARG_LEN];
  char root[MAX_ARG_LEN];
  while (i < argc) 
  {
    const char *argval = argv[i];
    if (strcmp("--port", argval) == 0) 
    {
      i++;
      argval = argv[i];
      strcpy(port, argval);
    }
    if (strcmp("--root", argval) == 0) 
    {
      i++;
      argval = argv[i];
      strcpy(root, argval);
    }
    i++;
  }
  
  printf("start server at port: %s\n", port);
  hgs_http_init(port, root);
}
