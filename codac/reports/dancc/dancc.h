
#ifndef DANCC_H
#define DANCC_H

#include <iostream>

#include "dan/common_defs.h"
#include "dan/timestamp.h"
#include "dan/prof_service.h"
#include "dan/dan_log_service.h"
#include "dan/archive/archive_api.h"
#include "dan/client_api/archive_stream_api_client.h"


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



// SOURCE_STATIC_CONFIG //
static std::ostream &
operator << (std::ostream &o, const SOURCE_STATIC_CONFIG &d) {
    return o
            << "source_type ................ " << d.source_type << std::endl
            << "source_version ............. " << d.source_version << std::endl
            << "source_serial_number ....... " << d.source_serial_number << std::endl
            << "operational_mode_major ..... " << d.operational_mode_major << std::endl
            << "item_type .................. " << d.item_type << std::endl
            << "stream_type ................ " << d.stream_type << std::endl
            << "in_sample_meta_header_len .. " << d.in_sample_meta_header_len << std::endl
            << "in_sample_meta_footer_len .. " << d.in_sample_meta_footer_len << std::endl
            ;
}

static std::ostream &
operator << (std::ostream &o, const SOURCE_STATIC_CONFIG *d) {
    return o << *d;
}



// PUBLISHER_STATIC_CONFIG //
static std::ostream &
operator << (std::ostream &o, const PUBLISHER_STATIC_CONFIG &d) {
    return o
            << "publisher_version .. " << d.publisher_version << std::endl
            << "source_id .......... " << d.source_id << std::endl
            << "time_format ........ " << d.time_format << std::endl
               ;
}

static std::ostream &
operator << (std::ostream &o, const PUBLISHER_STATIC_CONFIG *d) {
    return o << *d;
}


// PUBLISHER_DATA_BLOCK_HEADER //
static std::ostream &
operator << (std::ostream &o, const PUBLISHER_DATA_BLOCK_HEADER &d) {
    return o
            << "dim1 .............. " << d.dim1 << "\n"
            << "dim2 .............. " << d.dim2 << "\n"
            << "operational_mode .. " << d.operational_mode << "\n"
            << "reserve ........... " << d.reserve << "\n"
            << "sampling_rate ..... " << d.sampling_rate << "\n"
               ;
}

static std::ostream &
operator << (std::ostream &o, const PUBLISHER_DATA_BLOCK_HEADER *d) {
    return o << *d;
}


// DAN_CLT_DATA_BLOCK_HEADER //
static std::ostream &
operator << (std::ostream &o, const DAN_CLT_DATA_BLOCK_HEADER &d) {
    return o
            << "start_sample_id: " << d.start_sample_id << "\n"
            << " [pbl_data]\n" << d.pbl_data << "\n";
}

static std::ostream &
operator << (std::ostream &o, const DAN_CLT_DATA_BLOCK_HEADER *d) {
    return o << *d;
}



// ARCHIVE_STREAM_CLIENT_GROUP //
static std::ostream &
operator << (std::ostream &o, const ARCHIVE_STREAM_CLIENT_GROUP &d) {
    return o
            << "ARCHIVE_STREAM_CLIENT_GROUP\n"
            << "name .......... " << d.name << "\n"
            << "connected ..... " << d.connected << "\n"
            << "connectionID .. " << d.connectionID << "\n"
            << "n_streams ..... " << d.n_streams << "\n"
            << "pulse_id ...... " << d.pulse_id << "\n"
            << "p_streams ..... " << d.p_streams << "\n"
               ;
}

static std::ostream &
operator << (std::ostream &o, const ARCHIVE_STREAM_CLIENT_GROUP *d) {
    return o << *d;
}



#endif // DANCC_H
