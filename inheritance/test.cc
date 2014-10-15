#include <iostream>

class Base
{
public:
    virtual ~Base() { std::cout << "Base destructor" << std::endl; }

    void func() { std::cout << "Base func()" << std::endl; }
    virtual void vfunc() {
        std::cout << "Base virtual func()" << std::endl;
    }

    void func2() { std::cout << "Base func2()" << std::endl; }

    void print_value() { std::cout << "value=" << value << std::endl; }
    virtual void vprint_value() { std::cout << "value=" << value << std::endl; }

    virtual void vprint_value2() {
        std::cout << "value2=" << value2 << std::endl;
    }

protected:
    Base() : value(1), value2(10) {
        std::cout << "Base constructor" << std::endl;
    }

    int value;
    int value2;
};

class Derived : public Base
{
public:
    Derived() {
        value = 2;
        value2 = 20;
        std::cout << "Derived constructor" << std::endl;
    }

    ~Derived() { std::cout << "Derived destructor" << std::endl; }

    void func() { std::cout << "Derived func()" << std::endl; }
    virtual void vfunc() {
        std::cout << "Derived virtual func(): " << value << std::endl;
        std::cout << "Base value: " << Base::value << std::endl;
    }

    virtual void vprint_value() {
        std::cout << "Derived value=" << value << std::endl;
    }

    virtual void vprint_value2() {
        std::cout << "Derived value2=" << value2 << std::endl;
        std::cout << "Base value2=" << Base::value2 << std::endl;
    }

protected:
    int value2;
};

int
main()
{
    Base* b;

    /*
    b = new Base();
    b->func();
    b->vfunc();
    b->func2();
    delete b;
    */

    b = new Derived();
    b->func();
    b->vfunc();
    b->func2();
    b->print_value();
    b->vprint_value();
    b->vprint_value2();
    delete b;

    return 0;
}
