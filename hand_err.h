#ifndef HAND_ERR_H

#define HAND_ERR_H

void internal_mem_error(const int line, const char *file, const char *func, const char *err);
void internal_mem_error_size(const int line, const char *file, const char *func, const unsigned long size);
void internal_file_error(const int line, const char *file, const char *func, const char *fn);

#endif
