#include <QCoreApplication>
#include <iostream>
#include <utility>
#include "myshared_ptr.h"
#include <memory>

using namespace std;

extern void vectorfunc(void);
extern int shared_ptr_test_1();
extern int shared_ptr_test_2();
extern void myunique_ptr_test();
extern int weak_ptr_test();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //shared_ptr_test_1();
    myunique_ptr_test();
    weak_ptr_test();

    return 0;
}
