#include "read.h"
#include "../src/read.h"

START_TEST(read_file) 
{
    hantek_drc_info info = {
        .caps = hantek_drc_6254bd()
    };
    ck_assert(hantek_drc_read_file("samples/channels/1_ch.0.drc", &info));
    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 9);
    ck_assert_uint_eq(info.buffer_length, HANTEK_DRC_DATA_BUFFER_LENGTH);
    ck_assert_uint_eq(info.channel[0].index, 0);
    ck_assert_uint_eq(info.channel[0].number, 1);
    ck_assert_uint_eq(hantek_drc_channel_volts_milli(&info.channel[0]), 1000ULL);
    ck_assert_uint_eq(hantek_drc_info_timediv_nanos(&info), 10000000ULL);
}
END_TEST

TCase* read_tcase(void) {
    TCase *tcase = tcase_create("read");
    tcase_add_test(tcase, read_file);
    return tcase;
}
