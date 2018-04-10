#ifndef DANCC_DATA_ITEM_H
#define DANCC_DATA_ITEM_H

#include <vector>

#include "dancc.h"


namespace dancc {

template < typename T >
class D0WAVE : public std::vector<T> {
    typedef std::vector<T> BaseClass;
public:
    D0WAVE(size_t size) : BaseClass(size) {}

 // TO BE CONTINUED
};










} // dancc
#endif // DANCC_DATA_ITEM_H
