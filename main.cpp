#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "MemoryManager.h"
#include "Object.hpp"

using namespace ManualMemory;

struct MyStruct_1B {
    MyStruct_1B () : x (1) {}
    MyStruct_1B (int x) : x (x) {}
    std::uint8_t x;
    void Print () const { std::cout << "x: " << (int) x << std::endl; }
};

struct MyStruct_2B {
    MyStruct_2B () : x (1), y (2) {}
    MyStruct_2B (int x, int y) : x (x), y (y) {}
    std::uint8_t x;
    std::uint8_t y;
    void Print () const { std::cout << "x: " << (int) x << " y: " << (int) y << std::endl; }
};

struct MyStruct_3B {
    MyStruct_3B () : x (1), y (2), z (3) {}
    MyStruct_3B (int x, int y, int z) : x (x), y (y), z (z) {}
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t z;
    void Print () const { std::cout << "x: " << (int) x << " y: " << (int) y << " z: " << (int) z << std::endl; }
};

struct MyStruct_8B {
    MyStruct_8B () {}
    MyStruct_8B (int a) { arr[0] = a; }
    std::array<std::uint8_t, 8> arr;
    void Print () const { std::cout << (int) arr[0] << std::endl; }
};

struct MyObject {
    int x;
    int y;
    int z;
    MyObject (int x, int y, int z) : x (x), y (y), z (z) {}
    MyObject () : x (1), y (2), z (3) {}
    void Print () const { std::cout << " x: " << (int) x << " y: " << (int) y << " z: " << (int) z << std::endl; }
};

struct NestedObject {
    Object<MyObject> obj;
    int x;
    int y;
    NestedObject (int x, int y, int z, int x1, int y1) : obj (x, y, z), x (x1), y (y1) {};
    NestedObject () : obj () {};
    void Print () const { std::cout << "x1: " << x << " y1 " << y; obj->Print (); }
};

int main () {
    MemoryManager& instance = MemoryManager::GetInstance ();
    {
        Object<MyStruct_8B> a {1};
        a->Print ();
        Object<MyStruct_8B> b {2};
        b->Print ();
        Object<MyStruct_8B> c {3};
        c->Print ();
        instance.PrintMemory ();
    }
    std::cout << "---------------------------" << std::endl;
    {
        Object<NestedObject> a {3,4,5,1,2};
        a->Print ();
        instance.PrintMemory ();
    }
    instance.PrintDescriptors ();

    /*Object<MyStruct_2B> a {1, 2};
    Object<MyStruct_2B> b (3, 4);
    instance.PrintDescriptors ();

    Object<MyStruct_3B> c {5, 6, 7};
    instance.PrintDescriptors ();

    Object<MyStruct_1B> d {8};
    instance.PrintDescriptors ();

    a->Print ();
    b->Print ();
    c->Print ();
    d->Print ();

    instance.PrintMemory ();*/
}