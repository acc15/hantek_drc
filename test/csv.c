#include "csv.h"
#include "../src/csv.h"
#include "../src/read.h"

START_TEST(write_csv) 
{
    hantek_drc_data_format_params format_params = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_csv_params csv_params = {
        .path = "build/test.csv",
        .columns = hantek_drc_csv_columns(2, HANTEK_DRC_CSV_COLUMN_DATA, HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX),
        .format = hantek_drc_data_format_handler(&format_params)
    };
    hantek_drc_info info = {
        .caps = hantek_drc_6254bd(),
        .frame_handler = hantek_drc_csv_handler(&csv_params)
    };
    
    ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_20ms_vdiv_500mv_triangle_full_scale.0.drc", &info));
    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 25);
    ck_assert_uint_eq(hantek_drc_channel_volts_milli(&info.channel[0]), 500ULL);
    ck_assert_uint_eq(hantek_drc_info_sampling_rate_milli(&info), 12500000ULL);
    hantek_drc_info_free(&info);
}
END_TEST

START_TEST(csv_columns) 
{
    hantek_drc_csv_column cols = hantek_drc_csv_columns(3, HANTEK_DRC_CSV_COLUMN_CHANNEL, HANTEK_DRC_CSV_COLUMN_DATA);
    ck_assert_uint_eq(hantek_drc_csv_column_at(cols, 0), HANTEK_DRC_CSV_COLUMN_CHANNEL);
    ck_assert_uint_eq(hantek_drc_csv_column_at(cols, 1), HANTEK_DRC_CSV_COLUMN_DATA);
}
END_TEST

TCase* csv_tcase(void) {
    TCase *tcase = tcase_create("csv");
    tcase_add_test(tcase, write_csv);
    tcase_add_test(tcase, csv_columns);
    return tcase;
}
