
#include <string>
#include <math.h>
#include <stdlib.h>


#include "dan/archive/archive_api.h"

#include "dancc.h"
#include "dancc_data_source.h"


namespace dancc {

void DataSource::initDataBlock(size_t size) {
    if(data_buffer_size != size) {
        if(data_buffer) free(data_buffer);
        data_buffer = malloc(size);
        data_buffer_size = size;
        data_item_size = 1;
        if(data_header.sampling_rate <= .0)
            data_header.sampling_rate = (double)size; // one block per second //
    }
}







} // dancc
