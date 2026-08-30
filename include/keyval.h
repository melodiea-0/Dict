#ifndef KEYVAL_H
#define KEYVAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define keyval_create(name, ...)                                        \
  TypeFuncs deliberately_strange_name_##name = typefuncs_chars();       \
  KeyVal name = (KeyVal){.key = NULL,                                   \
                         .value = NULL,                                 \
                         .key_funcs = deliberately_strange_name_##name, \
                         .val_funcs = deliberately_strange_name_##name, \
                         __VA_ARGS__};

#define keyval_copy(name, keyval)             \
    KeyVal name = keyval_copy_f(&keyval); 

#define KeyValCheckError(assertion, error_action, code) \
  if (assertion) {                                      \
    fprintf(stderr, "%s\n", keyval_error_string(code)); \
    error_action                                        \
  }

typedef enum {
  KEYVAL_OK = 0,
  KEYVAL_ERROR_NULL_ARGUMENT,
  KEYVAL_ERROR_INDEX_OUT_OF_RANGE,
  KEYVAL_ERROR_EMPTY,
  KEYVAL_ERROR_MEMORY,
  KEYVAL_ERROR_NULL_FUNCTION
} KeyValError;

typedef struct TypeFuncs {
  void* (*create)(void*);
  void* (*free)(void*);
  void* (*print)(void*);
} TypeFuncs;

typedef struct KeyVal {
  void* key;
  void* value;
  struct TypeFuncs key_funcs;
  struct TypeFuncs val_funcs;
} KeyVal;

// внешний интерфейс

void* keyval_fill(KeyVal* kv, void* key, void* value);
void* keyval_free(KeyVal* kv);
void* keyval_print(KeyVal* kv);
void* keyval_update(KeyVal* kv, void* new_value);
KeyVal keyval_copy_f(KeyVal* kv);
TypeFuncs typefuncs_chars();
const char* keyval_error_string(KeyValError error);

#endif  // KEYVAL_H