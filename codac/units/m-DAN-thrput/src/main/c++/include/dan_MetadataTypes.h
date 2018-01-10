#ifndef SRC_TEST_C_DATACORE_API_DAN_DBHEADERS_H_
#define SRC_TEST_C_DATACORE_API_DAN_DBHEADERS_H_

#include "dan/common_defs.h"

typedef struct
{
    PUBLISHER_DATA_BLOCK_HEADER std_header;
    short pos_x;
    short pos_y;
} __attribute__((packed)) DAN_DATA_BLOCK_HEADER_TYPE_IMG;

#endif
