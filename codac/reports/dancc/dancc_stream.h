#ifndef DANCC_STREAM_H
#define DANCC_STREAM_H


#include "dan/client_api/archive_stream_api_client.h"
#include "dan/archive/archive_api.h"


#include "dancc.h"
#include "dancc_data_source.h"




namespace dancc {





////////////////////////////////////////////////////////////////////////////////
//  Stream  ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Stream {

    PUBLISHER_STATIC_CONFIG pbl_static_config;
    ARCHIVE_STREAM_CLIENT  *m_stream;
    const DataSource       *m_source;
public:

    Stream(ARCHIVE_STREAM_CLIENT *other = NULL) :
        m_stream(other),
        m_source(NULL)
    {
        init_stream();
    }

    ~Stream() {}

    const DataSource * getDataSource() const { return m_source; }

    ARCHIVE_STREAM_CLIENT *getStream() { return m_stream; }
    const ARCHIVE_STREAM_CLIENT *getStream() const { return m_stream; }

    err_t init_stream() {
        err_t res = 0;
        if (!m_stream) {
            m_stream = (ARCHIVE_STREAM_CLIENT*)malloc(sizeof(ARCHIVE_STREAM_CLIENT));
            dan_MetadataCash c_meta;
            dan_metadata_cash_init(&c_meta);
            res = dan_create_archive_stream(m_stream, sizeof(dan_MetadataCash),&c_meta);
        }
        return res;
    }

    void setDataSource(const DataSource &src) {
        err_t res;
        m_source = &src;
        //        dan_MetadataCash *meta = &m_stream->meta_cash;

        init_stream();
        m_stream->meta_cash = *(const dan_MetadataCash*)src.meta();

        // PROCESS META //
        dan_process_metadata(m_stream);

        // DATA HEADER //
        m_stream->p_header_buffer = (char*)malloc(sizeof(DAN_CLT_DATA_BLOCK_HEADER));
        m_stream->header_len = sizeof(DAN_CLT_DATA_BLOCK_HEADER);
        const void *d; const void *h;
        src.getDataBlock(d,h);
        dan_set_publisher_header(m_stream, sizeof(PUBLISHER_DATA_BLOCK_HEADER),h);
    }

    err_t addAttribute(const stream_attribute &att) {
        err_t res;
        init_stream();

        dan_MetadataCash *meta = &m_stream->meta_cash;
        stream_attribute *new_attributes;
        stream_attribute *old_attributes;
        buf_t old_attributes_len;
        dan_metadata_cash_get_data(meta, metaTypeUserAttributes,
                                   &old_attributes_len, (void**)&old_attributes);

        new_attributes = (stream_attribute *)malloc(old_attributes_len+sizeof(stream_attribute));
        if(old_attributes_len) memcpy(new_attributes,old_attributes,old_attributes_len);
        memcpy(((char*)new_attributes)+old_attributes_len,&att,sizeof(stream_attribute));

        res = dan_metadata_cash_set_data(meta,
                                         metaTypeUserAttributes,
                                         old_attributes_len + sizeof(stream_attribute),
                                         new_attributes);
        free(old_attributes);
        return res;
    }


};




} // dancc


#endif // DANCC_STREAM_H
