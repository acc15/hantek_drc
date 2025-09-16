#include "range.h"

#include "hantek_drc/range.h"

void check_range(hantek_drc_range actual, hantek_drc_range expected) {
    ck_assert_uint_eq(actual.enabled, expected.enabled);
    ck_assert_uint_eq(actual.min, expected.min);
    ck_assert_uint_eq(actual.max, expected.max);
}

START_TEST(parse_range) 
{
    check_range(hantek_drc_parse_range(NULL), (hantek_drc_range) {.enabled=false});
    check_range(hantek_drc_parse_range("11"), (hantek_drc_range) {.enabled=true, .min = 11, .max = 11});
    check_range(hantek_drc_parse_range("1-12"), (hantek_drc_range) {.enabled=true, .min = 1, .max = 12});
    check_range(hantek_drc_parse_range("letters"), (hantek_drc_range) {.enabled=false});
    check_range(hantek_drc_parse_range("2 2"), (hantek_drc_range) {.enabled=true, .min = 2, .max = 2});
}
END_TEST

TCase* range_tcase(void) {
    TCase *tcase = tcase_create("read");
    tcase_add_test(tcase, parse_range);
    return tcase;
}
