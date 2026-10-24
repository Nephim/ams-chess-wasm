#ifndef TOUCHGFX_MVPHEAP_HPP
#define TOUCHGFX_MVPHEAP_HPP

#include <common/Partition.hpp>

namespace touchgfx
{

class MVPApplication;

class MVPHeap
{
public:
    MVPHeap(AbstractPartition& pres, AbstractPartition& scr, AbstractPartition& tra, MVPApplication& app)
        : presenterStorage(pres), screenStorage(scr), transitionStorage(tra), frontendApplication(app)
    {}

    virtual ~MVPHeap() {}

    AbstractPartition& presenterStorage;
    AbstractPartition& screenStorage;
    AbstractPartition& transitionStorage;
    MVPApplication& frontendApplication;
};

} // namespace touchgfx

#endif // TOUCHGFX_MVPHEAP_HPP
