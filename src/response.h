#ifndef RESPONSE_H
#define RESPONSE_H

char *get_mime_type(char *path);
void send_response(int client_fd, char *path);
void generate_headers(int client_fd, char *path);

#endif // !RESPONSE_H
