#include "csv.h"

START_TEST(write_csv) 
{
    hantek_drc_info info;
    hantek_drc_init_6254bd(&info);
    hantek_drc_csv_init(&info, "build/test.csv", NULL);
    ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_20ms_vdiv_500mv_triangle_full_scale.0.drc", &info));
    hantek_drc_free(&info);

    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 25);
    ck_assert_float_eq(hantek_drc_channel_volts(&info.channel[0]), 0.5f);
    ck_assert_float_eq(hantek_drc_info_sampling_rate(&info), 12500.f);

}
END_TEST

TCase* csv_tcase(void) {
    TCase *tcase = tcase_create("csv");
    tcase_add_test(tcase, write_csv);
    return tcase;
}
