#include "utils.h"
#include <string.h>

void construct_path(const char *request_path, char *result, size_t size) {

  char base_path[] = "public";

  if (strcmp(request_path, "/") == 0) {
    snprintf(result, size, "%s/index.html", base_path);
  } else {
    snprintf(result, size, "%s%s", base_path, request_path);
  }
}
