#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "server.h"

int main() {

  if (chdir("/home/vladyslavyavorskyi/c_projects/http-project") != 0) {
    perror("chdir failed");
    exit(EXIT_FAILURE);
  }


  server_init();
  return EXIT_SUCCESS;
}
