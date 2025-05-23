/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ide_test.h"
#include "teeio_debug.h"

#include "library/spdm_requester_lib.h"
#include "hal/library/memlib.h"
#include "helperlib.h"
#include "cxl_ide_lib.h"
#include "soc_cxl_common.h"

// Test case of "IdeCapabilityStructure"

bool soc_cxl_test_ide_capability_structure_1_setup (void *test_context)
{
  return true;
}
void soc_cxl_test_ide_capability_structure_1_run (void *test_context)
{
  uint8_t* ptr = NULL;
  int i = 0;

  ide_common_test_case_context_t *case_context = (ide_common_test_case_context_t *)test_context;
  TEEIO_ASSERT(case_context);
  TEEIO_ASSERT(case_context->signature == CASE_CONTEXT_SIGNATURE);

  soc_cxl_test_group_context_t *group_context = (soc_cxl_test_group_context_t *)case_context->group_context;
  TEEIO_ASSERT(group_context);
  TEEIO_ASSERT(group_context->common.signature == GROUP_CONTEXT_SIGNATURE);

  ide_run_test_case_t* test_case = case_context->test_case;
  TEEIO_ASSERT(test_case);
  int case_class = test_case->class_id;
  int case_id = test_case->case_id;

  // INTEL_KEYP_CXL_ROOT_COMPLEX_KCBAR *kcbar_ptr = (INTEL_KEYP_CXL_ROOT_COMPLEX_KCBAR *)group_context->common.upper_port.mapped_kcbar_addr;
  ide_common_test_port_context_t* upper_port = &group_context->common.upper_port;
  // ide_common_test_port_context_t* lower_port = &group_context->common.lower_port;

  CXL_CAPABILITY_XXX_HEADER* cap_header = upper_port->cxl_data.memcache.cap_headers;
  int cap_headers_cnt = upper_port->cxl_data.memcache.cap_headers_cnt;
  uint8_t* mapped_memcache_reg_block = upper_port->cxl_data.memcache.mapped_memcache_reg_block;
  for(; i < cap_headers_cnt; i++) {
    if(cap_header[i].cap_id == CXL_CAPABILITY_ID_IDE_CAP) {
      break;
    }
  }

  if(i == cap_headers_cnt) {
    teeio_record_assertion_result(case_class, case_id, 0, IDE_COMMON_TEST_CASE_ASSERTION_TYPE_TEST, TEEIO_TEST_RESULT_FAILED, "CXL IDE Capability is not found.");
    TEEIO_DEBUG((TEEIO_DEBUG_ERROR, "Cannot find CXL IDE Capability!\n"));
    return;
  }

  // Refer to CXL Spec Section 8.2.4.22
  // CXL IDE Capability Structure
  ptr = mapped_memcache_reg_block + cap_header[i].pointer + OFFSET_OF(CXL_IDE_CAPABILITY_STRUCT, error_status);
  CXL_IDE_ERROR_STATUS error_status = {.raw = mmio_read_reg32(ptr)};

  teeio_test_result_t assertion_result = error_status.rx_error_status == 0 ? TEEIO_TEST_RESULT_PASS : TEEIO_TEST_RESULT_FAILED;
  teeio_record_assertion_result(case_class, case_id, 1, IDE_COMMON_TEST_CASE_ASSERTION_TYPE_TEST, assertion_result, "rx_error_status = 0x%lx", error_status.rx_error_status);
  
  assertion_result = error_status.tx_error_status == 0 ? TEEIO_TEST_RESULT_PASS : TEEIO_TEST_RESULT_FAILED;
  teeio_record_assertion_result(case_class, case_id, 2, IDE_COMMON_TEST_CASE_ASSERTION_TYPE_TEST, assertion_result, "tx_error_status = 0x%lx", error_status.tx_error_status);

}
void soc_cxl_test_ide_capability_structure_1_teardown (void *test_context)
{
}
