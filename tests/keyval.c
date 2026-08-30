#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyval.h"


/* ============================================================
 * Test type: ArrayInt_s
 * ============================================================ */

typedef struct ArrayInt_s {
    int *array;
    size_t size;
} ArrayInt_s;


#define create_ArrayInt_s(...) \
    (ArrayInt_s){.array = NULL, .size = 0, __VA_ARGS__}


void *member_create_ArrayInt_s(void *bytes)
{
    ArrayInt_s *tmp = (ArrayInt_s *)bytes;

    KeyValCheckError(
        !tmp,
        return NULL;,
        KEYVAL_ERROR_NULL_ARGUMENT
    );

    ArrayInt_s *member = malloc(sizeof(*member));

    KeyValCheckError(
        !member,
        return NULL;,
        KEYVAL_ERROR_MEMORY
    );

    member->size = tmp->size;
    member->array = malloc(sizeof(*member->array) * member->size);

    KeyValCheckError(
        member->size != 0 && !member->array,
        free(member); return NULL;,
        KEYVAL_ERROR_MEMORY
    );

    memcpy(
        member->array,
        tmp->array,
        sizeof(*member->array) * member->size
    );

    return member;
}


void *member_free_ArrayInt_s(void *bytes)
{
    ArrayInt_s *tmp = (ArrayInt_s *)bytes;

    KeyValCheckError(
        !tmp,
        return NULL;,
        KEYVAL_ERROR_NULL_ARGUMENT
    );

    free(tmp->array);
    free(tmp);

    return NULL;
}


void *member_print_ArrayInt_s(void *bytes)
{
    ArrayInt_s *tmp = (ArrayInt_s *)bytes;

    if (tmp == NULL) {
        printf("{ NULL }");
        return NULL;
    }

    printf("{ ");

    for (size_t i = 0; i < tmp->size; ++i) {
        if (i != 0)
            printf(", ");

        printf("%d", tmp->array[i]);
    }

    printf(" | size: %zu }", tmp->size);

    return NULL;
}


static TypeFuncs array_int_funcs = {
    .create = member_create_ArrayInt_s,
    .free   = member_free_ArrayInt_s,
    .print  = member_print_ArrayInt_s
};


/* ============================================================
 * Test helpers
 * ============================================================ */

static int print_called = 0;


static void *test_print(void *data)
{
    (void)data;

    ++print_called;

    return NULL;
}


static void* member_create_chars_test(void* bytes) {
  char* str = (char*)bytes;
  KeyValCheckError(!str, return NULL;, KEYVAL_ERROR_NULL_ARGUMENT);
  char* member = malloc(strlen(str) + 1);
  KeyValCheckError(!member, return NULL;, KEYVAL_ERROR_MEMORY);
  memcpy(member, str, strlen(str) + 1);
  return member;
}


static void* member_free_chars_test(void* bytes) { 
  free(bytes);
  return NULL;
}


static TypeFuncs test_funcs = {
    .create = member_create_chars_test,
    .free   = member_free_chars_test,
    .print  = test_print
};


/* ============================================================
 * CREATE
 * ============================================================ */

START_TEST(test_keyval_create_default)
{
    keyval_create(kv);

    ck_assert_ptr_null(kv.key);
    ck_assert_ptr_null(kv.value);

    ck_assert(kv.key_funcs.create != NULL);
    ck_assert(kv.key_funcs.free != NULL);
    ck_assert(kv.key_funcs.print != NULL);

    ck_assert(kv.val_funcs.create != NULL);
    ck_assert(kv.val_funcs.free != NULL);
    ck_assert(kv.val_funcs.print != NULL);
}
END_TEST


START_TEST(test_keyval_create_custom)
{
    keyval_create(
        kv,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    ck_assert_ptr_null(kv.key);
    ck_assert_ptr_null(kv.value);

    ck_assert(kv.key_funcs.create == array_int_funcs.create);
    ck_assert(kv.key_funcs.free == array_int_funcs.free);
    ck_assert(kv.key_funcs.print == array_int_funcs.print);

    ck_assert(kv.val_funcs.create == array_int_funcs.create);
    ck_assert(kv.val_funcs.free == array_int_funcs.free);
    ck_assert(kv.val_funcs.print == array_int_funcs.print);
}
END_TEST


START_TEST(test_keyval_create_custom_key_only)
{
    keyval_create(
        kv,
        .key_funcs = array_int_funcs
    );

    ck_assert(kv.key_funcs.create == array_int_funcs.create);
    ck_assert(kv.key_funcs.free == array_int_funcs.free);
    ck_assert(kv.key_funcs.print == array_int_funcs.print);

    ck_assert(kv.val_funcs.create != NULL);
    ck_assert(kv.val_funcs.free != NULL);
    ck_assert(kv.val_funcs.print != NULL);
}
END_TEST


/* ============================================================
 * FILL
 * ============================================================ */

START_TEST(test_keyval_fill_default)
{
    keyval_create(kv);

    keyval_fill(&kv, "key1", "value1");

    ck_assert_ptr_nonnull(kv.key);
    ck_assert_ptr_nonnull(kv.value);

    ck_assert_str_eq((char *)kv.key, "key1");
    ck_assert_str_eq((char *)kv.value, "value1");

    ck_assert_ptr_ne(kv.key, "key1");
    ck_assert_ptr_ne(kv.value, "value1");

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_fill_custom)
{
    int key_data[] = {1, 2, 3, 4};
    int value_data[] = {10, 20, 30};

    ArrayInt_s key = create_ArrayInt_s(
        .array = key_data,
        .size = 4
    );

    ArrayInt_s value = create_ArrayInt_s(
        .array = value_data,
        .size = 3
    );

    keyval_create(
        kv,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    keyval_fill(&kv, &key, &value);

    ArrayInt_s *created_key = kv.key;
    ArrayInt_s *created_value = kv.value;

    ck_assert_ptr_nonnull(created_key);
    ck_assert_ptr_nonnull(created_value);

    ck_assert_uint_eq(created_key->size, 4);
    ck_assert_uint_eq(created_value->size, 3);

    for (size_t i = 0; i < 4; ++i)
        ck_assert_int_eq(created_key->array[i], key_data[i]);

    for (size_t i = 0; i < 3; ++i)
        ck_assert_int_eq(created_value->array[i], value_data[i]);

    ck_assert_ptr_ne(created_key, &key);
    ck_assert_ptr_ne(created_value, &value);

    ck_assert_ptr_ne(created_key->array, key.array);
    ck_assert_ptr_ne(created_value->array, value.array);

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_fill_null_key)
{
    keyval_create(kv);

    keyval_fill(&kv, NULL, "value");

    ck_assert_ptr_null(kv.key);
    ck_assert_ptr_nonnull(kv.value);

    ck_assert_str_eq((char *)kv.value, "value");

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_fill_null_value)
{
    keyval_create(kv);

    keyval_fill(&kv, "key", NULL);

    ck_assert_ptr_nonnull(kv.key);
    ck_assert_ptr_null(kv.value);

    ck_assert_str_eq((char *)kv.key, "key");

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_fill_both_null)
{
    keyval_create(kv);

    keyval_fill(&kv, NULL, NULL);

    ck_assert_ptr_null(kv.key);
    ck_assert_ptr_null(kv.value);
}
END_TEST


/* ============================================================
 * UPDATE
 * ============================================================ */

START_TEST(test_keyval_update_default)
{
    keyval_create(kv);

    keyval_fill(&kv, "key", "old");

    keyval_update(&kv, "new");

    ck_assert_str_eq((char *)kv.key, "key");
    ck_assert_str_eq((char *)kv.value, "new");

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_update_custom)
{
    int old_data[] = {1, 2, 3};
    int new_data[] = {100, 200, 300, 400};

    ArrayInt_s old_value = create_ArrayInt_s(
        .array = old_data,
        .size = 3
    );

    ArrayInt_s new_value = create_ArrayInt_s(
        .array = new_data,
        .size = 4
    );

    keyval_create(
        kv,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    keyval_fill(&kv, &old_value, &old_value);

    keyval_update(&kv, &new_value);

    ArrayInt_s *result = kv.value;

    ck_assert_uint_eq(result->size, 4);

    for (size_t i = 0; i < 4; ++i)
        ck_assert_int_eq(result->array[i], new_data[i]);

    ck_assert_ptr_ne(result->array, new_value.array);

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_update_preserves_key)
{
    keyval_create(kv);

    keyval_fill(&kv, "key", "old");

    void *old_key = kv.key;

    keyval_update(&kv, "new");

    ck_assert_ptr_eq(kv.key, old_key);
    ck_assert_str_eq((char *)kv.key, "key");
    ck_assert_str_eq((char *)kv.value, "new");

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_update_null)
{
    keyval_create(kv);

    keyval_fill(&kv, "key", "value");

    keyval_update(&kv, NULL);

    ck_assert_str_eq((char *)kv.key, "key");
    ck_assert_ptr_null(kv.value);

    keyval_free(&kv);
}
END_TEST


/* ============================================================
 * COPY
 * ============================================================ */

START_TEST(test_keyval_copy_default)
{
    keyval_create(original);

    keyval_fill(&original, "key", "value");

    keyval_copy(copy, original);

    ck_assert_str_eq((char *)copy.key, "key");
    ck_assert_str_eq((char *)copy.value, "value");

    ck_assert_ptr_ne(copy.key, original.key);
    ck_assert_ptr_ne(copy.value, original.value);

    keyval_free(&original);
    keyval_free(&copy);
}
END_TEST


START_TEST(test_keyval_copy_custom)
{
    int key_data[] = {1, 2, 3};
    int value_data[] = {10, 20, 30, 40};

    ArrayInt_s key = create_ArrayInt_s(
        .array = key_data,
        .size = 3
    );

    ArrayInt_s value = create_ArrayInt_s(
        .array = value_data,
        .size = 4
    );

    keyval_create(
        original,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    keyval_fill(&original, &key, &value);

    keyval_copy(copy, original);

    ArrayInt_s *original_key = original.key;
    ArrayInt_s *original_value = original.value;

    ArrayInt_s *copy_key = copy.key;
    ArrayInt_s *copy_value = copy.value;

    ck_assert_ptr_ne(copy_key, original_key);
    ck_assert_ptr_ne(copy_value, original_value);

    ck_assert_ptr_ne(copy_key->array, original_key->array);
    ck_assert_ptr_ne(copy_value->array, original_value->array);

    ck_assert_uint_eq(copy_key->size, original_key->size);
    ck_assert_uint_eq(copy_value->size, original_value->size);

    for (size_t i = 0; i < copy_key->size; ++i)
        ck_assert_int_eq(
            copy_key->array[i],
            original_key->array[i]
        );

    for (size_t i = 0; i < copy_value->size; ++i)
        ck_assert_int_eq(
            copy_value->array[i],
            original_value->array[i]
        );

    keyval_free(&original);
    keyval_free(&copy);
}
END_TEST


START_TEST(test_keyval_copy_is_independent)
{
    keyval_create(original);

    keyval_fill(&original, "key", "value");

    keyval_copy(copy, original);

    strcpy((char *)original.key, "changed");

    ck_assert_str_eq((char *)original.key, "changed");
    ck_assert_str_eq((char *)copy.key, "key");

    strcpy((char *)copy.value, "changed");

    ck_assert_str_eq((char *)copy.value, "changed");
    ck_assert_str_eq((char *)original.value, "value");

    keyval_free(&original);
    keyval_free(&copy);
}
END_TEST


START_TEST(test_keyval_copy_array_is_independent)
{
    int data[] = {1, 2, 3};

    ArrayInt_s value = create_ArrayInt_s(
        .array = data,
        .size = 3
    );

    keyval_create(
        original,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    keyval_fill(&original, &value, &value);

    keyval_copy(copy, original);

    ArrayInt_s *original_value = original.value;
    ArrayInt_s *copy_value = copy.value;

    original_value->array[0] = 999;

    ck_assert_int_eq(original_value->array[0], 999);
    ck_assert_int_eq(copy_value->array[0], 1);

    copy_value->array[1] = 888;

    ck_assert_int_eq(copy_value->array[1], 888);
    ck_assert_int_eq(original_value->array[1], 2);

    keyval_free(&original);
    keyval_free(&copy);
}
END_TEST


/* ============================================================
 * FREE
 * ============================================================ */

START_TEST(test_keyval_free_default)
{
    keyval_create(kv);

    keyval_fill(&kv, "key", "value");

    keyval_free(&kv);

    /*
     * Сейчас keyval_free() освобождает память,
     * но не обнуляет поля.
     *
     * Поэтому здесь нельзя обращаться к kv.key/value.
     */
    ck_assert(1);
}
END_TEST


START_TEST(test_keyval_free_custom)
{
    int data[] = {1, 2, 3};

    ArrayInt_s value = create_ArrayInt_s(
        .array = data,
        .size = 3
    );

    keyval_create(
        kv,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    keyval_fill(&kv, &value, &value);

    keyval_free(&kv);

    ck_assert(1);
}
END_TEST


/* ============================================================
 * PRINT
 * ============================================================ */

START_TEST(test_keyval_print_calls_functions)
{
    print_called = 0;

    keyval_create(
        kv,
        .key_funcs = test_funcs,
        .val_funcs = test_funcs
    );

    keyval_fill(&kv, "key", "value");

    keyval_print(&kv);

    ck_assert_int_eq(print_called, 2);

    keyval_free(&kv);
}
END_TEST


START_TEST(test_keyval_print_custom)
{
    int data[] = {1, 2, 3};

    ArrayInt_s value = create_ArrayInt_s(
        .array = data,
        .size = 3
    );

    keyval_create(
        kv,
        .key_funcs = array_int_funcs,
        .val_funcs = array_int_funcs
    );

    keyval_fill(&kv, &value, &value);

    keyval_print(&kv);

    keyval_free(&kv);

    ck_assert(1);
}
END_TEST


/* ============================================================
 * ERRORS
 * ============================================================ */

START_TEST(test_chars_create_null)
{
    TypeFuncs funcs = typefuncs_chars();

    ck_assert_ptr_null(funcs.create(NULL));
}
END_TEST


START_TEST(test_chars_free_null)
{
    TypeFuncs funcs = typefuncs_chars();

    ck_assert_ptr_null(funcs.free(NULL));
}
END_TEST


START_TEST(test_keyval_fill_null_functions)
{
    KeyVal kv = {
        .key = NULL,
        .value = NULL,
        .key_funcs = {0},
        .val_funcs = {0}
    };

    ck_assert_ptr_null(keyval_fill(&kv, "key", "value"));

    ck_assert_ptr_null(kv.key);
    ck_assert_ptr_null(kv.value);
}
END_TEST


START_TEST(test_keyval_free_null_functions)
{
    KeyVal kv = {
        .key = NULL,
        .value = NULL,
        .key_funcs = {0},
        .val_funcs = {0}
    };

    ck_assert_ptr_null(keyval_free(&kv));
}
END_TEST


START_TEST(test_keyval_print_null_functions)
{
    KeyVal kv = {
        .key = NULL,
        .value = NULL,
        .key_funcs = {0},
        .val_funcs = {0}
    };

    ck_assert_ptr_null(keyval_print(&kv));
}
END_TEST


START_TEST(test_keyval_update_null_functions)
{
    KeyVal kv = {
        .key = NULL,
        .value = NULL,
        .key_funcs = {0},
        .val_funcs = {0}
    };

    ck_assert_ptr_null(keyval_update(&kv, "value"));
}
END_TEST


/* ============================================================
 * ERROR STRING
 * ============================================================ */

START_TEST(test_keyval_error_string)
{
    ck_assert_str_eq(
        keyval_error_string(KEYVAL_OK),
        "no error"
    );

    ck_assert_str_eq(
        keyval_error_string(KEYVAL_ERROR_NULL_ARGUMENT),
        "null argument"
    );

    ck_assert_str_eq(
        keyval_error_string(KEYVAL_ERROR_INDEX_OUT_OF_RANGE),
        "index out of range"
    );

    ck_assert_str_eq(
        keyval_error_string(KEYVAL_ERROR_EMPTY),
        "keyval is empty"
    );

    ck_assert_str_eq(
        keyval_error_string(KEYVAL_ERROR_MEMORY),
        "no memory"
    );

    ck_assert_str_eq(
        keyval_error_string(KEYVAL_ERROR_NULL_FUNCTION),
        "null function"
    );

    ck_assert_str_eq(
        keyval_error_string((KeyValError)-1),
        "unknown error"
    );
}
END_TEST


/* ============================================================
 * ArrayInt_s
 * ============================================================ */

START_TEST(test_array_int_create)
{
    int data[] = {1, 2, 3};

    ArrayInt_s original = create_ArrayInt_s(
        .array = data,
        .size = 3
    );

    ArrayInt_s *copy = member_create_ArrayInt_s(&original);

    ck_assert_ptr_nonnull(copy);
    ck_assert_uint_eq(copy->size, 3);
    ck_assert_ptr_ne(copy->array, original.array);

    for (size_t i = 0; i < copy->size; ++i)
        ck_assert_int_eq(copy->array[i], original.array[i]);

    member_free_ArrayInt_s(copy);
}
END_TEST


START_TEST(test_array_int_create_null)
{
    ck_assert_ptr_null(
        member_create_ArrayInt_s(NULL)
    );
}
END_TEST


START_TEST(test_array_int_empty)
{
    ArrayInt_s original = create_ArrayInt_s(
        .array = NULL,
        .size = 0
    );

    ArrayInt_s *copy = member_create_ArrayInt_s(&original);

    ck_assert_ptr_nonnull(copy);
    ck_assert_uint_eq(copy->size, 0);

    free(copy->array);
    free(copy);
}
END_TEST


/* ============================================================
 * Suite
 * ============================================================ */

static Suite *keyval_suite(void)
{
    Suite *s = suite_create("KeyVal");


    /* Create */

    TCase *tc_create = tcase_create("Create");

    tcase_add_test(tc_create, test_keyval_create_default);
    tcase_add_test(tc_create, test_keyval_create_custom);
    tcase_add_test(tc_create, test_keyval_create_custom_key_only);

    suite_add_tcase(s, tc_create);


    /* Fill */

    TCase *tc_fill = tcase_create("Fill");

    tcase_add_test(tc_fill, test_keyval_fill_default);
    tcase_add_test(tc_fill, test_keyval_fill_custom);
    tcase_add_test(tc_fill, test_keyval_fill_null_key);
    tcase_add_test(tc_fill, test_keyval_fill_null_value);
    tcase_add_test(tc_fill, test_keyval_fill_both_null);

    suite_add_tcase(s, tc_fill);


    /* Update */

    TCase *tc_update = tcase_create("Update");

    tcase_add_test(tc_update, test_keyval_update_default);
    tcase_add_test(tc_update, test_keyval_update_custom);
    tcase_add_test(tc_update, test_keyval_update_preserves_key);
    tcase_add_test(tc_update, test_keyval_update_null);

    suite_add_tcase(s, tc_update);


    /* Copy */

    TCase *tc_copy = tcase_create("Copy");

    tcase_add_test(tc_copy, test_keyval_copy_default);
    tcase_add_test(tc_copy, test_keyval_copy_custom);
    tcase_add_test(tc_copy, test_keyval_copy_is_independent);
    tcase_add_test(tc_copy, test_keyval_copy_array_is_independent);

    suite_add_tcase(s, tc_copy);


    /* Free */

    TCase *tc_free = tcase_create("Free");

    tcase_add_test(tc_free, test_keyval_free_default);
    tcase_add_test(tc_free, test_keyval_free_custom);

    suite_add_tcase(s, tc_free);


    /* Print */

    TCase *tc_print = tcase_create("Print");

    tcase_add_test(tc_print, test_keyval_print_calls_functions);
    tcase_add_test(tc_print, test_keyval_print_custom);

    suite_add_tcase(s, tc_print);


    /* Errors */

    TCase *tc_errors = tcase_create("Errors");

    tcase_add_test(tc_errors, test_chars_create_null);
    tcase_add_test(tc_errors, test_chars_free_null);
    tcase_add_test(tc_errors, test_keyval_fill_null_functions);
    tcase_add_test(tc_errors, test_keyval_free_null_functions);
    tcase_add_test(tc_errors, test_keyval_print_null_functions);
    tcase_add_test(tc_errors, test_keyval_update_null_functions);

    suite_add_tcase(s, tc_errors);


    /* Error string */

    TCase *tc_error_string = tcase_create("ErrorString");

    tcase_add_test(tc_error_string, test_keyval_error_string);

    suite_add_tcase(s, tc_error_string);


    /* ArrayInt_s */

    TCase *tc_array = tcase_create("ArrayInt");

    tcase_add_test(tc_array, test_array_int_create);
    tcase_add_test(tc_array, test_array_int_create_null);
    tcase_add_test(tc_array, test_array_int_empty);

    suite_add_tcase(s, tc_array);


    return s;
}


int main(void)
{
    Suite *s = keyval_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    int failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed == 0
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}