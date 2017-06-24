#ifndef __TIPPSE_MISC__
#define __TIPPSE_MISC__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

char** merge_sort(char** sort1, char** sort2, size_t count);

char* strip_file_name(const char* file);
char* combine_path_file(const char* path, const char* file);
char* correct_path(const char* path);
char* relativate_path(const char* base, const char* path);
int is_directory(const char* path);

#endif /* #ifndef __TIPPSE_MISC__ */
