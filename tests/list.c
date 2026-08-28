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
 * NULL arguments
 * ============================================================ */

START_TEST(test_list_get_null_list)
{
    ListNode* result = list_get(NULL, 0);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_get_null_list_invalid_index)
{
    ListNode* result = list_get(NULL, 100);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_null_list)
{
    ListNode node = list_create_node();

    List* result = list_append(NULL, &node);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_null_list_and_node)
{
    List* result = list_append(NULL, NULL);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_null_node)
{
    List list = list_create();

    List* result = list_append(&list, NULL);

    ck_assert_ptr_eq(result, &list);
    ck_assert_uint_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);
    ck_assert_int_eq(list.error, LIST_ERROR_NULL_ARGUMENT);
}
END_TEST


START_TEST(test_list_append_head_null_list)
{
    ListNode node = list_create_node();

    List* result = list_append_head(NULL, &node);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_head_null_list_and_node)
{
    List* result = list_append_head(NULL, NULL);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_head_null_node)
{
    List list = list_create();

    List* result = list_append_head(&list, NULL);

    ck_assert_ptr_eq(result, &list);
    ck_assert_uint_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);
    ck_assert_int_eq(list.error, LIST_ERROR_NULL_ARGUMENT);
}
END_TEST


START_TEST(test_list_append_index_null_list)
{
    ListNode node = list_create_node();

    List* result = list_append_index(NULL, &node, 0);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_index_null_list_and_node)
{
    List* result = list_append_index(NULL, NULL, 0);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_append_index_null_node)
{
    List list = list_create();

    List* result = list_append_index(&list, NULL, 0);

    ck_assert_ptr_eq(result, &list);
    ck_assert_uint_eq(list.size, 0);
    ck_assert_ptr_null(list.head);
    ck_assert_ptr_null(list.tail);
    ck_assert_int_eq(list.error, LIST_ERROR_NULL_ARGUMENT);
}
END_TEST


START_TEST(test_list_remove_index_null_list)
{
    ListNode* result = list_remove_index(NULL, 0);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_remove_index_null_list_invalid_index)
{
    ListNode* result = list_remove_index(NULL, 100);

    ck_assert_ptr_null(result);
}
END_TEST


START_TEST(test_list_pop_null_list)
{
    ListNode* result = list_pop(NULL);

    ck_assert_ptr_null(result);
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

    TCase* tc_null = tcase_create("NULL arguments");

    tcase_add_test(tc_null, test_list_get_null_list);
    tcase_add_test(tc_null, test_list_get_null_list_invalid_index);

    tcase_add_test(tc_null, test_list_append_null_list);
    tcase_add_test(tc_null, test_list_append_null_list_and_node);
    tcase_add_test(tc_null, test_list_append_null_node);

    tcase_add_test(tc_null, test_list_append_head_null_list);
    tcase_add_test(tc_null, test_list_append_head_null_list_and_node);
    tcase_add_test(tc_null, test_list_append_head_null_node);

    tcase_add_test(tc_null, test_list_append_index_null_list);
    tcase_add_test(tc_null, test_list_append_index_null_list_and_node);
    tcase_add_test(tc_null, test_list_append_index_null_node);

    tcase_add_test(tc_null, test_list_remove_index_null_list);
    tcase_add_test(tc_null, test_list_remove_index_null_list_invalid_index);

    tcase_add_test(tc_null, test_list_pop_null_list);


    suite_add_tcase(suite, tc_create);
    suite_add_tcase(suite, tc_append);
    suite_add_tcase(suite, tc_head);
    suite_add_tcase(suite, tc_get);
    suite_add_tcase(suite, tc_index);
    suite_add_tcase(suite, tc_remove);
    suite_add_tcase(suite, tc_pop);
    suite_add_tcase(suite, tc_null);

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