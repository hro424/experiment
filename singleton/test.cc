#include <iostream>

template <class T>
class BaseSingleton
{
public:
    static T& getInstance();

protected:
    BaseSingleton() {
        std::cout << "BaseSingleton::ctor" << std::endl;
    }

private:
    BaseSingleton(const BaseSingleton&) = delete;
    BaseSingleton& operator=(const BaseSingleton&) = delete;
    BaseSingleton(BaseSingleton&&) = delete;
    BaseSingleton& operator=(BaseSingleton&&) = delete;
};

template<class T> T&
BaseSingleton<T>::getInstance()
{
    static T instance;
    return instance;
}

class DerivedSingleton : public BaseSingleton<DerivedSingleton>
{
public:
    virtual void print() {
        std::cout << "This is DerivedSingleton." << std::endl;
    }

protected:
    friend class BaseSingleton<DerivedSingleton>;

    DerivedSingleton() {
        std::cout << "DerivedSingleton::ctor" << std::endl;
    }
};

static DerivedSingleton &ds = DerivedSingleton::getInstance();

class MoreDerivedSingleton : public BaseSingleton<MoreDerivedSingleton>,
    public DerivedSingleton
{
public:
    virtual void print() {
        std::cout << "This is MoreDerivedSingleton." << std::endl;
    }

protected:
    MoreDerivedSingleton() {
        std::cout << "MoreDerivedSingleton::ctor" << std::endl;
    }
};

static DerivedSingleton &mds = MoreDerivedSingleton::getInstance();

int
main()
{
    DerivedSingleton& obj1 = DerivedSingleton::getInstance();
    DerivedSingleton& obj2 = MoreDerivedSingleton::getInstance();

    obj1.print();
    obj2.print();
}
