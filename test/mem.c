#include "mem.h"
#include "../src/mem.h"
#include "../src/read.h"

struct test_frame_data {
    size_t frame;
    size_t offset;
    float data[3];
};

START_TEST(write_mem) 
{
    hantek_drc_info info = hantek_drc_init_6254bd();

    hantek_drc_mem_params mem = {0};
    hantek_drc_mem_ext(&info, &mem);
    
    hantek_drc_data_format_params data_format = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_data_format_ext(&info, &data_format);

    ck_assert(hantek_drc_read_file("samples/data/ch_1_timediv_20ms_vdiv_500mv_triangle_full_scale.0.drc", &info));
    ck_assert_uint_eq(info.channel_count, 1);
    ck_assert_uint_eq(info.frame_count, 25);
    ck_assert_uint_eq(mem.channels[0].recorded, 25);
    ck_assert_uint_eq(hantek_drc_channel_volts_milli(&info.channel[0]), 500ULL);
    ck_assert_uint_eq(hantek_drc_info_sampling_rate_milli(&info), 12500000ULL);
    
    const float** frames = (const float**) mem.channels[0].frames;

    const struct test_frame_data frame_data[] = {
        { 0, 0,                                         { 0.062502F, 0.062502F, 0.078127F } },
        { 0, info.buffer_length - 3,                    { 1.984436F, 1.984436F, 1.984436F } },
        { info.frame_count - 1, 0,                      { 0.062502F, 0.062502F, 0.078127F } },
        { info.frame_count - 1, info.buffer_length - 3, { 1.984436F, 1.984436F, 1.984436F } }
    };

    const float tol = 0.0001F;
    for (size_t i = 0; i < sizeof(frame_data) / sizeof(frame_data[0]); ++i) {
        const struct test_frame_data* test_frame = &frame_data[i];
        const float* frame = frames[test_frame->frame];
        for (size_t j = 0; j < sizeof(frame_data[0].data) / sizeof(float); ++j) {
            ck_assert_float_eq_tol(frame[test_frame->offset + j], test_frame->data[j], tol);
        }
    }
    hantek_drc_free(&info);
}
END_TEST

TCase* mem_tcase(void) {
    TCase *tcase = tcase_create("mem");
    tcase_add_test(tcase, write_mem);
    return tcase;
}
