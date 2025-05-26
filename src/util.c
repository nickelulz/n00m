#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "log.h"

/*
 * Reads the entire contents of a file into a null-terminated
 * string. Returns NULL on an error, and the result MUST be
 * manually freed by the caller (not memory safe alone).
 */
char *read_file_to_str(const char *filename)
{
  FILE *f = fopen(filename, "r");
  if (!f) {
    log_error("Failed to open file %s: %s",
              filename, strerror(errno));
    return NULL;
  }

  /* seek to the end to determine the file size */
  if (fseek(f, 0, SEEK_END) != 0) {
    log_error("Failed to seek file %s: %s",
              filename, strerror(errorno));
    return NULL;
  }

  uint64_t len = ftell(f);
  if (length < 0) {
    log_error("Failed to tell size of file %s: %s",
              filename, strerror(errno));
    fclose(f);
    return NULL;
  }

  /* rewind file back to the beginning */
  rewind(f);

  /* allocate buffer (+1 for the null terminator) */
  char *buf = malloc(len + 1);
  if (!buf) {
    log_error("Failed to allocate string buffer for file %s: %s",
              filename, strerror(errno));
    fclose(f);
    return NULL;
  }

  size_t read_size = fread(buf, 1, len, f);
  if (read_size != (size_t) len) {
    log_error("Failed to read entirety of file %s: %s",
              filename, strerror(errno));
    free(buf);
    return NULL;
  }

  /* null-terminate the string */
  buf[len] = '\0';
  fclose(f);
  return buf;
}
