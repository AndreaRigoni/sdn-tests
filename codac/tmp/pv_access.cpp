#include <string.h>
#include <iostream>
#include <stdlib.h>

#include <log-api.h>

#include "pv_access.h"


double caTimeout = DEFAULT_TIMEOUT;


struct PVBase::pv_t {
    char* name;
    chid  id;
    size_t size;
    long  dbfType;
    long  dbrType;
    unsigned long nElems;       // True length of data in value
    unsigned long reqElems;     // Requested length of data
    int status;
    void* value;
    epicsTimeStamp tsPreviousC;
    epicsTimeStamp tsPreviousS;
    char firstStampPrinted;
    char onceConnected;
};

//static void valueChangedCB(struct event_handler_args args);

static void connectionChangedCB(struct connection_handler_args args)
{
    /* Branch depending on the state */
    chid pCh = args.chid;
    switch(ca_state(pCh)) {
    case cs_conn:
        log_info("%s Connection successful\n");
        break;
    case cs_never_conn:
        log_alert("%s Cannot connect\n");
        break;
    case cs_prev_conn:
        log_alert("%s Lost connection\n");
        break;
    case cs_closed:
        log_alert("%s Connection closed\n");
        break;
    }
}

//static void valueChangedCB(struct event_handler_args args)
//{
//    if(args.status == ECA_NORMAL && args.dbr) {}
//}

////////////////////////////////////////////////////////////////////////////////
//  PVBASE  ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


PVBase::PVBase(const char *name)

{
    pv = new struct pv_t;
    pv->name = strdup(name);
    this->create_channel();

    pv->dbfType = ca_field_type(pv->id);
    pv->dbrType = dbf_type_to_DBR(pv->dbfType);
    pv->size = dbr_size[pv->dbrType];
}

PVBase::~PVBase() {
    this->clear_channel();

    free(pv->name);
    delete pv;
}

struct PVBase::PVStatus PVBase::status() const
{
    return PVStatus(pv->status);
}

chanId PVBase::id() const
{
    return pv->id;
}

void PVBase::create_channel()
{
    pv->status = 0;
    pv->status = ca_create_channel (pv->name,
                                   connectionChangedCB,
                                   pv,
                                   DEFAULT_CA_PRIORITY,
                                   &pv->id);
    if (pv->status == ECA_NORMAL)
        ca_pend_event(DEFAULT_TIMEOUT);
    else
        throw(PVStatusException(pv->status));
    //    pv->status = ca_pend_io (caTimeout);
}

void PVBase::clear_channel()
{
    ca_clear_channel(pv->id);
}




////////////////////////////////////////////////////////////////////////////////
//  PVSTATUS  //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::ostream &
operator <<(std::ostream &o, const PVBase::PVStatus &st)
{
    return o << ca_message(st.value);
}




















