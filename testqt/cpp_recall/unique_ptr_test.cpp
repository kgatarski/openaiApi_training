#include "myunique_ptr.h"

class Base {
    public: 
        Base() {
            cout << "Base()\n";
        }
        virtual ~Base() {
            cout << "~Base()\n";
        }
};

void func(myunique_ptr<Base>&& p) {
    cout << "inside func " << p.get() << endl;
}

void myunique_ptr_test() {
    myunique_ptr<Base> ub = new Base();
    func(std::move(ub));
}