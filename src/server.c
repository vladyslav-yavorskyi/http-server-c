#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "response.h"
#include "utils.h"
#include "config.h"


void server_init() {
  struct sockaddr_in server_addr;
  int server_fd;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  };

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, MAX_CONNECTIONS) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("==== Server started on port %d ====\n", PORT);
  
  while (1) {
    int client_fd;
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len)) < 0) {
      perror("accept failed");
      exit(EXIT_FAILURE);
    }

    printf("Connection from %s, port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    if (fork() == 0) {
      close(server_fd);
      char buffer[1024];
      int n;

      if ((n = recv(client_fd, buffer, sizeof(buffer), 0)) < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
      }

      buffer[n] = '\0';
      char path[1024], full_path[1024];

      sscanf(buffer, "GET %s HTTP/1.1", path);

      construct_path(path, full_path, sizeof(path)); 

      send_response(client_fd, full_path);
      close(client_fd);
      exit(EXIT_SUCCESS);
    }
  }
}
