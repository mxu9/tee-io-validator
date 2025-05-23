/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "hal/base.h"
#include "hal/library/debuglib.h"

#include "library/spdm_requester_lib.h"
#include "library/spdm_crypt_lib.h"
#include "library/cxl_ide_km_requester_lib.h"
#include "ide_test.h"
#include "helperlib.h"
#include "teeio_debug.h"
#include "cxl_ide_lib.h"
#include "pcie_ide_lib.h"
#include "teeio_spdmlib.h"
#include "soc_cxl_test_lib.h"

extern const char* m_cxl_ide_mode_names[];

/**
* This function works to setup link_ide
*
* 1. open cofiguration_space and find the ecap_offset
* 2. map kcbar to user space
* 3. initialize spdm_context and doe_context
* 4. setup spdm_session
*/
static bool common_test_group_setup(void *test_context)
{
  bool ret = false;

  soc_cxl_test_group_context_t *context = (soc_cxl_test_group_context_t *)test_context;
  TEEIO_ASSERT(context->common.signature == GROUP_CONTEXT_SIGNATURE);

  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "test_group_setup start\n"));

  // first scan devices
  if(!cxl_scan_devices(test_context)) {
    teeio_record_group_result(TEEIO_TEST_GROUP_FUNC_SETUP, TEEIO_TEST_RESULT_FAILED, "Scan device failed.");
    return false;
  }

  // // initialize lower_port
  // ret = cxl_init_dev_port(context);
  // if(!ret) {
  //   teeio_record_group_result(TEEIO_TEST_GROUP_FUNC_SETUP, TEEIO_TEST_RESULT_FAILED, "Initialize device port failed.");
  //   return false;
  // }

  IDE_TEST_TOPOLOGY *top = context->common.top;
  TEEIO_ASSERT(top->connection == IDE_TEST_CONNECT_DIRECT || top->connection == IDE_TEST_CONNECT_SWITCH);

  ret = cxl_init_root_port(context);
  if (!ret) {
    teeio_record_group_result(TEEIO_TEST_GROUP_FUNC_SETUP, TEEIO_TEST_RESULT_FAILED, "Initialize root port failed.");
    return false;
  }

  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "test_group_setup done\n"));

  teeio_record_group_result(TEEIO_TEST_GROUP_FUNC_SETUP, TEEIO_TEST_RESULT_PASS, "");

  return true;
}

static bool common_test_group_teardown(void *test_context)
{
  soc_cxl_test_group_context_t *context = (soc_cxl_test_group_context_t *)test_context;
  TEEIO_ASSERT(context->common.signature == GROUP_CONTEXT_SIGNATURE);

  IDE_TEST_TOPOLOGY *top = context->common.top;
  TEEIO_ASSERT(top->connection == IDE_TEST_CONNECT_DIRECT || top->connection == IDE_TEST_CONNECT_SWITCH);

  // close ports
  // cxl_close_dev_port(&context->common.lower_port, top->type);
  cxl_close_root_port(context);

  teeio_record_group_result(TEEIO_TEST_GROUP_FUNC_TEARDOWN, TEEIO_TEST_RESULT_PASS, "");

  return true;
}

bool soc_cxl_test_group_setup(void *test_context)
{
  return common_test_group_setup(test_context);
}

bool soc_cxl_test_group_teardown(void *test_context)
{
  return common_test_group_teardown(test_context);
}
 