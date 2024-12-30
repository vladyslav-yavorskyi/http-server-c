#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../src/response.h"

void test_generate_headers() {
  int pipe_fd[2];
  pipe(pipe_fd);

  generate_headers(pipe_fd[1], "public/index.html");

  char buffer[1024];
  read(pipe_fd[0], buffer, sizeof(buffer));

  assert(strstr(buffer, "HTTP/1.1 200 OK") != NULL);
  assert(strstr(buffer, "Content-Type: text/html") != NULL);
  assert(strstr(buffer, "Content-Length:") != NULL);

  close(pipe_fd[0]);
  close(pipe_fd[1]);

  printf("test_generate_headers passed\n");
}


void test_send_response() {
  int pipe_fd[2];
  pipe(pipe_fd);

  send_response(pipe_fd[1], "public/index.html");
  char buffer[1024];
  read(pipe_fd[0], buffer, sizeof(buffer));

  assert(strstr(buffer, "HTTP/1.1 200 OK") != NULL);

  send_response(pipe_fd[1], "public/404.html");
  read(pipe_fd[0], buffer, sizeof(buffer));

  assert(strstr(buffer, "HTTP/1.1 404 Not Found") != NULL);

  close(pipe_fd[0]);
  close(pipe_fd[1]);

  printf("test_send_response passed\n");
}


int main() {
  test_generate_headers();
  test_send_response();
  printf("All tests passed\n");
  return EXIT_SUCCESS;
}
