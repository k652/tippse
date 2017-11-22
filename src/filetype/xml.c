// Tippse - File type - XML

#include "xml.h"

struct file_type* file_type_xml_create(struct config* config) {
  struct file_type_xml* this = malloc(sizeof(struct file_type_xml));
  this->vtbl.create = file_type_xml_create;
  this->vtbl.destroy = file_type_xml_destroy;
  this->vtbl.name = file_type_xml_name;
  this->vtbl.mark = file_type_xml_mark;
  this->vtbl.bracket_match = file_type_bracket_match;
  return (struct file_type*)this;
}

void file_type_xml_destroy(struct file_type* base) {
  struct file_type_xml* this = (struct file_type_xml*)base;
  free(this);
}

const char* file_type_xml_name(void) {
  return "XML";
}

void file_type_xml_mark(struct file_type* base, int* visual_detail, struct encoding_cache* cache, int same_line, int* length, int* flags) {
  codepoint_t cp1 = encoding_cache_find_codepoint(cache, 0);
  codepoint_t cp2 = encoding_cache_find_codepoint(cache, 1);

  *length = 1;
  int before = *visual_detail;
  if (before&VISUAL_DETAIL_NEWLINE) {
    before &= ~(VISUAL_DETAIL_STRING0|VISUAL_DETAIL_STRING1);
  }

  int before_masked = before&VISUAL_DETAIL_STATEMASK;
  int after = before&~(VISUAL_DETAIL_INDENTATION|VISUAL_DETAIL_WORD);

  if (before_masked&VISUAL_DETAIL_STRINGESCAPE) {
    after &= ~VISUAL_DETAIL_STRINGESCAPE;
  } else {
    if (cp1=='<') {
      if (before_masked==0) {
        after |= VISUAL_DETAIL_COMMENT2;
      }
    } else if (cp1=='>') {
      if (before_masked==VISUAL_DETAIL_COMMENT2) {
        after &= ~VISUAL_DETAIL_COMMENT2;
      }
    } else if (cp1=='-') {
      if (cp2=='-') {
        if (before_masked==VISUAL_DETAIL_COMMENT2) {
          *length = 2;
          after |= VISUAL_DETAIL_COMMENT0;
        } else if (before_masked==(VISUAL_DETAIL_COMMENT2|VISUAL_DETAIL_COMMENT0)) {
          *length = 2;
          after &= ~VISUAL_DETAIL_COMMENT0;
        }
      }
    } else if (cp1=='"') {
      if (before_masked&VISUAL_DETAIL_STRING0) {
        after &= ~VISUAL_DETAIL_STRING0;
      } else if (before_masked==VISUAL_DETAIL_COMMENT2) {
        after |= VISUAL_DETAIL_STRING0;
      }
    } else if (cp1=='\'') {
      if (before_masked&VISUAL_DETAIL_STRING1) {
        after &= ~VISUAL_DETAIL_STRING1;
      } else if (before_masked==VISUAL_DETAIL_COMMENT2) {
        after |= VISUAL_DETAIL_STRING1;
      }
    }
  }

  if (cp1=='\t' || cp1==' ') {
    after |= VISUAL_DETAIL_INDENTATION;
  }

  if ((cp1>='a' && cp1<='z') || (cp1>='A' && cp1<='Z') || (cp1>='0' && cp1<='9') || cp1=='_') {
    after |= VISUAL_DETAIL_WORD;
  }

  if ((before|after)&(VISUAL_DETAIL_STRING0|VISUAL_DETAIL_STRING1)) {
    *flags = VISUAL_FLAG_COLOR_STRING;
  } else if ((before|after)&(VISUAL_DETAIL_COMMENT0)) {
    *flags = VISUAL_FLAG_COLOR_BLOCKCOMMENT;
  } else if ((before|after)&(VISUAL_DETAIL_COMMENT2)) {
    *flags = VISUAL_FLAG_COLOR_TYPE;
  } else if ((after)&(VISUAL_DETAIL_INDENTATION)) {
    *length = 0;
    *flags = 0;
  } else {
    if (!(before&VISUAL_DETAIL_WORD) && (after&VISUAL_DETAIL_WORD)) {
      *length = 0;
      *flags = 0;
    }

    if (*flags==0) {
      *length = 0;
    }
  }

  *visual_detail = after;
}
