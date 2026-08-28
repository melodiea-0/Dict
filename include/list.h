#ifndef LIST_H
#define LIST_H
#include <stddef.h>
#include <stopif.h>

typedef struct ListNode {
  struct ListNode* prev;
  struct ListNode* next;
} ListNode;

typedef struct List {
  size_t size;
  ListNode* head;
  ListNode* tail;
  char error;
} List;

List list_create(); // создаёт пустой лист
ListNode list_create_node(); //  создаёт пустую ноду
ListNode* list_get(List* list, size_t index); // отдаёт ноду с индексом от 0-size
void list_append(List* list, ListNode* node); //  добавляет ноду в конец
void list_append_head(List* list, ListNode* node); // добавляет ноду в начало
void list_append_index(List* list, ListNode* node, size_t index); // добавляет ноду на место переданного индекса
ListNode* list_remove_index(List* list, size_t index); // удаляет ноду по индексу и возвращает ссылку на неё (в неё уже не будет указателей на ноды оставшие в листе)
ListNode* list_pop(List* list); // удаляет ноду конца и возвращает ссылку на неё (в неё уже не будет указателей на ноды оставшие в листе)

#endif  // LIST_H