#include <thread>
#include <vector>
#include <atomic>

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

TEST(MySharedPtrTest, ConcurrencyReferenceCounting) {
    constexpr int num_threads = 16;
    constexpr int iterations = 10000;
    myshared_ptr<Dummy> shared(new Dummy(123));
    std::atomic<int> ready{0};
    std::atomic<int> go{0};
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&]() {
            ++ready;
            while (!go.load()) {}
            for (int j = 0; j < iterations; ++j) {
                myshared_ptr<Dummy> local = shared;
                EXPECT_EQ(local->value, 123);
            }
        });
    }
    while (ready.load() < num_threads) {}
    go = 1;
    for (auto& t : threads) t.join();
    EXPECT_EQ(shared.use_count(), 1);
}

TEST(MySharedPtrTest, ConcurrencyResetAndAssign) {
    constexpr int num_threads = 8;
    constexpr int iterations = 1000;
    std::vector<myshared_ptr<Dummy>> ptrs(num_threads);
    myshared_ptr<Dummy> base(new Dummy(77));
    std::atomic<int> ready{0};
    std::atomic<int> go{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, i]() {
            ++ready;
            while (!go.load()) {}
            for (int j = 0; j < iterations; ++j) {
                ptrs[i] = base;
                ptrs[i].reset();
            }
        });
    }
    while (ready.load() < num_threads) {}
    go = 1;
    for (auto& t : threads) t.join();
    EXPECT_EQ(base.use_count(), 1);
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
