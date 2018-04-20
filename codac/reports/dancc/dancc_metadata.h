#ifndef DANCC_METADATA_H
#define DANCC_METADATA_H

#include <dan/archive/archive_api.h>

#include <Core/unique_ptr.h>

namespace dancc {

////////////////////////////////////////////////////////////////////////////////
//  METADATA  //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//typedef enum
//{
//    metaTypeSourceStaticConfig,
//    metaTypeStaticConfig,
//    //???	metaTypeStreamConfig,
//    metaTypeItemFields,
//    metaTypeUserHeaderFields,
//    metaTypeStdChannelInfo,
//    metaTypeUserChannelInfoFields,
//    metaTypeUserChannelInfo,
//    metaTypeUserAttributes,
//    metaTypeMarkerFields,
//    // Max transfer rate, max block size, etc
//    metaTypeStreamOptimize,
//    metaTypeCalibration,
//    metaTypeSubscriber,
//    // Instead of chunk
//    metaTypeDebug,
//    metaTypeReserve0,
//    metaTypeReserve1,
//    // Could be extended in future
//    metaTypeLast5_0,
//    metaTypeLast = metaTypeLast5_0,
//} enumMetaDataType;

template < class T > class convertMeta {
    typedef T cType;
    static enumMetaDataType mType;
    static buf_t size;
    static T * getInfo(const dan_MetadataCash *meta) { return 0; }
};
#   define _convertMetaDataType(cT,mT,init) \
    template <> class convertMeta<cT> { public: \
    typedef cT cType; \
    static const enumMetaDataType mType = mT; \
    static const buf_t size = sizeof(cT); \
    static cT *getInfo(dan_MetadataCash *meta) \
    { buf_t s; void *out; dan_metadata_cash_get_data(meta,mT,&s,&out); \
      if(!out) { \
       cT info; init(&info); \
       dan_metadata_cash_set_data(meta, mT, size, &info); \
       return getInfo(meta); \
      } \
      return (cT*)out; } \
     }
_convertMetaDataType(SOURCE_STATIC_CONFIG, metaTypeSourceStaticConfig, init_source_static_config);
_convertMetaDataType(PUBLISHER_STATIC_CONFIG, metaTypeStaticConfig, init_publisher_static_config);
_convertMetaDataType(SUBSCRIBER_STATIC_CONFIG, metaTypeSubscriber, init_subscriber_static_config);
_convertMetaDataType(STREAM_OPTIMIZATION, metaTypeStreamOptimize, init_stream_optimization);
_convertMetaDataType(STREAM_DEBUG, metaTypeDebug, init_stream_debug);
// FINIRE //
#undef _convertMetaDataType

class StreamMetadata
{

    typedef PUBLISHER_STATIC_CONFIG PublisherInfo;
    typedef SOURCE_STATIC_CONFIG    SourceInfo;
    typedef channel_info            ChannelInfo;

    unique_ptr<dan_MetadataCash> m_meta;

public:

    StreamMetadata() : m_meta(new dan_MetadataCash)
    {
        dan_metadata_cash_init(m_meta);
        convertMeta<PUBLISHER_STATIC_CONFIG>::getInfo(m_meta);
        convertMeta<SUBSCRIBER_STATIC_CONFIG>::getInfo(m_meta);
        convertMeta<STREAM_OPTIMIZATION>::getInfo(m_meta);
        convertMeta<STREAM_DEBUG>::getInfo(m_meta);

    }

    operator dan_MetadataCash *() { return m_meta; }
    operator const dan_MetadataCash *() const { return m_meta; }

    template < class T >
    err_t setInfo(const T &info) {
        typedef convertMeta<T> type;
        return dan_metadata_cash_set_data(m_meta, type::mType, type::size, &info);
    }

    template < class T >
    T * getInfo() { return convertMeta<T>::getInfo(m_meta); }

    PublisherInfo & publisherInfo() { return *convertMeta<PublisherInfo>::getInfo(m_meta); }
    SourceInfo &    sourceInfo() { return *convertMeta<SourceInfo>::getInfo(m_meta); }



};







} // dancc



#endif // DANCC_METADATA_H
