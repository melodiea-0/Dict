#include <check.h>
#include <stdlib.h>

#include "list.h"

char error_mode = 'r';
FILE* error_log = NULL;

/* ============================================================
 * list_create
 * ============================================================ */

START_TEST(test_list_create_empty)
{
    List list = list_create();

    ck_assert_uint_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);
}
END_TEST


/* ============================================================
 * list_create_node
 * ============================================================ */

START_TEST(test_list_create_node_empty)
{
    ListNode node = list_create_node();

    ck_assert_ptr_null(node.prev);
    ck_assert_ptr_null(node.next);
}
END_TEST


/* ============================================================
 * list_append
 * ============================================================ */

START_TEST(test_list_append_to_empty)
{
    List list = list_create();
    ListNode node = list_create_node();

    list_append(&list, &node);

    ck_assert_uint_eq(list.size, 1);
    ck_assert_ptr_eq(list.head, &node);
    ck_assert_ptr_eq(list.tail, &node);

    ck_assert_ptr_null(node.prev);
    ck_assert_ptr_null(node.next);
}
END_TEST


START_TEST(test_list_append_to_nonempty)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);

    ck_assert_uint_eq(list.size, 2);

    ck_assert_ptr_eq(list.head, &first);
    ck_assert_ptr_eq(list.tail, &second);

    ck_assert_ptr_null(first.prev);
    ck_assert_ptr_eq(first.next, &second);

    ck_assert_ptr_eq(second.prev, &first);
    ck_assert_ptr_null(second.next);
}
END_TEST


/* ============================================================
 * list_append_head
 * ============================================================ */

START_TEST(test_list_append_head_to_empty)
{
    List list = list_create();
    ListNode node = list_create_node();

    list_append_head(&list, &node);

    ck_assert_uint_eq(list.size, 1);
    ck_assert_ptr_eq(list.head, &node);
    ck_assert_ptr_eq(list.tail, &node);

    ck_assert_ptr_null(node.prev);
    ck_assert_ptr_null(node.next);
}
END_TEST


START_TEST(test_list_append_head_to_nonempty)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();

    list_append(&list, &first);
    list_append_head(&list, &second);

    ck_assert_uint_eq(list.size, 2);

    ck_assert_ptr_eq(list.head, &second);
    ck_assert_ptr_eq(list.tail, &first);

    ck_assert_ptr_null(second.prev);
    ck_assert_ptr_eq(second.next, &first);

    ck_assert_ptr_eq(first.prev, &second);
    ck_assert_ptr_null(first.next);
}
END_TEST


/* ============================================================
 * list_get
 * ============================================================ */

START_TEST(test_list_get_first)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);

    ck_assert_ptr_eq(list_get(&list, 0), &first);
}
END_TEST


START_TEST(test_list_get_middle)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);
    list_append(&list, &third);

    ck_assert_ptr_eq(list_get(&list, 1), &second);
}
END_TEST


START_TEST(test_list_get_last)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);
    list_append(&list, &third);

    ck_assert_ptr_eq(list_get(&list, 2), &third);
}
END_TEST


/* ============================================================
 * list_append_index
 * ============================================================ */

START_TEST(test_list_append_index_to_empty)
{
    List list = list_create();
    ListNode node = list_create_node();

    list_append_index(&list, &node, 0);

    ck_assert_uint_eq(list.size, 1);
    ck_assert_ptr_eq(list.head, &node);
    ck_assert_ptr_eq(list.tail, &node);

    ck_assert_ptr_null(node.prev);
    ck_assert_ptr_null(node.next);
}
END_TEST


START_TEST(test_list_append_index_to_beginning)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &third);

    list_append_index(&list, &second, 0);

    ck_assert_uint_eq(list.size, 3);

    ck_assert_ptr_eq(list.head, &second);
    ck_assert_ptr_eq(list.tail, &third);

    ck_assert_ptr_eq(second.next, &first);
    ck_assert_ptr_eq(first.prev, &second);
    ck_assert_ptr_eq(first.next, &third);
    ck_assert_ptr_eq(third.prev, &first);
    ck_assert_ptr_null(second.prev);
    ck_assert_ptr_null(third.next);
}
END_TEST


START_TEST(test_list_append_index_to_middle)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);

    list_append_index(&list, &third, 1);

    ck_assert_uint_eq(list.size, 3);

    ck_assert_ptr_eq(list.head, &first);
    ck_assert_ptr_eq(list.tail, &second);

    ck_assert_ptr_eq(first.next, &third);
    ck_assert_ptr_eq(third.prev, &first);
    ck_assert_ptr_eq(third.next, &second);
    ck_assert_ptr_eq(second.prev, &third);
}
END_TEST


START_TEST(test_list_append_index_out_of_bounds)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();

    list_append(&list, &first);

    list_append_index(&list, &second, 1);

    /* здесь проверяем твой контракт ошибки */
}
END_TEST


/* ============================================================
 * list_remove_index
 * ============================================================ */

START_TEST(test_list_remove_only_node)
{
    List list = list_create();
    ListNode node = list_create_node();

    list_append(&list, &node);

    ListNode* removed = list_remove_index(&list, 0);

    ck_assert_ptr_eq(removed, &node);

    ck_assert_uint_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);

    ck_assert_ptr_null(node.prev);
    ck_assert_ptr_null(node.next);
}
END_TEST


START_TEST(test_list_remove_head)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);
    list_append(&list, &third);

    ListNode* removed = list_remove_index(&list, 0);

    ck_assert_ptr_eq(removed, &first);

    ck_assert_uint_eq(list.size, 2);
    ck_assert_ptr_eq(list.head, &second);
    ck_assert_ptr_eq(list.tail, &third);

    ck_assert_ptr_null(first.prev);
    ck_assert_ptr_null(first.next);

    ck_assert_ptr_null(second.prev);
    ck_assert_ptr_eq(second.next, &third);

    ck_assert_ptr_eq(third.prev, &second);
    ck_assert_ptr_null(third.next);
}
END_TEST


START_TEST(test_list_remove_middle)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);
    list_append(&list, &third);

    ListNode* removed = list_remove_index(&list, 1);

    ck_assert_ptr_eq(removed, &second);

    ck_assert_uint_eq(list.size, 2);
    ck_assert_ptr_eq(list.head, &first);
    ck_assert_ptr_eq(list.tail, &third);

    ck_assert_ptr_null(second.prev);
    ck_assert_ptr_null(second.next);

    ck_assert_ptr_eq(first.next, &third);
    ck_assert_ptr_eq(third.prev, &first);
}
END_TEST


START_TEST(test_list_remove_tail)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);
    list_append(&list, &third);

    ListNode* removed = list_remove_index(&list, 2);

    ck_assert_ptr_eq(removed, &third);

    ck_assert_uint_eq(list.size, 2);
    ck_assert_ptr_eq(list.head, &first);
    ck_assert_ptr_eq(list.tail, &second);

    ck_assert_ptr_null(third.prev);
    ck_assert_ptr_null(third.next);

    ck_assert_ptr_eq(first.next, &second);
    ck_assert_ptr_eq(second.prev, &first);
    ck_assert_ptr_null(second.next);
}
END_TEST


/* ============================================================
 * list_pop
 * ============================================================ */

START_TEST(test_list_pop_only_node)
{
    List list = list_create();
    ListNode node = list_create_node();

    list_append(&list, &node);

    ListNode* removed = list_pop(&list);

    ck_assert_ptr_eq(removed, &node);

    ck_assert_uint_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);

    ck_assert_ptr_null(node.prev);
    ck_assert_ptr_null(node.next);
}
END_TEST


START_TEST(test_list_pop_from_nonempty)
{
    List list = list_create();

    ListNode first = list_create_node();
    ListNode second = list_create_node();
    ListNode third = list_create_node();

    list_append(&list, &first);
    list_append(&list, &second);
    list_append(&list, &third);

    ListNode* removed = list_pop(&list);

    ck_assert_ptr_eq(removed, &third);

    ck_assert_uint_eq(list.size, 2);
    ck_assert_ptr_eq(list.head, &first);
    ck_assert_ptr_eq(list.tail, &second);

    ck_assert_ptr_null(third.prev);
    ck_assert_ptr_null(third.next);

    ck_assert_ptr_eq(first.next, &second);
    ck_assert_ptr_eq(second.prev, &first);
    ck_assert_ptr_null(second.next);
}
END_TEST


/* ============================================================
 * Suite
 * ============================================================ */

Suite* list_suite(void)
{
    Suite* suite = suite_create("List");

    TCase* tc_create = tcase_create("Create");
    tcase_add_test(tc_create, test_list_create_empty);
    tcase_add_test(tc_create, test_list_create_node_empty);

    TCase* tc_append = tcase_create("Append");
    tcase_add_test(tc_append, test_list_append_to_empty);
    tcase_add_test(tc_append, test_list_append_to_nonempty);

    TCase* tc_head = tcase_create("Append head");
    tcase_add_test(tc_head, test_list_append_head_to_empty);
    tcase_add_test(tc_head, test_list_append_head_to_nonempty);

    TCase* tc_get = tcase_create("Get");
    tcase_add_test(tc_get, test_list_get_first);
    tcase_add_test(tc_get, test_list_get_middle);
    tcase_add_test(tc_get, test_list_get_last);

    TCase* tc_index = tcase_create("Append index");
    tcase_add_test(tc_index, test_list_append_index_to_empty);
    tcase_add_test(tc_index, test_list_append_index_to_beginning);
    tcase_add_test(tc_index, test_list_append_index_to_middle);
    tcase_add_test(tc_index, test_list_append_index_out_of_bounds);

    TCase* tc_remove = tcase_create("Remove");
    tcase_add_test(tc_remove, test_list_remove_only_node);
    tcase_add_test(tc_remove, test_list_remove_head);
    tcase_add_test(tc_remove, test_list_remove_middle);
    tcase_add_test(tc_remove, test_list_remove_tail);

    TCase* tc_pop = tcase_create("Pop");
    tcase_add_test(tc_pop, test_list_pop_only_node);
    tcase_add_test(tc_pop, test_list_pop_from_nonempty);

    suite_add_tcase(suite, tc_create);
    suite_add_tcase(suite, tc_append);
    suite_add_tcase(suite, tc_head);
    suite_add_tcase(suite, tc_get);
    suite_add_tcase(suite, tc_index);
    suite_add_tcase(suite, tc_remove);
    suite_add_tcase(suite, tc_pop);

    return suite;
}


int main(void)
{
    Suite* suite = list_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);

    int failed = srunner_ntests_failed(runner);

    srunner_free(runner);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}


// #include <list.h>
// #include <stdint.h>

// void list_print(List list) {
//   Stopif(!list.head, return;, "=== empty list ===");
//   ListNode* node = list.head;
//   do {
//     printf("%04lx <- %04lx -> %04lx\n",
//        (unsigned long)((uintptr_t)node->prev & 0xffff),
//        (unsigned long)((uintptr_t)node & 0xffff),
//        (unsigned long)((uintptr_t)node->next  & 0xffff));
//     node = node->next;
//   } while (node != NULL);
// }

// int main() {
//   List list;
//   list = list_create();
//   ListNode node1 = list_create_node();
//   ListNode node2 = list_create_node();
//   ListNode node3 = list_create_node();
//   list_append(&list, &node1);
//   list_append(&list, &node2);
//   list_append(&list, &node3);
//   list_print(list);
//   printf("\n");
//   ListNode node4 = list_create_node();
//   list_append_head(&list, &node4);
//   list_print(list);
//   for (size_t i = 0; i < 6; ++i) printf("node been found: %p\n", (void*)list_get(&list, i));
//   ListNode node5 = list_create_node();
//   ListNode node6 = list_create_node();
//   list_append_index(&list, &node5, 2);
//   list_append_index(&list, &node6, 2);
//   list_print(list);
//   printf("\n");
//   ListNode node7 = list_create_node();
//   ListNode node8 = list_create_node();
//   list_append_index(&list, &node7, 0);
//   list_append_index(&list, &node8, 6);
//   list_print(list);
//   printf("\n");
//   list_pop(&list);
//   list_pop(&list);
//   list_pop(&list);
//   list_pop(&list);
//   list_pop(&list);
//   list_pop(&list);
//   list_pop(&list);
//   list_pop(&list);
//   list_print(list);
//   printf("\n");
//   list_append(&list, &node1);
//   list_append(&list, &node2);
//   list_append(&list, &node3);
//   list_append_index(&list, &node7, 0);
//   list_append_index(&list, &node8, 2);
//   list_append_index(&list, &node5, 2);
//   list_append_index(&list, &node6, 2);
//   list_append_head(&list, &node4);
//   list_print(list);
//   printf("\n");
//   list_remove_index(&list, 7);
//   list_print(list);
//   printf("===\n");
//   for (int i = 0; i < 7; ++i) {
//     printf("%p\n", list_remove_index(&list, 0));
//   }
//   list_print(list);
//   printf("\n");
//   return 0;
// }