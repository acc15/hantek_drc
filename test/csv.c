#include "csv.h"
#include "../src/read.h"
#include "../src/csv.h"

START_TEST(write_csv) 
{
    hantek_drc_info info = hantek_drc_init_6254bd();
    hantek_drc_csv_init(&info, "build/test.csv");
    hantek_drc_data_format_volts(&info, HANTEK_DRC_DATA_TYPE_F32);
    ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_20ms_vdiv_500mv_triangle_full_scale.0.drc", &info));
    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 25);
    ck_assert_uint_eq(hantek_drc_channel_volts_milli(&info.channel[0]), 500ULL);
    ck_assert_uint_eq(hantek_drc_info_sampling_rate_milli(&info), 12500000ULL);
    hantek_drc_free(&info);
}
END_TEST

TCase* csv_tcase(void) {
    TCase *tcase = tcase_create("csv");
    tcase_add_test(tcase, write_csv);
    return tcase;
}
