#ifndef CONFIG_H
#define CONFIG_H

#define PORT 9999
#define MAX_CONNECTIONS 10
#define PUBLIC_DIR "public"
#define INDEX_FILE "index.html"

#define RESPONSE \
  "HTTP/1.1 200 OK\r\n" \
  "Content-Type: text/html\r\n" \
  "Content-Length: 11\r\n" \
  "\r\n" \
  "Hello World"

#endif
