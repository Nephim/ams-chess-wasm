#ifndef TOUCHGFX_PARTITION_HPP
#define TOUCHGFX_PARTITION_HPP

#include <touchgfx/hal/Types.hpp>
#include <common/Meta.hpp>

namespace touchgfx
{

class AbstractPartition
{
public:
    virtual ~AbstractPartition() {}
    virtual void* allocate(uint16_t size) = 0;
    virtual void clear() = 0;

    template <typename T>
    void* allocate()
    {
        return allocate(static_cast<uint16_t>(sizeof(T)));
    }
};

template <typename ListOfTypes, uint16_t NUMBER_OF_ELEMENTS>
class Partition : public AbstractPartition
{
public:
    typedef typename meta::select_type_maxsize<ListOfTypes>::type MaxType;

    Partition() {}
    virtual ~Partition() {}

    virtual void* allocate(uint16_t /*size*/) override
    {
        return &storage[0];
    }

    virtual void clear() override {}

private:
    alignas(MaxType) uint8_t storage[NUMBER_OF_ELEMENTS * sizeof(MaxType)];
};

} // namespace touchgfx

#endif // TOUCHGFX_PARTITION_HPP
