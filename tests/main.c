#include "multigrids.h"
#include "graph.h"
#include "multigrid-converter.h"
#include "path-finder.h"

#include <check.h>
#include <stdlib.h>

int main(/*int argc, char *argv[]*/) {
    int nfailed = 0;

    Suite *s;
    SRunner *runner;

    s = multigrid_suite();
    runner = srunner_create(s);

    s = graph_suite();
    srunner_add_suite(runner, s);

    s = multigrid_converter_suite();
    srunner_add_suite(runner, s);

    s = path_finder_suite();
    srunner_add_suite(runner, s);

    srunner_run_all(runner, CK_NORMAL);
    nfailed = srunner_ntests_failed(runner);

    srunner_free(runner);

    return nfailed ? EXIT_FAILURE : EXIT_SUCCESS;
}
