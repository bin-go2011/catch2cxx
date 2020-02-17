#define CATCH_CONFIG_MAIN

#include <catch.hpp>

class ZooAnimal {
public:
    ZooAnimal() {}
    ZooAnimal(std::string name) : name(name) {}
    virtual ~ZooAnimal() {}

    virtual void rotate() {}
protected:
    int loc;
    std::string name;
};

class Bear : public ZooAnimal {
public:
    Bear() {}
    ~Bear() {}

    void rotate() {} 
    virtual void dance() {}
protected:
    int cell_block;
};

TEST_CASE("memory layout", "[object]") {
    SECTION("Base class") {
        ZooAnimal za("Zoey");
        ZooAnimal *pza = &za;
    }

    SECTION("Add Polymorphism") {
        Bear b;
        ZooAnimal *pz = &b;
        Bear *pb = &b;
    }
}