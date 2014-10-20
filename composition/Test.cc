#include <iostream>

class Base
{
};

template <class T>
class Singleton
{
public:
    static T& getInstance();

protected:
    Singleton() {
        std::cout << "Singleton::ctor" << std::endl;
    }

private:
    // copy constructor
    Singleton(const Singleton&) = delete;

    // assignment operator (copy)
    Singleton& operator=(const Singleton&) = delete;

    Singleton(Singleton&&) = delete;

    Singleton& operator=(Singleton&&) = delete;
};

template<class T> T&
Singleton<T>::getInstance()
{
    static T instance;
    return instance;
}

class Layer1 : public Singleton<Layer1>, public Base
{
public:
    void print() {
        std::cout << "This is Layer1." << std::endl;
    }

protected:
    friend class Singleton<Layer1>;

    Layer1() {
        std::cout << "Layer1::ctor" << std::endl;
    }
};


class Layer2 : public Singleton<Layer2>, public Base
{
public:
    void print() {
        std::cout << "This is Layer2." << std::endl;
        layer1.print();
    }

    Layer1& getLayer1() { return layer1; }

protected:
    friend class Singleton<Layer2>;

    Layer2() : layer1(Layer1::getInstance()) {
        std::cout << "Layer2::ctor" << std::endl;
    }

    Layer1& layer1;
};

class Layer3 : public Singleton<Layer3>, public Base
{
public:
    void print() {
        std::cout << "This is Layer3." << std::endl;
        layer2.print();
    }

    Layer2& getLayer2() { return layer2; }

protected:
    friend class Singleton<Layer3>;

    Layer3() : layer2(Layer2::getInstance()) {
        std::cout << "Layer3::ctor" << std::endl;
    }

    Layer2& layer2;
};

static Layer1 &__layer1 = Layer1::getInstance();
static Layer2 &__layer2 = Layer2::getInstance();
static Layer3 &__layer3 = Layer3::getInstance();

int
main()
{
    Layer3& l = Layer3::getInstance();
    l.print();
    return 0;
}
