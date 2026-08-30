#ifndef LIST_H
#define LIST_H
#include <stddef.h>
#include <stopif.h>

#define ListCheckError(assertion, error_action, code)                         \
  if (assertion) {                                                            \
    fprintf(error_log ? error_log : stderr, "%s\n", list_error_string(code)); \
    error_action                                                              \
  }

typedef enum {
    LIST_OK = 0,
    LIST_ERROR_NULL_ARGUMENT,
    LIST_ERROR_INDEX_OUT_OF_RANGE,
    LIST_ERROR_EMPTY
} ListError;

typedef struct ListNode {
  struct ListNode* prev;
  struct ListNode* next;
} ListNode;

typedef struct List {
  size_t size;
  ListNode* head;
  ListNode* tail;
  ListError error;
} List;

List list_create(); // создаёт пустой лист
ListNode list_create_node(); //  создаёт пустую ноду
ListNode* list_get(List* list, size_t index); // отдаёт ноду с индексом от 0-size
List* list_append(List* list, ListNode* node); //  добавляет ноду в конец 
List* list_append_head(List* list, ListNode* node); // добавляет ноду в начало
List* list_append_index(List* list, ListNode* node, size_t index); // добавляет ноду на место переданного индекса
ListNode* list_remove_index(List* list, size_t index); // удаляет ноду по индексу и возвращает ссылку на неё (в неё уже не будет указателей на ноды оставшие в листе)
ListNode* list_pop(List* list); // удаляет ноду конца и возвращает ссылку на неё (в неё уже не будет указателей на ноды оставшие в листе)
const char* list_error_string(ListError error); // разворачивает код ошибки в понятную строку

#endif  // LIST_H