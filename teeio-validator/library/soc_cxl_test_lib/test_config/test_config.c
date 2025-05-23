/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#include <stdlib.h>
#include <ctype.h>

#include "hal/base.h"
#include "hal/library/debuglib.h"

#include "ide_test.h"
#include "helperlib.h"
#include "teeio_debug.h"
#include "cxl_ide_lib.h"
#include "soc_cxl_common.h"

bool soc_cxl_test_config_default_enable(void *test_context)
{
  return true;
}

bool soc_cxl_test_config_default_disable(void *test_context)
{
  return true;
}

bool soc_cxl_test_config_default_support(void *test_context)
{
  return true;
}

bool soc_cxl_test_config_default_check(void *test_context)
{
  return true;
}
