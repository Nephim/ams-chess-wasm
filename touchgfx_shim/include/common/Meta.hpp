#ifndef TOUCHGFX_META_HPP
#define TOUCHGFX_META_HPP

namespace touchgfx
{
namespace meta
{

struct Nil {};

template <typename First, typename Next>
struct TypeList
{
    typedef First first;
    typedef Next next;
};

template <typename T1, typename T2, bool choose1 = (sizeof(T1) >= sizeof(T2))>
struct type_max
{
    typedef T1 type;
};

template <typename T1, typename T2>
struct type_max<T1, T2, false>
{
    typedef T2 type;
};

template <typename T>
struct select_type_maxsize;

template <typename First, typename Next>
struct select_type_maxsize< TypeList<First, Next> >
{
    typedef typename type_max<First, typename select_type_maxsize<Next>::type>::type type;
};

template <typename First>
struct select_type_maxsize< TypeList<First, Nil> >
{
    typedef First type;
};

template <>
struct select_type_maxsize< TypeList<Nil, Nil> >
{
    typedef Nil type;
};

template <>
struct select_type_maxsize< Nil >
{
    typedef Nil type;
};

} // namespace meta
} // namespace touchgfx

#endif // TOUCHGFX_META_HPP
