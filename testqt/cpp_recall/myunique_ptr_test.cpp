#include <gtest/gtest.h>
#include "myunique_ptr.h"

class Dummy {
public:
    Dummy(int v = 0) : value(v) {}
    int value;
};

TEST(MyUniquePtrTest, BasicOwnership) {
    myunique_ptr<Dummy> ptr(new Dummy(42));
    EXPECT_EQ(ptr->value, 42);
    EXPECT_EQ((*ptr).value, 42);
}

TEST(MyUniquePtrTest, MoveSemantics) {
    myunique_ptr<Dummy> ptr1(new Dummy(7));
    myunique_ptr<Dummy> ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr2->value, 7);
    EXPECT_EQ(ptr1.get(), nullptr);
}

TEST(MyUniquePtrTest, Release) {
    myunique_ptr<Dummy> ptr(new Dummy(99));
    Dummy* raw = ptr.release();
    EXPECT_EQ(raw->value, 99);
    EXPECT_EQ(ptr.get(), nullptr);
    delete raw;
}
