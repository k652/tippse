#ifndef TIPPSE_MISC_H
#define TIPPSE_MISC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include "types.h"
#include "encoding.h"
#include "encoding/native.h"
#include "unicode.h"

struct directory {
  DIR* dir;             // directory stream
  struct dirent* entry; // last read directory entry
};

char** merge_sort(char** sort1, char** sort2, size_t count);

char* extract_file_name(const char* file);
char* strip_file_name(const char* file);
char* combine_string(const char* string1, const char* string2);
char* combine_path_file(const char* path, const char* file);
char* correct_path(const char* path);
char* relativate_path(const char* base, const char* path);
char* home_path(void);
int is_directory(const char* path);

int64_t tick_count(void);
int64_t tick_ms(int64_t ms);

uint64_t decode_based_unsigned_offset(struct encoding_cache* cache, int base, size_t* offset, size_t count);
uint64_t decode_based_unsigned(struct encoding_cache* cache, int base, size_t count);

int64_t decode_based_signed_offset(struct encoding_cache* cache, int base, size_t* offset, size_t count);
int64_t decode_based_signed(struct encoding_cache* cache, int base, size_t count);

struct directory* directory_create(const char* path);
const char* directory_next(struct directory* base);
void directory_destroy(struct directory* base);

#endif /* #ifndef TIPPSE_MISC_H */
