#include <list.h>

List list_create() {
  return (List){.size = 0, .head = NULL, .tail = NULL, .error='0'};
}

ListNode list_create_node() { return (ListNode){.prev = NULL, .next = NULL}; }

void list_append(List* list, ListNode* node) {
  Stopif(!list || !node, list->error = 'e';, "=== empty list or node ===");
  if (list->tail == NULL) {
    list->tail = list->head = node;
  } else {
    ListNode* old_node = list->tail;
    list->tail = old_node->next = node;
    node->prev = old_node;
    node->next = NULL;
  }
  list->size += 1;
}

void list_append_head(List* list, ListNode* node) {
  Stopif(!list || !node, list->error = 'e';, "=== empty list or node ===");
  if (list->tail == NULL) {
    list->tail = list->head = node;
  } else {
    ListNode* old_node = list->head;
    list->head = old_node->prev = node;
    node->prev = NULL;
    node->next = old_node;
  }
  list->size += 1;
}

void list_append_index(List* list, ListNode* node, size_t index) {
  Stopif(!list, return;, "=== empty list ===");
  Stopif(!node, list->error = 'e';, "=== empty node ===");
  ListNode* old_node = list_get(list, index);
  if (index == 0) { 
    list_append_head(list, node); 
  } else if (old_node != NULL) {
    ListNode* prev = old_node->prev;
    prev->next = old_node->prev = node;
    node->prev = prev;
    node->next = old_node;
    list->size += 1;
  }
}

ListNode* list_get(List *list, size_t index) {
  Stopif(!list, return NULL;, "=== empty list ===");
  Stopif(index >= list->size, list->error = 'e'; return NULL;, "=== incorrect index ===");
  ListNode* node = list->head;
  for (size_t i = 0; i < index; ++i) {
    node = node->next;
  }
  return node;
}

ListNode* list_remove_index(List* list, size_t index) {
  Stopif(!list, return NULL;, "=== empty list ===");
  if (index + 1 == list->size) return list_pop(list);
  ListNode* node = list_get(list, index);
  if (node != NULL) {
    ListNode* prev = node->prev;
    ListNode* next = node->next;
    if (prev == NULL) list->head = next;
    else prev->next = next;
    next->prev = prev;
    node->prev = NULL;
    node->next = NULL;
    list->size -= 1;
  }
  return node;
}

ListNode* list_pop(List* list) {
  Stopif(!list, return NULL;, "=== empty list ===");
  ListNode* node = list_get(list, list->size - 1);
  if (node != NULL) {
    list->size -= 1;
    if (list->size == 0) {
      list->tail = list->head = NULL;
    } else {
      ListNode* prev = node->prev;
      prev->next = NULL;
      list->tail = prev;
    }
    node->prev = node->next = NULL;
  }
  return node;
}
