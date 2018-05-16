
#include <iostream>
#include <string.h>
#include <cstdlib>

#include <hdf5/hdf5.h>

#include <Core/Options.h>

#include "dan/archive/archive_api.h"
#include "dan/archive/hdf_api.h"
#include "dan/archive/hdf_common_api.h"


static const char *FILE_NAME = "write_hdf.h5";
static const size_t chunk_items = 1024;
static const size_t dset_size  = 102400; //chunks

int read_out() {

   hid_t       file_id, group_id, dataset_id;  /* identifiers */
   herr_t      status;
   int         i, j, *dset_data;

   dset_data = (int*)malloc(sizeof(int)*dset_size*chunk_items);

   /* Open an existing file. */
   file_id = H5Fopen(FILE_NAME, H5F_ACC_RDWR, H5P_DEFAULT);
   group_id = H5Gopen2(file_id, "group1", H5P_DEFAULT);
   dataset_id = H5Dopen2(group_id, "data1", H5P_DEFAULT);
   status = H5Dread(dataset_id, H5T_NATIVE_INT,
                    H5S_ALL, H5S_ALL, H5P_DEFAULT,
                    dset_data);

   size_t errors = 0;
   for(size_t i=0; i<dset_size; ++i)
       for(size_t j=0; j<chunk_items; ++j)
           if(dset_data[i*chunk_items+j] != (int)j) errors++;

   if(errors) std::cerr << "data wrong in file\n";

   status = H5Dclose(dataset_id);
   status = H5Gclose(group_id);
   status = H5Fclose(file_id);

   return errors;
}






int main(int argc, char *argv[])
{
    err_t res;

    HDF_FILE file;
    HDF_GROUP group;
    HDF_DATASET dataset;

    hdfsrv_create_file(&file,FILE_NAME,0,0);
    // hdfsrv_open_file(&file,"write_hdf.h5",1);
    hdfsrv_create_group_in_file(&group,&file,"group1");

    int data[chunk_items];
    for(size_t i=0; i<chunk_items; ++i) data[i] = i; // RAMP //

    hdfsrv_create_dataset(&dataset,&group,"data1",H5T_NATIVE_INT,chunk_items,0,0);

    hdfsrv_flush_file(&file);
    hdfsrv_flush_group(&group);

    for(size_t i=0; i<dset_size; ++i)
        hdfsrv_write_data(&dataset,chunk_items,data);

    hdfsrv_flush(&dataset);
    hdfsrv_flush_group(&group);
    hdfsrv_flush_file(&file);

    hdfsrv_close_file(&file);

    // read_out();

    return 0;
}



