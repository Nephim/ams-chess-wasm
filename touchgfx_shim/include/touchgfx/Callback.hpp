#ifndef TOUCHGFX_CALLBACK_HPP
#define TOUCHGFX_CALLBACK_HPP

namespace touchgfx
{

template <class T1 = void, class T2 = void, class T3 = void>
class GenericCallback
{
public:
    virtual ~GenericCallback() {}
    virtual void execute(T1 val1, T2 val2, T3 val3) = 0;
    virtual bool isValid() const = 0;
};

template <class T1, class T2>
class GenericCallback<T1, T2, void>
{
public:
    virtual ~GenericCallback() {}
    virtual void execute(T1 val1, T2 val2) = 0;
    virtual bool isValid() const = 0;
};

template <class T1>
class GenericCallback<T1, void, void>
{
public:
    virtual ~GenericCallback() {}
    virtual void execute(T1 val1) = 0;
    virtual bool isValid() const = 0;
};

template <>
class GenericCallback<void, void, void>
{
public:
    virtual ~GenericCallback() {}
    virtual void execute() = 0;
    virtual bool isValid() const = 0;
};

template <class dest_type, typename T1 = void, typename T2 = void, typename T3 = void>
class Callback : public GenericCallback<T1, T2, T3>
{
public:
    Callback() : pobject(nullptr), pmemfun_3(nullptr) {}
    Callback(dest_type* pobject, void (dest_type::*pmemfun_3)(T1, T2, T3))
        : pobject(pobject), pmemfun_3(pmemfun_3) {}

    virtual void execute(T1 t1, T2 t2, T3 t3) override
    {
        if (pobject && pmemfun_3)
        {
            (pobject->*pmemfun_3)(t1, t2, t3);
        }
    }

    virtual bool isValid() const override
    {
        return (pobject != nullptr) && (pmemfun_3 != nullptr);
    }

private:
    dest_type* pobject;
    void (dest_type::*pmemfun_3)(T1, T2, T3);
};

template <class dest_type, typename T1, typename T2>
class Callback<dest_type, T1, T2, void> : public GenericCallback<T1, T2>
{
public:
    Callback() : pobject(nullptr), pmemfun_2(nullptr) {}
    Callback(dest_type* pobject, void (dest_type::*pmemfun_2)(T1, T2))
        : pobject(pobject), pmemfun_2(pmemfun_2) {}

    virtual void execute(T1 t1, T2 t2) override
    {
        if (pobject && pmemfun_2)
        {
            (pobject->*pmemfun_2)(t1, t2);
        }
    }

    virtual bool isValid() const override
    {
        return (pobject != nullptr) && (pmemfun_2 != nullptr);
    }

private:
    dest_type* pobject;
    void (dest_type::*pmemfun_2)(T1, T2);
};

template <class dest_type, typename T1>
class Callback<dest_type, T1, void, void> : public GenericCallback<T1>
{
public:
    Callback() : pobject(nullptr), pmemfun_1(nullptr) {}
    Callback(dest_type* pobject, void (dest_type::*pmemfun_1)(T1))
        : pobject(pobject), pmemfun_1(pmemfun_1) {}

    virtual void execute(T1 t1) override
    {
        if (pobject && pmemfun_1)
        {
            (pobject->*pmemfun_1)(t1);
        }
    }

    virtual bool isValid() const override
    {
        return (pobject != nullptr) && (pmemfun_1 != nullptr);
    }

private:
    dest_type* pobject;
    void (dest_type::*pmemfun_1)(T1);
};

template <class dest_type>
class Callback<dest_type, void, void, void> : public GenericCallback<>
{
public:
    Callback() : pobject(nullptr), pmemfun_0(nullptr) {}
    Callback(dest_type* pobject, void (dest_type::*pmemfun_0)())
        : pobject(pobject), pmemfun_0(pmemfun_0) {}

    virtual void execute() override
    {
        if (pobject && pmemfun_0)
        {
            (pobject->*pmemfun_0)();
        }
    }

    virtual bool isValid() const override
    {
        return (pobject != nullptr) && (pmemfun_0 != nullptr);
    }

private:
    dest_type* pobject;
    void (dest_type::*pmemfun_0)();
};

} // namespace touchgfx

#endif // TOUCHGFX_CALLBACK_HPP
