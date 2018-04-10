#ifndef DANCC_METADATA_H
#define DANCC_METADATA_H

#include <dan/archive/archive_api.h>

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
#   define _convertMetaDataType(cT,mT) \
    template <> class convertMetaDataType<cT> { public: \
    typedef cT cType; \
    static const enumMetaDataType mType = mT; \
    static const buf_t size = sizeof(cT); \
    static cT *getInfo(const dan_MetadataCash *meta) \
    { buf_t s; void *out; dan_metadata_cash_get_data(meta,mT,&s,&out); return (cT*)out;} \
     }
_convertMetaDataType(SOURCE_STATIC_CONFIG, metaTypeSourceStaticConfig);
_convertMetaDataType(PUBLISHER_STATIC_CONFIG, metaTypeStaticConfig);
_convertMetaDataType(SUBSCRIBER_STATIC_CONFIG, metaTypeSubscriber);
// FINIRE //
#undef _convertMetaDataType

class StreamMetadata
{

    typedef PUBLISHER_STATIC_CONFIG PublisherInfo;
    typedef SOURCE_STATIC_CONFIG    SourceInfo;
    typedef channel_info            ChannelInfo;

    dan_MetadataCash *meta;

public:

    StreamMetadata() {
        dan_metadata_cash_init(meta);

        PUBLISHER_STATIC_CONFIG pbl;
        init_publisher_static_config(&pbl);
        setInfo(pbl);

        setInfo(SUBSCRIBER_STATIC_CONFIG());

    }

    template < class T >
    err_t setInfo(const T &info) {
        typedef convertMeta<T> type;
        return dan_metadata_cash_set_data(meta, type::mType, type::size, &info);
    }

    PublisherInfo & publisherInfo() { return *convertMeta<PublisherInfo>::getInfo(meta); }
    SourceInfo &    sourceInfo() { return *convertMeta<PublisherInfo>::getInfo(meta); }

};







} // dancc



#endif // DANCC_METADATA_H
