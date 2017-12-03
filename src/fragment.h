#ifndef TIPPSE_FRAGMENT_H
#define TIPPSE_FRAGMENT_H

struct fragment;

#define FRAGMENT_MEMORY 0
#define FRAGMENT_FILE 1

#include <stdlib.h>
#include "types.h"
struct file_cache;
struct file_cache_node;

struct fragment {
  int count;
  int type;
  uint8_t* buffer;
  file_offset_t offset;
  struct file_cache* cache;
  struct file_cache_node* cache_node;

  size_t length;
};

#include "filecache.h"
#include "documentfile.h"

struct fragment* fragment_create_memory(uint8_t* buffer, size_t length);
struct fragment* fragment_create_file(struct file_cache* cache, file_offset_t offset, size_t length, struct document_file* file);
void fragment_cache(struct fragment* base);
void fragment_reference(struct fragment* base, struct document_file* file);
void fragment_dereference(struct fragment* base, struct document_file* file);

#endif /* #ifndef TIPPSE_FRAGMENT_H */
