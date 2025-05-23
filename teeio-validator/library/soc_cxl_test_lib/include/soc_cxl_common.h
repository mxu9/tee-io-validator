/**
 *  Copyright Notice:
 *  Copyright 2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#ifndef _SOC_CXL_COMMON_H_
#define _SOC_CXL_COMMON_H_

#include "stdbool.h"

//
// SOC_CXL test cases
//

// IDE Error Status Case 1
bool soc_cxl_test_ide_capability_structure_1_setup (void *test_context);
void soc_cxl_test_ide_capability_structure_1_run (void *test_context);
void soc_cxl_test_ide_capability_structure_1_teardown (void *test_context);

//
// SOC_CXL Test Config
//

// default config
bool soc_cxl_test_config_default_enable (void *test_context);
bool soc_cxl_test_config_default_disable (void *test_context);
bool soc_cxl_test_config_default_support (void *test_context);
bool soc_cxl_test_config_default_check (void *test_context);

//
// SOC_CXL Test Group
//

// selective_ide test group
bool soc_cxl_test_group_setup (void *test_context);
bool soc_cxl_test_group_teardown (void *test_context);

#endif
