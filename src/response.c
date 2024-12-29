#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "response.h"

char *get_mime_type(char *path) {
  const char *ext = strrchr(path, '.');
  if (ext) {
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".pdf") == 0) return "application/pdf";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";
  }
  return "application/octet-stream"; // default MIME type
}

void send_notfound_response(int client_fd) {
  char *status_line = "HTTP/1.1 404 Not Found\r\n";
  char *content_type = "Content-Type: text/html\r\n";
  char *length = "Content-Length: 40\r\n";
  char *body = "<html><body><h1>404 Not Found</h1></body></html>";
  char *end_of_headers = "\r\n";

  send(client_fd, status_line, strlen(status_line), 0);
  send(client_fd, content_type, strlen(content_type), 0);
  send(client_fd, length, strlen(length), 0);
  send(client_fd, end_of_headers, strlen(end_of_headers), 0);
  send(client_fd, body, strlen(body), 0);
  
  close(client_fd);
  return;
}

void send_response(int client_fd, char *path) {

  printf("path: %s\n", path);
  int fd;
  if ((fd = open(path, O_RDONLY)) < 0) {
    send_notfound_response(client_fd);
    perror("open failed");
    return;
  }

  printf("file opened\n");
  generate_headers(client_fd, path);

  printf("sending file\n"); 
  char buffer[1024];
  int n;

  while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
    if (send(client_fd, buffer, n, 0) < 0) {
      perror("send failed");
      return;
    }
  }
  printf("file sent\n");
  close(fd);
}


void generate_headers(int client_fd, char *path) {
  char *status_line = "HTTP/1.1 200 OK\r\n";
  char *content_type = get_mime_type(path); 

  char content_type_header[128];
  sprintf(content_type_header, "Content-Type: %s\r\n", content_type);

  char *content_length = "Content-Length: ";
  struct stat st;
  stat(path, &st);
  char content_length_header[128];
  sprintf(content_length_header, "%s%ld\r\n", content_length, st.st_size);
  
  char *connection = "Connection: close\r\n";
  char *end_of_headers = "\r\n";
  ssize_t sent;

  sent = send(client_fd, status_line, strlen(status_line), 0);
  if (sent < 0) {
      perror("send status line failed");
      return;
  }

  sent = send(client_fd, content_type_header, strlen(content_type_header), 0);
  if (sent < 0) {
      perror("send content type header failed");
      return;
  }

  sent = send(client_fd, content_length_header, strlen(content_length_header), 0);
  if (sent < 0) {
      perror("send content length header failed");
      return;
  }


  sent = send(client_fd, connection, strlen(connection), 0);
  if (sent < 0) {
      perror("send connection header failed");
      return;
  }

  sent = send(client_fd, end_of_headers, strlen(end_of_headers), 0);
  if (sent < 0) {
      perror("send end of headers failed");
      return;
  }

  printf("Headers sent successfully.\n");
}
