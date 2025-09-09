#include "mem.h"
#include "../src/mem.h"
#include "../src/read.h"

START_TEST(write_mem) 
{
    hantek_drc_info info = hantek_drc_init_6254bd();
    hantek_drc_mem_params mem_params = {0};
    hantek_drc_mem_ext(&info, &mem_params);
    
    hantek_drc_data_format_params data_format = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_data_format_ext(&info, &data_format);

    ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_20ms_vdiv_500mv_triangle_full_scale.0.drc", &info));
    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 25);
    ck_assert_uint_eq(hantek_drc_channel_volts_milli(&info.channel[0]), 500ULL);
    ck_assert_uint_eq(hantek_drc_info_sampling_rate_milli(&info), 12500000ULL);
    
    hantek_drc_mem_params* params = (hantek_drc_mem_params*)info.frame_handler.params;
    float** frames = (float**) params->channels[0].frames;

    float tol = 0.0001f;
    
    float first_frame_head[] = { 0.062502f, 0.062502f, 0.078127f };
    for (size_t i = 0; i < sizeof(first_frame_head)/sizeof(first_frame_head[0]); ++i) {
        ck_assert_float_eq_tol(frames[0][i], first_frame_head[i], tol);
    }

    float first_frame_tail[] = { 1.984436f, 1.984436f, 1.984436f };
    size_t first_tail_index = info.buffer_length - sizeof(first_frame_tail)/sizeof(first_frame_tail[0]);
    for (size_t i = 0; i < sizeof(first_frame_tail)/sizeof(first_frame_tail[0]); ++i) {
        ck_assert_float_eq_tol(frames[0][first_tail_index + i], first_frame_tail[i], tol);
    }

    float last_frame_head[] = { 0.062502f, 0.062502f, 0.078127f };
    for (size_t i = 0; i < sizeof(last_frame_head)/sizeof(last_frame_head[0]); ++i) {
        ck_assert_float_eq_tol(frames[info.frame_count - 1][i], last_frame_head[i], tol);
    }
    float last_frame_tail[] = {1.984436f, 1.984436f, 1.984436f };
    size_t last_tail_index = info.buffer_length - sizeof(last_frame_tail)/sizeof(last_frame_tail[0]);
    for (size_t i = 0; i < sizeof(last_frame_tail)/sizeof(last_frame_tail[0]); ++i) {
        ck_assert_float_eq_tol(frames[info.frame_count - 1][last_tail_index + i], last_frame_tail[i], tol);
    }

    hantek_drc_free(&info);
}
END_TEST

TCase* mem_tcase(void) {
    TCase *tcase = tcase_create("mem");
    tcase_add_test(tcase, write_mem);
    return tcase;
}
