/**
 *  Copyright Notice:
 *  Copyright 2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "soc_cxl_test_lib.h"
#include "teeio_debug.h"
#include "soc_cxl_common.h"

// SOC-CXL supported config items
const char *m_soc_cxl_test_configuration_name[] = {
  "default",
  NULL
};

ide_test_config_funcs_t m_soc_cxl_config_func = {
  // Default Config
  soc_cxl_test_config_default_enable,
  soc_cxl_test_config_default_disable,
  soc_cxl_test_config_default_support,
  soc_cxl_test_config_default_check
};

ide_test_group_funcs_t m_soc_cxl_group_func = {
  soc_cxl_test_group_setup, soc_cxl_test_group_teardown
};

// TODO
// If there are more test cases, add them here
ide_test_case_name_t m_soc_cxl_test_case_names[] = {
    {"IdeCapabilityStructure",  "1",    SOC_CXL_TEST_CASE_IDE_CAPABILITY_STRUCTURE },
    {NULL,                      NULL,   SOC_CXL_TEST_CASE_NUM                       }
};

ide_test_case_funcs_t m_soc_cxl_test_ide_error_status_register_cases[] = {
  {
      soc_cxl_test_ide_capability_structure_1_setup, soc_cxl_test_ide_capability_structure_1_run,
      soc_cxl_test_ide_capability_structure_1_teardown, false
  }
};

TEEIO_TEST_CASES m_soc_cxl_test_case_funcs[SOC_CXL_TEST_CASE_NUM] = {
  {m_soc_cxl_test_ide_error_status_register_cases,  MAX_SOC_CXL_TEST_CASE_IDE_CAPABILITY_STRUCTURE_CASE_ID},
};

static const char* get_test_configuration_name (int configuration_type)
{
    if (configuration_type >
        sizeof (m_soc_cxl_test_configuration_name) / sizeof (const char*)) {
        return NULL;
    }

    return m_soc_cxl_test_configuration_name[configuration_type];
}

static uint32_t get_test_configuration_bitmask (int top_tpye)
{
    return (uint32_t) SOC_CXL_TEST_CONFIGURATION_BITMASK;
}

static ide_test_config_funcs_t* get_test_configuration_funcs (int top_type, int configuration_type)
{
    TEEIO_ASSERT (configuration_type == IDE_TEST_CONFIGURATION_TYPE_DEFAULT);

    return &m_soc_cxl_config_func;
}

static ide_test_group_funcs_t* get_test_group_funcs (int top_type)
{
    return &m_soc_cxl_group_func;
}

static ide_test_case_funcs_t* get_test_case_funcs (int case_class, int case_id)
{
    TEEIO_ASSERT (case_class < SOC_CXL_TEST_CASE_NUM);
    TEEIO_TEST_CASES *test_cases = &m_soc_cxl_test_case_funcs[case_class];

    TEEIO_ASSERT (case_id < test_cases->cnt);

    return &test_cases->funcs[case_id];
}

static ide_test_case_name_t* get_test_case_name (int case_class)
{
    TEEIO_ASSERT (case_class < SOC_CXL_TEST_CASE_NUM + 1);

    return &m_soc_cxl_test_case_names[case_class];
}

static void* alloc_soc_cxl_test_group_context (void)
{
  soc_cxl_test_group_context_t *context =
        (soc_cxl_test_group_context_t*) malloc (sizeof (soc_cxl_test_group_context_t));

    TEEIO_ASSERT (context);
    memset (context, 0, sizeof (soc_cxl_test_group_context_t));
    context->common.signature = GROUP_CONTEXT_SIGNATURE;

    return context;
}

static bool soc_cxl_check_configuration_bitmap (uint32_t *bitmap)
{
    // default config is always set
    *bitmap |= SOC_CXL_TEST_CONFIGURATION_BITMASK;

    TEEIO_DEBUG ((TEEIO_DEBUG_INFO, "soc_cxl configuration bitmap=0x%08x\n", *bitmap));

    return true;
}

bool soc_cxl_test_lib_register_test_suite_funcs (teeio_test_funcs_t *funcs)
{
    TEEIO_ASSERT (funcs);

    funcs->get_case_funcs_func = get_test_case_funcs;
    funcs->get_case_name_func = get_test_case_name;
    funcs->get_configuration_bitmask_func = get_test_configuration_bitmask;
    funcs->get_configuration_funcs_func = get_test_configuration_funcs;
    funcs->get_configuration_name_func = get_test_configuration_name;
    funcs->get_group_funcs_func = get_test_group_funcs;
    funcs->alloc_test_group_context_func = alloc_soc_cxl_test_group_context;
    funcs->check_configuration_bitmap_func = soc_cxl_check_configuration_bitmap;

    return true;
}
