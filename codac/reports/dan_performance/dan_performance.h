#ifndef DAN_PERFORMANCE_H
#define DAN_PERFORMANCE_H

#include <iostream>
#include <cstring>

#include "dan/common_defs.h"
#include "dan/timestamp.h"
#include "dan/prof_service.h"
#include "dan/dan_log_service.h"

// #include "dan_metadata_cache.h"
#include "dan/archive/archive_api.h"

// get subscriber conf //
#include "dan/client_api/archive_stream_api_client.h"


////////////////////////////////////////////////////////////////////////////////
//  UTILS  /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>

char *rtrim(const char *s)
{
  while( isspace(*s) || !isprint(*s) ) ++s;
  return strdup(s);
}

char *ltrim(const char *s)
{
  char *r = strdup(s);
  if (r != NULL)
  {
    char *fr = r + strlen(s) - 1;
    while( (isspace(*fr) || !isprint(*fr) || *fr == 0) && fr >= r) --fr;
    *++fr = 0;
  }
  return r;
}

char *trim(const char *s)
{
  char *r = rtrim(s);
  char *f = ltrim(r);
  free(r);
  return f;
}

size_t options_get_size(const char *p_size)
{
    char *trd = trim(p_size);
    std::string trd_str(trd);
    free(trd);
    trd = (char *)trd_str.c_str();

    int len = strlen(trd);
    if (len == 0) return -1;
    char last_char = trd[len - 1];
    if (isdigit(last_char)) return atol(trd);
    if (len == 1) return -1;
    switch (last_char) {
        case 'K': return atol(trd) * 1024UL;
        case 'M': return atol(trd) * 1048576UL;
        case 'G': return atol(trd) * 1073741824UL;
        case 'T': return atol(trd) * 1099511627776UL;
        case 'E': return atol(trd) * 1125899906842620UL;
    }

    std::cerr << "error parsing size \n";
    return -1;
}

size_t options_get_size(const std::string &str) {
    return options_get_size(str.c_str());
}



////////////////////////////////////////////////////////////////////////////////
//  OFSTREAM INSERTERS  ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// HDF_DATASET //
static std::ostream &
operator << (std::ostream &o, const HDF_DATASET &d) {
    o << "dataset       h5:" << d.dataset << "\n"
      << "name          " << d.name << "\n"
      << "type_id       h5:" << d.type_id << "\n"
      << "item_size     " << d.item_size << "\n"
      << "cur_size      " << d.cur_size << "\n"
      << "cur_pos       " << d.cur_pos << "\n"
      << "chunk_size    " << d.chunk_size << "\n"
      << "memspace      h5:" << d.memspace << "\n"
      << "dataspace     h5:" << d.dataspace << "\n"
      << "cur_size_byte " << (int)d.cur_size_byte << "\n"
      << "mode          " << d.mode << "\n";
    return o;
}

static std::ostream &
operator << (std::ostream &o, const HDF_DATASET *d) {
    return o << *d;
}


// DAN_CLIENT_CONFIG //
static std::ostream &
operator << (std::ostream &o, const DAN_CLIENT_CONFIG &d) {
    return o
            << "buffer size:   " << d.serverBufferSize << std::endl
            << "server name:   " << d.serverName << std::endl
            << "server port:   " << d.serverPort << std::endl
            << "subscriber id: " << d.subscriber_id << std::endl
               ;
}

static std::ostream &
operator << (std::ostream &o, const DAN_CLIENT_CONFIG *d) {
    return o << *d;
}





////////////////////////////////////////////////////////////////////////////////
//  DEVICE INIT  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Initialization of obligatory source level metadata binded with basic
// hardware or software characteristics
//
int device_init(dan_MetadataCash *meta_mngr)
{
    err_t res;

    SOURCE_STATIC_CONFIG static_config;
    init_source_static_config(&static_config);

    SAFE_STRCPY(static_config.source_type, "Test_Device");
    SAFE_STRCPY(static_config.source_version, "1.0Dev");
    SAFE_STRCPY(static_config.source_serial_number, "ManufactureCode111");

    ////////////////////////////////////////////////////////////////////////////////
    //  SOURCE CONDFIG /////////////////////////////////////////////////////////////
    //
    int in_sample_meta_header_len = 0;
    int in_sample_meta_footer_len = 0;

    static_config.item_type = danItemInt32;
    static_config.stream_type = streamTypeD0WAVE;
    static_config.in_sample_meta_header_len = in_sample_meta_header_len;
    static_config.in_sample_meta_footer_len = in_sample_meta_footer_len;

    // SET_META_SOURCE_STATIC //
    res = dan_metadata_cash_set_data(meta_mngr,
                                     metaTypeSourceStaticConfig,
                                     sizeof(SOURCE_STATIC_CONFIG),
                                     &static_config);


    ////////////////////////////////////////////////////////////////////////////////
    //  ADDITIONAL ATTRIBUTES  /////////////////////////////////////////////////////
    //
    stream_attribute attributes[2];
    memset(attributes, 0, 2 * sizeof(stream_attribute));

    SAFE_STRCPY(attributes[0].name, "user_attribute_1");
    SAFE_STRCPY(attributes[0].value, "1111");
    SAFE_STRCPY(attributes[0].unit, "uuu");
    SAFE_STRCPY(attributes[0].description, "This is user_attribute_1");

    SAFE_STRCPY(attributes[1].name, "user_attribute_2");
    SAFE_STRCPY(attributes[1].value, "1111");
    SAFE_STRCPY(attributes[1].unit, "uuu");
    SAFE_STRCPY(attributes[1].description, "This is user_attribute_2");


    // SET_META_USER_ATTRIBUTES(2, attributes) //
    res = dan_metadata_cash_set_data(meta_mngr,
                                     metaTypeUserAttributes,
                                     2 * sizeof(stream_attribute), attributes);


    ////////////////////////////////////////////////////////////////////////////////
    //  DEBUG  /////////////////////////////////////////////////////////////////////
    //
    STREAM_DEBUG debug;

    int nSamplesChunkSize = 1000000;
    int nHeadersChunkSize = 1;

    init_stream_debug(&debug);
    debug.payload_chunk_size = nSamplesChunkSize;
    debug.headers_chunk_size = nHeadersChunkSize;

    // SET METADATA //
    res = dan_metadata_cash_set_data(meta_mngr, metaTypeDebug,
                                     sizeof(STREAM_DEBUG), &debug);

    return res;
}


////////////////////////////////////////////////////////////////////////////////
//  CONFIG_INIT  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Initialization of obligatory publisher level metadata that helps integrate
// stream into whole system, identify stream and all its channels, map channels
// to PV names, etc.
//
int config_init(dan_MetadataCash *meta_mngr)
{
    int res;
    int n_channels = 1;

    PUBLISHER_STATIC_CONFIG pbl_static_config;
    init_publisher_static_config(&pbl_static_config);
    SAFE_STRCPY(pbl_static_config.source_id, "DAN_EXAMPLE");

    res = dan_metadata_cash_set_data(meta_mngr,
                               metaTypeStaticConfig,
                               sizeof(PUBLISHER_STATIC_CONFIG),
                               &pbl_static_config);

    channel_info channels_info[2];
    memset(channels_info, 0, 1 * sizeof(channel_info));

    channels_info[0].source_channel_number = 1;
    SAFE_STRCPY(channels_info[0].pv_name, "PV_NAME_1D_1");
    SAFE_STRCPY(channels_info[0].label, "FROM_DETECTOR_1");
    SAFE_STRCPY(channels_info[0].stream_unit, "a.u.");

    channels_info[1].source_channel_number = 2;
    SAFE_STRCPY(channels_info[1].pv_name, "PV_NAME_1D_2");
    SAFE_STRCPY(channels_info[1].label, "FROM_DETECTOR_2");
    SAFE_STRCPY(channels_info[1].stream_unit, "a.u.");

    // SET_META_STD_CHANNEL_INFO //
    res = dan_metadata_cash_set_data(meta_mngr,
                               metaTypeStdChannelInfo,
                               n_channels * sizeof(channel_info),
                               channels_info);

    return res;
}


////////////////////////////////////////////////////////////////////////////////
//  SUBSCRIBER INIT  ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int subscriber_init (dan_MetadataCash *meta) {
    err_t res;

    SUBSCRIBER_STATIC_CONFIG subscriber_conf;
    init_subscriber_static_config(&subscriber_conf);
    get_subscriber_static_config(&subscriber_conf);

    res = dan_metadata_cash_set_data(meta, metaTypeSubscriber,
                                     sizeof(SUBSCRIBER_STATIC_CONFIG),
                                     &subscriber_conf);

    return res;
}




////////////////////////////////////////////////////////////////////////////////
//  BLOCK HEADER DEFINITION  ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#ifdef USE_PACKED_STRUCTS
#define PACKED_ATTRIBUTE __attribute__((packed))
#else
#define PACKED_ATTRIBUTE
#endif


// Define extended header datastructure
typedef struct
{
    PUBLISHER_DATA_BLOCK_HEADER std_header;
#   ifdef EXTENDED_HEADER
    char grid;
    short filter;
    double wavelength_pos;
#   endif
} PACKED_ATTRIBUTE DUMMY_PUBLISHER_DATA_BLOCK_HEADER;

typedef struct
{
    pos_t start_sample_id;
    DUMMY_PUBLISHER_DATA_BLOCK_HEADER pbl_data;
}   PACKED_ATTRIBUTE  DUMMY_SUBSCRIBER_DATA_BLOCK_HEADER;

typedef struct
{
    diff_time_t timestamp;
    block_t size;
    block_t n_samples;
    pos_t block_begin_samples;
    pos_t block_begin_items1;
    int error_tag;
    int srv_reserve;
    DUMMY_SUBSCRIBER_DATA_BLOCK_HEADER clt_data;
}   PACKED_ATTRIBUTE  DUMMY_SERVER_DATA_BLOCK_HEADER;









int generate_data(dan_MetadataCash *meta, size_t blk_size,
                  char *&data, size_t *size)
{

    static char * s_data_buffer = 0;
    static size_t s_blk_size = 0;
    err_t res = 0;

    if(!s_data_buffer || s_blk_size != blk_size) {

        SOURCE_STATIC_CONFIG *src_cfg;
        buf_t src_cfg_len;
        res = dan_metadata_cash_get_data(meta, metaTypeSourceStaticConfig,
                                         &src_cfg_len,(void **)&src_cfg);

        src_cfg->item_type;

        PUBLISHER_STATIC_CONFIG *pbs_cfg;
        buf_t pbs_src_len;
        res = dan_metadata_cash_get_data(meta, metaTypeStaticConfig,
                                         &pbs_src_len,(void **)&pbs_cfg);


        channel_info *ch_info;
        buf_t  ch_info_len;
        size_t n_ch = meta->length[metaTypeStdChannelInfo] / sizeof(channel_info);
        res = dan_metadata_cash_get_data(meta, metaTypeStdChannelInfo,
                                         &ch_info_len,
                                         (void **)&ch_info);


        // alloc data buffer //
        if(s_data_buffer) free(s_data_buffer);
        s_blk_size = blk_size;
        s_data_buffer = (char*)malloc(s_blk_size);

        // ZERO
        memset(s_data_buffer,0,s_blk_size);

        // fill buffer with int ramp //
        size_t s = s_blk_size/sizeof(int);
        std::cout << "size = " << s <<"\n";
        int *p_data_int  = (int*)s_data_buffer;
        for(size_t i=0; i<s; ++i) {
            p_data_int[i] = i;
        }
    }


    *size = s_blk_size;
    data = s_data_buffer;
    return res;
}







#endif // DAN_PERFORMANCE_H
