#ifndef TIPPSE_ENCODING_H
#define TIPPSE_ENCODING_H

#include <stdlib.h>
#include "types.h"

#define ENCODING_CACHE_SIZE 1024

struct encoding_cache_node {
  codepoint_t cp;
  size_t length;
};

struct encoding_cache {
  size_t start;                         // current position in cache
  size_t end;                           // last position in cache
  struct encoding* encoding;            // encoding used for stream
  struct stream* stream;                // input stream
  struct encoding_cache_node nodes[ENCODING_CACHE_SIZE];  // cache with lengths of codepoints
};

struct encoding {
  struct encoding* (*create)(void);
  void (*destroy)(struct encoding*);

  const char* (*name)(void);
  size_t (*character_length)(struct encoding*);
  codepoint_t (*visual)(struct encoding*, codepoint_t);
  codepoint_t (*decode)(struct encoding*, struct stream*, size_t*);
  size_t (*next)(struct encoding*, struct stream*);
  size_t (*encode)(struct encoding*, codepoint_t, uint8_t*, size_t);
};

void encoding_cache_clear(struct encoding_cache* base, struct encoding* encoding, struct stream* stream);
// Skip code points and rebase absolute offset
TIPPSE_INLINE void encoding_cache_advance(struct encoding_cache* base, size_t advance) {
  base->start += advance;
}

// Returned code point from relative offset
TIPPSE_INLINE struct encoding_cache_node encoding_cache_find_codepoint(struct encoding_cache* base, size_t offset) {
  size_t pos = (offset+base->start)&(ENCODING_CACHE_SIZE-1);
  if (offset+base->start>=base->end) {
    base->nodes[pos].cp = (*base->encoding->decode)(base->encoding, base->stream, &base->nodes[pos].length);
    base->end++;
  }

  return base->nodes[pos];
}

void encoding_init(void);
void encoding_free(void);
struct encoding* encoding_native_static();
struct encoding* encoding_utf8_static();

uint16_t* encoding_reverse_table(uint16_t* table, size_t length, size_t max);

struct range_tree_node* encoding_transform_stream(struct stream* stream, struct encoding* from, struct encoding* to);
uint8_t* encoding_transform_plain(const uint8_t* buffer, size_t length, struct encoding* from, struct encoding* to);

size_t encoding_strnlen(struct encoding* base, struct stream* stream, size_t size);
size_t encoding_strnlen_based(struct encoding* base, size_t (*next)(struct encoding*, struct stream*), struct stream* stream, size_t size);
size_t encoding_strlen(struct encoding* base, struct stream* stream);
size_t encoding_strlen_based(struct encoding* base, codepoint_t (*decode)(struct encoding*, struct stream*, size_t*), struct stream* stream);
size_t encoding_seek(struct encoding* base, struct stream* stream, size_t pos);
size_t encoding_seek_based(struct encoding* base, size_t (*next)(struct encoding*, struct stream*), struct stream* stream, size_t pos);

#include "stream.h"
#include "unicode.h"

// Construct visual characters
TIPPSE_INLINE void encoding_visuals(struct encoding* base, const struct unicode_sequence* in, struct unicode_sequence* out) {
  out->length = in->length;
  for (size_t n = 0; n<in->length; n++) {
    out->cp[n] = (base->visual)(base, in->cp[n]);
  }
}

#endif  /* #ifndef TIPPSE_ENCODING_H */
