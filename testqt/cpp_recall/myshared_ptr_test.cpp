#include <gtest/gtest.h>
#include "myshared_ptr.h"

class Dummy {
public:
    Dummy(int v = 0) : value(v) {}
    int value;
};

TEST(MySharedPtrTest, BasicOwnership) {
    myshared_ptr<Dummy> ptr1(new Dummy(42));
    EXPECT_EQ(ptr1->value, 42);
    EXPECT_EQ(ptr1.use_count(), 1);

    myshared_ptr<Dummy> ptr2 = ptr1;
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_EQ(ptr2->value, 42);

    ptr1.reset();
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(ptr1.get(), nullptr);
}

TEST(MySharedPtrTest, MoveSemantics) {
    myshared_ptr<Dummy> ptr1(new Dummy(7));
    myshared_ptr<Dummy> ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr2->value, 7);
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr2.use_count(), 1);
}

TEST(MySharedPtrTest, AssignmentAndReset) {
    myshared_ptr<Dummy> ptr1(new Dummy(1));
    myshared_ptr<Dummy> ptr2;
    ptr2 = ptr1;
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    ptr2.reset();
    EXPECT_EQ(ptr1.use_count(), 1);
    EXPECT_EQ(ptr2.get(), nullptr);
}
