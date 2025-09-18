#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include "myshared_ptr.h"

using namespace std::chrono_literals;

class Example
{
    public:
        void print() { cout << "I am alive " << i << endl;}
    int i = 0;
};

class Person {
public:
    std::string name;
    int age;

    // Konstruktor domyślny
    Person() : name("Unknown"), age(0) {
        std::cout << "Konstruktor domyślny\n";
    }

    // Konstruktor z parametrami
    Person(const std::string& n, int a) : name(n), age(a) {
        std::cout << "Konstruktor z parametrami\n";
    }

    // Konstruktor kopiujący
    Person(const Person& other) : name(other.name), age(other.age) {
        std::cout << "Konstruktor kopiujący\n";
    }

    // Konstruktor przenoszący
    Person(Person&& other) noexcept : name(std::move(other.name)), age(other.age) {
        std::cout << "Konstruktor przenoszący\n";
    }

    ~Person() {
        std::cout << "Destruktor\n";
    }

    void operator()() {
        std::cout << "op () \n";
    }
};

auto func(myshared_ptr<Person>&& foo) {
    cout << "inside func() \n";
    return foo;
}

using namespace std::chrono;
int shared_ptr_test_2()
{
    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    //A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or use the Non-Qt Plain C++ Application template.

    //a.exec();

    cout << "hello world" << endl;

    Person *p1 = new Person("Krzysiek", 50);

    myshared_ptr<Person> example = p1;

    myshared_ptr<Person> example2;
    example2 = example;

    cout << "func()\n";

    myshared_ptr<Person> example3 = func(std::move(example2));

    Person *pptr = example.get();
    (*pptr)();
    cout << "example->age = " << example->age << endl;
    cout << "example.age = " << (*example).age << endl;

    //std::shared_ptr<Person> stdexample(p1);

    myshared_ptr<Person> per1 = std::move(example3);
    //per1->age = 10;
    return 0;
}

struct Base
{
    Base() {
        std::cout << "Base::Base()\n";
    }

    explicit Base (int x) {
        std::cout << "Base::Base(int)\n";
    }

    // Note: non-virtual destructor is OK here
    virtual ~Base() {
        std::cout << "Base::~Base()\n";
    }
};

struct Derived : public Base
{
    Derived() { std::cout << "Derived::Derived()\n"; }

    virtual ~Derived() { std::cout << "Derived::~Derived() \n"; }
};


template<typename T>
using is_chrono_duration = std::enable_if_t<std::is_base_of_v<std::chrono::duration<typename T::rep, typename T::period>, T>>;
using hr_clock = std::chrono::high_resolution_clock;
using chrono_time = std::chrono::time_point<hr_clock>;

template <typename Time_t,
          typename = is_chrono_duration<Time_t>
          >
class Timer {
    public:
        Timer() {
            starttime = endtime = hr_clock::now();
        }
        void start() {
            starttime = hr_clock::now();
        }
        uint64_t stop() {
            endtime = hr_clock::now();
            return std::chrono::duration_cast<Time_t>(endtime - starttime).count();
        }
        uint64_t getTime() {
            return std::chrono::duration_cast<Time_t>(endtime - starttime).count();
        }
    private:
    chrono_time starttime;
    chrono_time endtime;
};

using Timer_ms = Timer<std::chrono::milliseconds>;
using Timer_us = Timer<std::chrono::microseconds>;

//#define sharedPtr std::shared_ptr
#define sharedPtr myshared_ptr

//void print(std::string rem, myshared_ptr<Base>& sp, Timer_us& timer)
void print(std::string rem, sharedPtr<Base>& sp, Timer_us& timer)
{
    std::cout << rem << "\n\tget() = " << sp.get()
              << ", use_count() = " << sp.use_count()
              << ", time = " << timer.stop()
              << '\n';
}

//void thr(myshared_ptr<Base> p,  Timer_us& timer)
void thr(sharedPtr<Base> p,  Timer_us& timer)
{
    std::this_thread::sleep_for(987ms);
    sharedPtr<Base> lp = p; // thread-safe, even though the
                                  // shared use_count is incremented
    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lk(io_mutex);
        print("Local pointer in a thread:", lp, timer);
    }
}

int shared_ptr_test_1()
{
    sharedPtr<Base> p = new Derived();
    Timer_us timer;

    print("Created a shared Derived (as a pointer to Base)", p, timer);
    
    timer.start();

    std::thread t1{thr, p, std::ref(timer)}, t2{thr, p, std::ref(timer)}, t3{thr, p, std::ref(timer)};
    p.reset(); // release ownership from main

    print("Shared ownership between 3 threads and released ownership from main:", p, timer);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "All threads completed, the last one deleted Derived.\n";

    return 0;
}