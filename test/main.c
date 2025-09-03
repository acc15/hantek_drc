#include <stdlib.h>
#include "value.h"
#include "file.h"
#include "csv.h"

Suite* hantek_drc_suite() {
    Suite* suite = suite_create("hantek_drc");
    suite_add_tcase(suite, file_tcase());
    suite_add_tcase(suite, csv_tcase());
    suite_add_tcase(suite, value_tcase());
    return suite;
}

int main(void)
{
    SRunner *runner = srunner_create(hantek_drc_suite());
    srunner_run_all(runner, CK_VERBOSE);
    int failed_count = srunner_ntests_failed(runner);
    srunner_free(runner);
    return failed_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
