#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/// Struct for which this file offers functionality in order to expand the array
/// and set / get its content.
typedef struct DynamicArray {
	uint8_t itemSize;
	uint32_t index;
	uint32_t allocated;
	char* content;
} DynamicArray;

#define EXPAND_STEP 15

/// Default initializers for the dynamic array.
#define CHAR_ARRAY  {1, 0, 0, NULL}
#define WORD_ARRAY  {2, 0, 0, NULL}
#define DWORD_ARRAY {4, 0, 0, NULL}
#define QWORD_ARRAY {8, 0, 0, NULL}
/// (Wasteful) utf-8 array, that always used 4 bytes in order to display a character,
/// even if the space is not required.
#define UTF8_ARRAY  DWORD_ARRAY


inline char*
gnext(DynamicArray *s) { return &s->content[s->index+=s->itemSize]; }

inline char*
get(DynamicArray const * s) { return &s->content[s->index]; }

inline char*
view(DynamicArray const * s, uint32_t i) {
	return s->content + i*s->itemSize;
}

inline char *
viewEnd(DynamicArray const *s, uint32_t i) {
	return s->content + s->index - (i + 1) * s->itemSize;
}

inline void
set(DynamicArray* s, char const *vals, uint8_t amount) {
	assert(amount <= s->itemSize);
	memcpy(s->content + s->index, vals, amount);
}

inline void
snext(DynamicArray* s, char const *vals, uint8_t amount) {
	set(s, vals, amount);
	s->index+=s->itemSize;
}

inline void
empty(DynamicArray* s) { s->index = 0; }

inline bool
isEmpty(DynamicArray* s) { return s->index == 0; }

inline uint32_t
size(DynamicArray const * s) { return s->index / s->itemSize; }

inline void
pop(DynamicArray* s) { s->index -= s->itemSize; }

inline void checkSetNext(DynamicArray *s, char const *c, uint8_t amount) {
	if (s->index + s->itemSize >= s->allocated) {
		if ((s->content = (char *)realloc(
						s->content, s->allocated += EXPAND_STEP * s->itemSize)) == NULL) {
			exit(1);
		};
	}
	if (amount) { snext(s, c, amount); }
}

char *checkGetNext(DynamicArray *s) {
	if (s->index + s->itemSize >= s->allocated) {
		if ((s->content = (char *)realloc(
						s->content, s->allocated += EXPAND_STEP * s->itemSize)) == NULL) {
			exit(1);
		};
	}
	s->index+=s->itemSize;
	return viewEnd(s, 0);
}

#define append(s, c) checkSetNext((s), (char const *) (c), (s)->itemSize)
#define appendPartial(s, c, i) checkSetNext((s), (char const *) (c), (i))
