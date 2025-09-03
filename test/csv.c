#include "csv.h"

START_TEST(write_csv) 
{
    FILE* csv_file = fopen("build/ch_1_timediv_1us_vdiv_1v_triangle_10hz_no_clamp.0.csv", "wt");
    ck_assert_ptr_nonnull(csv_file);

    hantek_drc_csv_payload csv_payload = { .csv_file = csv_file };
    hantek_drc_device_capabilities caps;
    hantek_drc_init_6254bd(&caps);
    hantek_drc_channel_info channels[4];
    hantek_drc_info info = {
        .caps = &caps,
        .channel_info = channels, 
        .on_frame = &hantek_drc_csv_append_data,
        .payload = &csv_payload
    };
    ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_1us_vdiv_1v_triangle_10hz_no_clamp.0.drc", &info));
    fclose(csv_file);

    // hantek_drc_info info = {};
    // hantek_drc_init_6254bd(&info);
    // hantek_drc_to_memory(&info);
    // hantek_drc_to_csv_file(&info, "build/ch_1_timediv_1us_vdiv_1v_triangle_10hz_no_clamp.0.csv");
    // //hantek_drc_to_csv(&info, csv_file);
    // ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_1us_vdiv_1v_triangle_10hz_no_clamp.0.drc", &info));
    // hantek_drc_free(&info);

}
END_TEST

TCase* csv_tcase() {
    TCase *tcase = tcase_create("csv");
    tcase_add_test(tcase, write_csv);
    return tcase;
}
