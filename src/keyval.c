#include <keyval.h>

static void* member_create_chars(void* bytes) {
  char* str = (char*)bytes;
  KeyValCheckError(!str, return NULL;, KEYVAL_ERROR_NULL_ARGUMENT);
  char* member = malloc(strlen(str) + 1);
  KeyValCheckError(!member, return NULL;, KEYVAL_ERROR_MEMORY);
  memcpy(member, str, strlen(str) + 1);
  return member;
}

static void* member_free_chars(void* bytes) { 
  free(bytes);
  return NULL;
}

static void* member_print_chars(void* bytes) {
  printf("\"%s\"", (char*)bytes);
  return NULL;
}

TypeFuncs typefuncs_chars() {
  return (TypeFuncs){
            .create = member_create_chars,
            .free = member_free_chars,
            .print = member_print_chars,
          };
}

void* keyval_fill(KeyVal* kv, void* key, void* value) {
  KeyValCheckError(!kv->key_funcs.create, fprintf(stderr, "%s\n", "no func key_create"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  KeyValCheckError(!kv->val_funcs.create, fprintf(stderr, "%s\n", "no func val_create"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  kv->key = kv->key_funcs.create(key);
  kv->value = kv->val_funcs.create(value);
  return NULL;
}

void* keyval_free(KeyVal* kv) {
  KeyValCheckError(!kv->key_funcs.free, fprintf(stderr, "%s\n", "no func key_free"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  KeyValCheckError(!kv->val_funcs.free, fprintf(stderr, "%s\n", "no func val_free"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  kv->key_funcs.free(kv->key);
  kv->val_funcs.free(kv->value);
  return NULL;
}

void* keyval_print(KeyVal* kv) {
  KeyValCheckError(!kv->key_funcs.print, fprintf(stderr, "%s\n", "no func key_print"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  KeyValCheckError(!kv->val_funcs.print, fprintf(stderr, "%s\n", "no func val_print"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  printf("key: ");
  kv->key_funcs.print(kv->key);
  printf(", value: ");
  kv->val_funcs.print(kv->value);
  printf("\n");
  return NULL;
}

void* keyval_update(KeyVal* kv, void* new_value) {
  KeyValCheckError(!kv->val_funcs.free, fprintf(stderr, "%s\n", "no func val_free"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  KeyValCheckError(!kv->val_funcs.create, fprintf(stderr, "%s\n", "no func val_create"); return NULL;, KEYVAL_ERROR_NULL_FUNCTION);
  kv->val_funcs.free(kv->value);
  kv->value = kv->val_funcs.create(new_value);
  return NULL;
}

KeyVal keyval_copy_f(KeyVal* kv) {
  keyval_create(kv_copy, .key_funcs = kv->key_funcs,
                         .val_funcs = kv->val_funcs);
  keyval_fill(&kv_copy, kv->key, kv->value);
  return kv_copy;
}

const char* keyval_error_string(KeyValError error) {
  switch (error) {
    case KEYVAL_OK:
      return "no error";
    case KEYVAL_ERROR_NULL_ARGUMENT:
      return "null argument";
    case KEYVAL_ERROR_INDEX_OUT_OF_RANGE:
      return "index out of range";
    case KEYVAL_ERROR_EMPTY:
      return "keyval is empty";
    case KEYVAL_ERROR_MEMORY:
      return "no memory";
    case KEYVAL_ERROR_NULL_FUNCTION:
      return "null function";
    default:
      return "unknown error";
  }
}
