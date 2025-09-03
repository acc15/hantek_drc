#include "file.h"
#include "../src/value.h"

START_TEST(read_file) 
{
    hantek_drc_device_capabilities caps;
    hantek_drc_init_6254bd(&caps);

    hantek_drc_channel_info channels[4];
    hantek_drc_info info = {
        .caps = &caps,
        .channel_info = channels
    };

    //hantek_drc_init_6254bd(&info);
    //hantek_drc_free(&info);

    ck_assert(hantek_drc_read_file("samples/channels/1_ch.0.drc", &info));
    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 9);
    ck_assert_uint_eq(info.buffer_length, HANTEK_DRC_DATA_BUFFER_LENGTH);
    ck_assert_uint_eq(channels[0].index, 0);
    ck_assert_uint_eq(channels[0].number, 0);
    ck_assert_float_eq(hantek_drc_info_voltage_to_volts(&channels[0]), 1.f);
    ck_assert_float_eq(hantek_drc_info_timediv_to_seconds(&info), 0.01f);
}
END_TEST

TCase* file_tcase() {
    TCase *tcase = tcase_create("file");
    tcase_add_test(tcase, read_file);
    return tcase;
}
