#ifndef DAN_METADATA_CASH_H
#define DAN_METADATA_CASH_H

// #include "dan/dan_cMetadataCash.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char data_model[SHORT_STRSIZE];
    char version[SHORT_STRSIZE];
    buf_t length[metaTypeLast];
    void *p_metadata[metaTypeLast];
} dan_MetadataCash;

void dan_metadata_cash_init(dan_MetadataCash *p_meta_cash);
void dan_metadata_cash_release(dan_MetadataCash *p_meta_cash);

void dan_metadata_cash_set_data_model(dan_MetadataCash *p_meta_cash, const char *p_data_model);

err_t dan_metadata_cash_set_data(dan_MetadataCash *p_meta_cash, enumMetaDataType type, buf_t len, const void *p_data);
err_t dan_metadata_cash_get_data(const dan_MetadataCash *p_meta_cash, enumMetaDataType type, buf_t *p_len, void **pp_data);

// allocates buffer, delegates ownership
void *dan_metadata_cash_get_all_data(dan_MetadataCash *p_meta_cash, int *dataSize);
err_t dan_metadata_cash_set_all_data(dan_MetadataCash *p_meta_cash, int dataSize, const void *p_all_metadata);

void dan_metadata_cash_clear_user_attributes(dan_MetadataCash *p_meta_cash);
// Overwrites attributes with the same name
void dan_metadata_cash_add_user_attributes(dan_MetadataCash *p_meta_cash, int n_records, stream_attribute *p_data);
#define ERROR_USER_ATTR_EXCEED_LIMIT -1
#define ERROR_USER_ATTR_ALREADY_EXCEED_LIMIT -2
int dan_metadata_cash_add_user_attributes_limit(dan_MetadataCash *p_meta_cash, int n_records, stream_attribute *p_data, int max_user_attributes);


//For debugging
void dan_metadata_cash_trace(const dan_MetadataCash *p_meta_cash);
void dan_metadata_cash_trace_full(const dan_MetadataCash *p_meta_cash);
void dan_metadata_cash_trace_full_serialized(int dataSize, const void *p_meta);

//New since 5.1
#ifndef VERSION_5_0
bool dan_metadata_cash_check_meta(const dan_MetadataCash *p_meta_cash, enumMetaDataType type);
bool dan_metadata_cash_verify(const dan_MetadataCash *p_meta_cash);
#endif

int get_estimated_meta_size(const char *p_data_model, int n_channels, int item_fields,
    int n_header_extansion_fields, int n_channel_info_extension_fields,
    int channel_info_extension_size, int n_user_attributes);



#ifdef __cplusplus
}
#endif




#endif // DAN_METADATA_CASH_H
