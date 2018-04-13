#ifndef DNACC_DATA_SOURCE_H
#define DNACC_DATA_SOURCE_H

#include "stdlib.h"
#include "string.h"
#include "math.h"
#include <vector>

#include "Core/Lockable.h"
#include <Math/ScalarArray.h>

#include "dan/common_defs.h"
#include "dan/archive/archive_api.h"


#include "dancc_metadata.h"

namespace dancc {

template < typename T >
struct cType2danItem { static const enumItemType dan_type = danItemComposite; };
#define GEN_DATA_TYPE_CONVERT(T,DT) \
    template <> struct cType2danItem<T> { static const enumItemType dan_type = DT; }

GEN_DATA_TYPE_CONVERT(char,danItemInt8);
GEN_DATA_TYPE_CONVERT(short,danItemInt16);
GEN_DATA_TYPE_CONVERT(int,danItemInt32);
GEN_DATA_TYPE_CONVERT(long,danItemInt64);
GEN_DATA_TYPE_CONVERT(float,danItemFloat);
GEN_DATA_TYPE_CONVERT(double,danItemDouble);
GEN_DATA_TYPE_CONVERT(unsigned char,danItemUint8);
GEN_DATA_TYPE_CONVERT(unsigned short,danItemUint16);
GEN_DATA_TYPE_CONVERT(unsigned int,danItemUint32);
GEN_DATA_TYPE_CONVERT(unsigned long,danItemUint64);
#undef GEN_DATA_TYPE_CONVERT



template < typename T >
static size_t initSine(void *data, size_t size, double period) {
    T *t_data = (T*)data;
    size_t len = size / sizeof(T);
    for (size_t i=0; i<len; ++i)
        t_data[i] = static_cast<T>(sin(static_cast<double>(i)/len*2*M_PI/period));
    return len;
}






template < typename T, unsigned int _Dim = 1 >
class D0WAVE {
    typedef ScalarArray<T,_Dim> VectorType;

    std::vector<VectorType> m_data;
    channel_info            m_chinfo[_Dim];

public:

    D0WAVE() {
        init_chinfo();
    }
    D0WAVE(const size_t size) {
                m_data.resize(size);
                init_chinfo();
    }

    D0WAVE & operator << (const VectorType &el) {        
        m_data.push_back(el);
        return *this;
    }

    VectorType & operator ()(size_t id) { return m_data.at(id); }
    const VectorType & operator ()(size_t id) const { return m_data.at(id); }

    const T *data(int i) const { return (const T*)m_data.front(); }

    void setInfo(size_t id, const channel_info &info) {
        m_chinfo[id] = info;
    }

    channel_info &info(size_t id = 0) { return m_chinfo[id]; }
    const channel_info &info(size_t id = 0) const { return m_chinfo[id]; }

    size_t size() const { return m_data.size(); }
    size_t dim() const { return _Dim; }

private:
    void init_chinfo() {
        for (size_t i=0; i<dim(); ++i) init_channel_info(&m_chinfo[i]);
    }
};







////////////////////////////////////////////////////////////////////////////////
//  DataSource  ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class DataSource
{
    typedef SOURCE_STATIC_CONFIG        SourceInfo;
    typedef PUBLISHER_STATIC_CONFIG     PublisherInfo;
    typedef PUBLISHER_DATA_BLOCK_HEADER PublisherHeader;


    PublisherHeader data_header;
    StreamMetadata m_meta;

    void * data_buffer;
    size_t data_buffer_size;
    size_t data_item_size;


public:
    DataSource(const char *name) {
        SAFE_STRCPY(publisherInfo().source_id, name);
        SAFE_STRCPY(publisherInfo().publisher_version, "0.1");
        data_buffer = NULL;
        data_header.sampling_rate = 0;
        data_item_size = 0;
    }

    ~DataSource() {}

    const StreamMetadata & meta() const { return m_meta; }

    SourceInfo &    sourceInfo() { return m_meta.sourceInfo(); }
    PublisherInfo & publisherInfo() { return m_meta.publisherInfo(); }

    PublisherHeader *getPublisherHeader() { return &data_header; }

    size_t getDataBlock(const void *&data,const void *&header ) const {
        data = data_buffer;
        header = &data_header;
        return data_buffer_size/data_item_size;
    }

    void setSampleRate(double rate) { data_header.sampling_rate = rate; }
    double getSampleRate() const { return data_header.sampling_rate; }

    void initDataBlock(size_t size);

    template<typename T>
    void initDataBlock(const std::vector<T> &data, size_t size);

    template<typename T, unsigned int _Dim >
    void initDataBlock(const D0WAVE<T,_Dim> &data, size_t size);

};




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

template<typename T>
void DataSource::initDataBlock(const std::vector<T> &data, size_t size = 0) {
    size_t data_size = data.size() * sizeof(T);
    if (data_size > size) size = data_size;

    this->initDataBlock(size);
    data_item_size = sizeof(T);

    SAFE_STRCPY(sourceInfo().source_type,"std_vector");
    SAFE_STRCPY(sourceInfo().source_version,"0.1");
    SAFE_STRCPY(sourceInfo().source_serial_number,"201803xx");

    sourceInfo().item_type = cType2danItem<T>::dan_type;
    sourceInfo().stream_type = streamTypeD0WAVE;
    sourceInfo().in_sample_meta_header_len = 0;
    sourceInfo().in_sample_meta_footer_len = size - data_size;
    memcpy(data_buffer,&data[0],size);

    data_header.dim1 = 1;
    data_header.dim2 = 1;
    data_header.operational_mode = 0;
    data_header.reserve = 0;

    // CHANNEL INFO //
    channel_info info;
    memset(&info, 0, sizeof(info));
    //
    info.source_channel_number = 1;
    SAFE_STRCPY(info.pv_name, "TEST_VECTOR");
    SAFE_STRCPY(info.label, "FROM_DUMMY_DATASOURCE");
    SAFE_STRCPY(info.stream_unit, "a.u.");
    //
    dan_metadata_cash_set_data(m_meta, metaTypeStdChannelInfo,
                               sizeof(info), &info);
}

template<typename T, unsigned int D>
void DataSource::initDataBlock(const D0WAVE<T,D> &data, size_t size = 0) {
    size_t data_size = data.size() * D * sizeof(T);
    if (data_size > size) size = data_size;

    this->initDataBlock(size);
    data_item_size = D * sizeof(T);
    data_header.sampling_rate = (double)size / sizeof(T); // one block per second //

    SAFE_STRCPY(sourceInfo().source_type,"D0WAVE");
    SAFE_STRCPY(sourceInfo().source_version,"0.1");
    SAFE_STRCPY(sourceInfo().source_serial_number,"201803xx");

    sourceInfo().item_type = cType2danItem<T>::dan_type;
    sourceInfo().stream_type = streamTypeD0WAVE;
    sourceInfo().in_sample_meta_header_len = 0;
    sourceInfo().in_sample_meta_footer_len = 0;

    for(size_t i=0; i<data.size(); ++i)
        memcpy( ((T*)data_buffer+i*D), (const T*)data(i), D*sizeof(T));

    data_header.dim1 = 1;
    data_header.dim2 = 1;
    data_header.operational_mode = 0;
    data_header.reserve = 0;

    // CHANNEL INFO //
    channel_info info[D];
    for(size_t i=0; i<D; ++i) {
        info[i] = data.info(i);
        info[i].source_channel_number = i;
        //        sprintf(name,"TEST_VECTOR_%d",(int)i);
        //        SAFE_STRCPY(info[i].pv_name, name);
        //        SAFE_STRCPY(info[i].label, "FROM_DUMMY_DATASOURCE");
        //        SAFE_STRCPY(info[i].stream_unit, "a.u.");
    }
    //
    dan_metadata_cash_set_data(m_meta, metaTypeStdChannelInfo,
                               D * sizeof(channel_info),
                               &info[0]);

}




} // dancc
#endif // DNACC_DATA_SOURCE_H
