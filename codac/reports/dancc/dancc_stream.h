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
        m_source(NULL) {
        init_stream();
    }

    ~Stream() {}

    const DataSource * getDataSource() const { return m_source; }

    ARCHIVE_STREAM_CLIENT *getStream() { return m_stream; }
    const ARCHIVE_STREAM_CLIENT *getStream() const { return m_stream; }

    err_t init_stream();

    void setDataSource(const DataSource &src);

    err_t addAttribute(const stream_attribute &att);

    void profileDataSave(const char *fname) {
        assert(0); // THIS WONT WORK BECAUSE GROUP DUPLICATE STREAM //
        profiler_data_save(&m_stream->profiler, fname);
    }
};




} // dancc


#endif // DANCC_STREAM_H
