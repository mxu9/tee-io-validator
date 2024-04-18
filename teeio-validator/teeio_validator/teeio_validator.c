/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#include "teeio_validator.h"
#include "hal/library/platform_lib.h"
#include <stdlib.h>
#include <ctype.h>
#include "ide_test.h"

char g_bdf[] = {'2','a',':','0','0','.','0','\0'};
char g_rp_bdf[] = {'2','9',':','0','2','.','0','\0'};
bool g_skip_tdisp = false;
uint8_t g_stream_id = 0;
bool g_pci_log = false;
bool g_ide_key_refresh = false;
TEST_IDE_TYPE g_test_ide_type = TEST_IDE_TYPE_SEL_IDE;
IDE_TEST_CONFIG ide_test_config = {0};
int g_top_id = 0;
int g_config_id = 0;
char g_test_case[MAX_CASE_NAME_LENGTH] = {0};
bool g_run_test_suite = true;

TEEIO_DEBUG_LEVEL g_debug_level = TEEIO_DEBUG_WARN;
uint8_t g_scan_bus = INVALID_SCAN_BUS;

FILE* m_logfile = NULL;

bool log_file_init(const char* filepath);
void log_file_close();

extern const char *IDE_TEST_IDE_TYPE_NAMES[];

bool parse_ide_test_init(IDE_TEST_CONFIG *test_config, const char* ide_test_ini);
bool parse_cmdline_option(int argc, char *argv[], char* file_name, IDE_TEST_CONFIG *ide_test_config, bool* print_usage, uint8_t* debug_level);
void print_usage();
bool run(IDE_TEST_CONFIG *test_config);
bool update_test_config_with_given_top_config_id(IDE_TEST_CONFIG *test_config, int top_id, int config_id, const char* test_case);

int main(int argc, char *argv[])
{
    char ide_test_ini_file[MAX_FILE_NAME] = {0};
    bool to_print_usage = false;
    int ret = -1;
    uint8_t debug_level = TEEIO_DEBUG_NUM;

    if (!log_file_init(LOGFILE)){
        TEEIO_PRINT(("Failed to init log file!\n"));
        return -1;
    }

    TEEIO_PRINT(("%s version %s\n", TEEIO_VALIDATOR_NAME, TEEIO_VALIDATOR_VERSION));

    // parse command line optioins
    if(!parse_cmdline_option(argc, argv, ide_test_ini_file, &ide_test_config, &to_print_usage, &debug_level)) {
        print_usage();
        goto MainDone;
    }

    if(debug_level != TEEIO_DEBUG_NUM) {
        g_debug_level = debug_level;
    }

    if(to_print_usage) {
        print_usage();
        ret = 0;
        goto MainDone;
    }

    if(ide_test_ini_file[0] == 0) {
        TEEIO_PRINT(("-f parameter is missing.\n"));
        print_usage();
        goto MainDone;
    }

    if(!parse_ide_test_init(&ide_test_config, ide_test_ini_file)) {
        TEEIO_PRINT(("Parse %s failed.\n", ide_test_ini_file));
        goto MainDone;
    }
    g_pci_log = ide_test_config.main_config.pci_log;

    if(debug_level == TEEIO_DEBUG_NUM) {
        g_debug_level = ide_test_config.main_config.debug_level;
    }

    // if g_top_ids is valid, then we go into xxx mode
    if(g_top_id != 0 && g_config_id != 0) {
        if(!update_test_config_with_given_top_config_id(&ide_test_config, g_top_id, g_config_id, g_test_case)) {
            goto MainDone;
        }
    }

    srand((unsigned int)time(NULL));

    run(&ide_test_config);

    ret = 0;

MainDone:
    log_file_close();

    return ret;
}
