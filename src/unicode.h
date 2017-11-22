#ifndef TIPPSE_UNICODE_H
#define TIPPSE_UNICODE_H

#include <stdlib.h>
#include "types.h"

#define UNICODE_CODEPOINT_MAX 0x110000
#define UNICODE_BITFIELD_MAX ((UNICODE_CODEPOINT_MAX/sizeof(unsigned int))+1)

#include "encoding.h"

void unicode_init(void);
void unicode_decode_rle(unsigned int* table, uint16_t* rle);
void unicode_update_combining_mark(codepoint_t codepoint);
int unicode_combining_mark(codepoint_t codepoint);
size_t unicode_read_combined_sequence(struct encoding_cache* cache, size_t offset, codepoint_t* codepoints, size_t max, size_t* advance, size_t* length);
int unicode_width(codepoint_t* codepoints, size_t max);

#endif /* #ifndef TIPPSE_UNICODE_H */
