#include "value.h"



START_TEST(value_from_index) 
{
    ck_assert_uint_eq(hantek_drc_value(0), 2);
    ck_assert_uint_eq(hantek_drc_value(1), 5);
    ck_assert_uint_eq(hantek_drc_value(2), 10);
    ck_assert_uint_eq(hantek_drc_value(3), 20);
    ck_assert_uint_eq(hantek_drc_value(4), 50);
    ck_assert_uint_eq(hantek_drc_value(5), 100);
    ck_assert_uint_eq(hantek_drc_value(6), 200);
    ck_assert_uint_eq(hantek_drc_value(7), 500);
    ck_assert_uint_eq(hantek_drc_value(8), 1000);
    ck_assert_uint_eq(hantek_drc_value(23), 100000000);
    ck_assert_uint_eq(hantek_drc_value(32), 100000000000);
    ck_assert_uint_eq(hantek_drc_value(32), 100000000000);
}
END_TEST

START_TEST(timediv_to_seconds) 
{
    ck_assert_float_eq(hantek_drc_timediv_seconds(35), 1000.0f);
    ck_assert_float_eq(hantek_drc_timediv_seconds(32), 100.0f);
    ck_assert_float_eq(hantek_drc_timediv_seconds(29), 10.0f);
    ck_assert_float_eq(hantek_drc_timediv_seconds(26), 1.0f);
}
END_TEST

START_TEST(generic_sampling_rate) 
{
    size_t x_div = 10;
    uint64_t max_rate = 1000000000000ULL;
    ck_assert_float_eq(hantek_drc_sampling_rate(x_div, max_rate, hantek_drc_timediv_nanos(35)), 0.25f);
    ck_assert_float_eq(hantek_drc_sampling_rate(x_div, max_rate, hantek_drc_timediv_nanos(34)), 0.5f);
    ck_assert_float_eq(hantek_drc_sampling_rate(x_div, max_rate, hantek_drc_timediv_nanos(33)), 1.25f);
    ck_assert_float_eq(hantek_drc_sampling_rate(x_div, max_rate, hantek_drc_timediv_nanos(32)), 2.5f);
    ck_assert_float_eq(hantek_drc_sampling_rate(x_div, max_rate, hantek_drc_timediv_nanos(7)), 500000000.f);
    for (uint16_t i = 0; i < 7; ++i) {
        ck_assert_float_eq(hantek_drc_sampling_rate(x_div, max_rate, hantek_drc_timediv_nanos(i)), 1000000000.f);
    }
}
END_TEST

TCase *value_tcase()
{
    TCase *tcase = tcase_create("value");
    tcase_add_test(tcase, value_from_index);
    tcase_add_test(tcase, timediv_to_seconds);
    tcase_add_test(tcase, generic_sampling_rate);
    return tcase;
}