#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <getopt.h>

#include "../../src/range.h"
#include "../../src/format.h"
#include "../../src/csv.h"
#include "../../src/filter.h"
#include "../../src/read.h"

#ifndef PROGRAM_NAME
#   define PROGRAM_NAME "hantek_drc_csv"
#endif

int usage(void) {
    // TODO improve
    printf("Usage:\n\n" PROGRAM_NAME " <drc> <csv>\n"
        "\t[-F <column-list>]\n"
        "\t[-d <data-type>]\n"
        "\t[-v <value-type>]\n"
        "\t[-c <channel-range>]\n"
        "\t[-f <frame-range>]\n"
        "\t[-s <column-separator>]\n"
        "\t[-n <line-separator>]\n\n"
        "<column-list> is a sequence of characters, each character designates CSV column:\n"
        "\t'r' - index of sample (per frame)\n"
        "\t'h' - index of sample (per channel)\n"
        "\t'g' - global index of sample\n" 
        "\t'f' - frame index\n"
        "\t'd' - sample data number\n" 
        "\t'c' - channel index\n" 
        "\t'n' - channel number\n" 
    );
    return -1;
}

hantek_drc_csv_column parse_columns(const char* col_letters) {
    hantek_drc_csv_column col_list = 0;
    for (size_t i = 0; col_letters[i] != 0; ++i) {
        hantek_drc_csv_column col = 0;
        switch (col_letters[i]) {
        case 'r': col = HANTEK_DRC_CSV_COLUMN_FRAME_INDEX; break;
        case 'h': col = HANTEK_DRC_CSV_COLUMN_CHANNEL_INDEX; break;
        case 'g': col = HANTEK_DRC_CSV_COLUMN_GLOBAL_INDEX; break;
        case 'f': col = HANTEK_DRC_CSV_COLUMN_FRAME; break;
        case 'd': col = HANTEK_DRC_CSV_COLUMN_DATA; break;
        case 'c': col = HANTEK_DRC_CSV_COLUMN_CHANNEL; break;
        case 'n': col = HANTEK_DRC_CSV_COLUMN_CHANNEL_NUMBER; break;
        }
        hantek_drc_csv_column_set(&col_list, i, col);
    }
    return col_list;
}

hantek_drc_data_type parse_data_type(const char type_letter) {
    switch (type_letter) {
    case 'c': return HANTEK_DRC_DATA_TYPE_U8;
    case 'C': return HANTEK_DRC_DATA_TYPE_I8;
    case 's': return HANTEK_DRC_DATA_TYPE_U16;
    case 'S': return HANTEK_DRC_DATA_TYPE_I16;
    case 'i': return HANTEK_DRC_DATA_TYPE_U32;
    case 'I': return HANTEK_DRC_DATA_TYPE_I32;
    case 'l': return HANTEK_DRC_DATA_TYPE_U64;
    case 'L': return HANTEK_DRC_DATA_TYPE_I64;
    case 'f': return HANTEK_DRC_DATA_TYPE_F32;
    case 'F': return HANTEK_DRC_DATA_TYPE_F64;
    default: return HANTEK_DRC_DATA_TYPE_F32;
    }
}

void parse_data_format(const char* data_format, hantek_drc_data_format_params* params) {
    
    params->positive = false;
    params->multiplier_fn = NULL;
    params->divider_fn = NULL;
    params->divider = INT16_MAX;

    if (*data_format == 'm' || *data_format == 'v') {
        params->multiplier_fn = &hantek_drc_channel_volts_milli;
        params->divider_fn = NULL;
        params->divider *= (*data_format == 'v' ? 1000 : 1);
        return;
    }

    if (*data_format == '+') {
        params->positive = true;
        ++data_format;
    }

    char* end_ptr;
    params->multiplier = strtoull(data_format, &end_ptr, 10);
    if (*end_ptr != '/') {
        return;
    }

    data_format = end_ptr;
    params->divider *= strtoull(data_format, &end_ptr, 10);

}

int main(int argc, char *argv[]) {

    hantek_drc_data_format_params format = hantek_drc_data_format_volts(HANTEK_DRC_DATA_TYPE_F32);
    hantek_drc_range_filter_params range = {0};
    hantek_drc_csv_params csv = {.format=hantek_drc_data_format(&format)};

    int opt;
    while ((opt = getopt(argc, argv, "F:d:v:c:f:s:n:")) != -1) {
        switch (opt) {
        case 'F':
            csv.columns = parse_columns(optarg);
            break;
        case 'd':
            format.type = parse_data_type(*optarg);
            break;
        case 'c':
            range.channel = hantek_drc_parse_range(optarg);
            break;
        case 'f':
            range.frame = hantek_drc_parse_range(optarg);
            break;
        case 's':
            csv.column_separator = optarg;
            break;
        case 'n':
            csv.line_separator = optarg;
            break;
        case 'v':
            parse_data_format(optarg, &format);
            break;
        default:
            return usage();
        }
    }

    if (argc - optind < 2) {
        return usage();
    }
    
    const char* drc_path = argv[optind];
    csv.path = argv[optind + 1];

    hantek_drc_filter_params filter = {
        .filter = hantek_drc_range_filter(&range),
        .handler = hantek_drc_csv(&csv)
    };
    hantek_drc_info info = {
        .caps = hantek_drc_6254bd(),
        .handler = hantek_drc_filter(&filter)
    };
    if (!hantek_drc_read_file(drc_path, &info)) {
        fprintf(stderr, "Can't read DRC file \"%s\"\n", drc_path);
        return -1;
    }
    if (!hantek_drc_info_finish_free(&info)) {
        fprintf(stderr, "Can't finish DRC: \"%s\"", drc_path);
        return -1;
    }
    return 0;
}
