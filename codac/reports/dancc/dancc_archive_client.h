#ifndef DANCC_ARCHIVE_H
#define DANCC_ARCHIVE_H

#include <Core/foreach.h>
#include <Core/Thread.h>
#include <Core/Lockable.h>

#include "dan/archive/archive_api.h"
#include "dan/client_api/archive_stream_api_client.h"

#include "dancc_data_source.h"
#include "dancc_stream.h"

namespace dancc {

static time_sec_t ts_secs_0 = 0;
static time_nanosec_t ts_nanos_0 = 1460000000000000000UL;


////////////////////////////////////////////////////////////////////////////////
//  ArchiveClient   ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// NOTE: All clients are also subscribers to data

class ArchiveClient : public Lockable
{

    typedef DAN_CLIENT_CONFIG           ClientConfig;
    typedef ARCHIVE_STREAM_CLIENT_GROUP ArchiveClientGroupInfo;
    typedef ARCHIVE_STREAM_CLIENT       ArchiveStreamClientInfo;


    class WriteDataBlockThread : public Thread {

        ArchiveClient *m_parent;
        ArchiveStreamClientInfo *m_stream;
        const DataSource *m_data;
        size_t m_nblocks;               

    public:
        WriteDataBlockThread(ArchiveClient *parent, ArchiveStreamClientInfo *stream,
                             const DataSource *data, size_t nblocks = 1) :
            m_parent(parent),
            m_stream(stream),
            m_data(data),
            m_nblocks(nblocks)
        {}

        void InternalThreadEntry();
    };


    ClientConfig            client;
    ArchiveClientGroupInfo  m_group;
    std::vector<Stream*>     streams;
public:

    ArchiveClient(const char *name) {
        err_t res;
        SAFE_STRCPY(m_group.name, name);
        m_group.n_streams = 0;
        m_group.p_streams = NULL;

    }

    ArchiveClientGroupInfo &group() { return m_group; }
    const ArchiveClientGroupInfo &group() const { return m_group; }

    void setServer(const char *name, const char *port) {
        SAFE_STRCPY(client.subscriber_id, m_group.name);
        SAFE_STRCPY(client.serverName, name);
        SAFE_STRCPY(client.serverPort, port);
        client.serverBufferSize = 1024*512;
    }

    void setPulse(const char *id) {
        SAFE_STRCPY(m_group.pulse_id, id);
        m_group.connected = 0;
    }

    void addDataStream(Stream &stream) {
        streams.push_back(&stream);
        if(m_group.n_streams) free(m_group.p_streams);
        m_group.p_streams = (ARCHIVE_STREAM_CLIENT *)malloc(sizeof(ARCHIVE_STREAM_CLIENT) * streams.size());
        m_group.n_streams = streams.size();
        for(int i=0; i<m_group.n_streams; ++i) {
            memcpy(&m_group.p_streams[i], streams[i]->getStream(), sizeof(ARCHIVE_STREAM_CLIENT));
        }
    }


    void writeAllBlocksPerStream(size_t n_datablocks = 1) {
        dan_open_stream_group ( &m_group, &client );
        for(int i=0; i<m_group.n_streams; ++i) {
            ARCHIVE_STREAM_CLIENT *stream = dan_get_archive_stream(&m_group, i);
            // ARCHIVE_STREAM_CLIENT *stream = streams[i]->getStream(); // NO !
            const DataSource *source = streams[i]->getDataSource();
            const void *buffer;
            const void *header;

            for (size_t b=0; b<n_datablocks; ++b) {
                size_t n_samples = source->getDataBlock(buffer,header);

                time_nanosec_t ts_nanos;
                ts_nanos = ts_secs_0 * 1e9 + ts_nanos_0 + b * n_samples * 1e9 / source->getSampleRate();
                dan_write_archive_datablock(stream, ts_nanos, 0, n_samples, buffer);

                // std::cout << (DAN_CLT_DATA_BLOCK_HEADER*)stream->p_header_buffer << "\n";
                dan_commit_datablock(stream, 0);
            }
        }
        dan_close_stream_group( &m_group );
    }

    void writeSingleBlockPerStream(size_t n_datablocks = 1) {
        dan_open_stream_group ( &m_group, &client );
        for (size_t b=0; b<n_datablocks; ++b) {
            for(int i=0; i<m_group.n_streams; ++i) {
                ARCHIVE_STREAM_CLIENT *stream = dan_get_archive_stream(&m_group, i);
                const DataSource *source = streams[i]->getDataSource();
                const void *buffer;
                const void *header;
                size_t n_samples = source->getDataBlock(buffer,header);

                time_nanosec_t ts_nanos;
                ts_nanos = ts_secs_0 * 1e9 + ts_nanos_0 + b * n_samples * 1e9 / source->getSampleRate();
                dan_write_archive_datablock(stream, ts_nanos, b, n_samples, buffer);
                dan_commit_datablock(stream, 0);
            }

        }
        dan_close_stream_group( &m_group );
    }


    void writeMultiThreads(size_t n_datablocks = 1) {
        dan_open_stream_group ( &m_group, &client );
        std::vector<WriteDataBlockThread *> thr(m_group.n_streams);

        for(int i=0; i<m_group.n_streams; ++i) {
            ARCHIVE_STREAM_CLIENT *stream = dan_get_archive_stream(&m_group, i);
            thr[i] = new WriteDataBlockThread(this, stream, streams[i]->getDataSource(), n_datablocks);
            thr[i]->StartThread();
        }
        for(int i=0; i<m_group.n_streams; ++i) {
            thr[i]->WaitForThreadToExit();            
            delete thr[i];
        }
        dan_close_stream_group( &m_group );
    }

};

void ArchiveClient::WriteDataBlockThread::InternalThreadEntry()
{
    const void *buffer;
    const void *header;    
    for(size_t i=0; i<m_nblocks; ++i) {
        size_t n_samples = m_data->getDataBlock(buffer,header);
        // ((PUBLISHER_DATA_BLOCK_HEADER *)header)->sampling_rate = source->getSampleRate();
        // dan_set_publisher_header(stream, sizeof(PUBLISHER_DATA_BLOCK_HEADER), header);

        time_nanosec_t ts_nanos;
        ts_nanos = ts_secs_0 * 1e9 + ts_nanos_0 + i * n_samples * 1e9 / m_data->getSampleRate();
        m_parent->lock();
        dan_write_archive_datablock(m_stream, ts_nanos, 0, n_samples, buffer);
        dan_commit_datablock(m_stream, 0);
        m_parent->unlock();

        // std::cout << "..... " << ((DAN_CLT_DATA_BLOCK_HEADER *)m_stream->p_header_buffer)->start_sample_id << "\n";

    }
}








} // dancc
#endif // DANCC_ARCHIVE_H
