#ifndef PV_ACCESS_H
#define PV_ACCESS_H

#include <cadef.h>
#include <exception>



#define DEFAULT_CA_PRIORITY 0  /* Default CA priority */
#define DEFAULT_TIMEOUT 1.0     /* Default CA timeout */



class CAContext {
    int status;
public:
    CAContext() :
        status(ca_context_create(ca_disable_preemptive_callback))
    {}
    ~CAContext() {
        ca_context_destroy();
    }
};

template < typename T >
struct dbr_type_resolve { static const chtype dbr_type = -1; };

#define _dbr_type_resolve(type, dbr_code) \
    template <> \
    struct dbr_type_resolve<type> { static const chtype dbr_type = dbr_code; }

_dbr_type_resolve(char *,DBR_STRING);
_dbr_type_resolve(bool  ,DBR_CHAR);
_dbr_type_resolve(char  ,DBR_CHAR);
_dbr_type_resolve(short ,DBR_SHORT);
_dbr_type_resolve(int   ,DBR_INT);
_dbr_type_resolve(long  ,DBR_LONG);
_dbr_type_resolve(float ,DBR_FLOAT);
_dbr_type_resolve(double,DBR_DOUBLE);

#undef _dbr_type_resolve



class PVBase {
    struct pv_t;
    struct pv_t *pv;
public:

    struct PVStatus {
        int value;
        PVStatus(const int v) : value(v) {}
        friend std::ofstream &
        operator << (std::ofstream &o, const struct PVStatus &st);
    };

    class PVStatusException : std::exception {
        PVStatus status;
      public:
        PVStatusException(const PVStatus st) : status(st) {}
        virtual const char* what() const throw() {
            return ca_message(status.value);
        }
    };

public:

    PVBase(const char *name);

    ~PVBase();

    struct PVStatus status() const;

    chanId id() const;

    void create_channel();

    void clear_channel();

    template < typename T >
    T get_data() const {
        T data;
        ca_get(dbr_type_resolve<T>::dbr_type,id(),&data);
        int status = ca_pend_io(DEFAULT_TIMEOUT);
        if(status != ECA_NORMAL) throw PVStatusException(status);
        return data;
    }

    template < typename T >
    void put_data( const T &data ) {
        ca_put(dbr_type_resolve<T>::dbr_type,id(),&data);
        int status = ca_pend_io(DEFAULT_TIMEOUT);
        if(status != ECA_NORMAL) throw PVStatusException(status);
    }

};

template < typename T >
class PV : public PVBase
{
public:
    PV(const char *name) : PVBase(name) {}
    PV & operator = (const T &data) { this->put_data<T>(data); return *this; }
    operator T () const { return this->get_data<T>(); }
};




#endif // PV_ACCESS_H
