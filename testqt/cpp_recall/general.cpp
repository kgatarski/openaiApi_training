#include <iostream>
#include <utility>
#include <vector>
#include <array>

using namespace std;

void vectorfunc(void)
{
    std::vector<int> intvec = {1,2,3,4,5};
    intvec.push_back(intvec.size() + 1);

    std::cout << "vectorfunc" << std::endl;

    for (int a : intvec)
    {
        std::cout << a << ", " << std::endl;
    }

    std::cout << "ctab and arrray" << std::endl;
    int ctab[10] = {1,2,5,6,7,8,9,10, 91 ,11};

    std::array<int, 10> arr;
    std::copy(std::begin(ctab), std::end(ctab), arr.begin());

    arr[0] = 10;

    // for (int i=0; i<arr.max_size(); ++i)
    // {
    //     arr.at(i) = i*10;
    // }

    for (auto elem: arr)
    {
        std::cout << elem << ", " << "\n";
    }
    
}

void lambda_tests(void)
{
    // generic lambda, operator() is a template with two parameters
    auto glambda = [](auto a, auto&& b) { return a < b; };
    bool b = glambda(3, 3.14); // OK

    // generic lambda, operator() is a template with one parameter
    auto vglambda = [](auto printer)
    {
        return [=](auto&&... ts) // generic lambda, ts is a parameter pack
        {
            printer(std::forward<decltype(ts)>(ts)...);
            // nullary lambda (takes no parameters):
            return [=] (){ printer(ts...); };
        };
    };

    auto realPrinter = [](auto v1, auto v2, auto v3)
    {
        std::cout << v1 << ", " << v2 << ", " << v3 << endl;;
        return 1;
    };

    auto newAutoPrinter = [] (auto&&... ts) {
        (
            (
                cout << std::forward<decltype(ts)>(ts) << " "
            ), ...
        );
        cout << endl;
        return 1;
    };

    // auto autoPrinter = [] (auto&&... ts) {
    //     //(cout << ... << (std::forward<decltype(ts)>(ts) << " "));
    //     (
    //         (
    //             std::cout << std::forward<decltype(ts)>(ts) << " "
    //         ), ...
    //     );
    //     cout << endl;
    //     return 1;
    // };

    auto p = vglambda(realPrinter);

    auto ap = vglambda(newAutoPrinter);

    struct Point { int x; int y; };
    Point p1{};       // x=0, y=0
    Point p2{3};      // x=3, y=0
    Point p3{3, 4};   // x=3, y=4

    auto aq = ap(1, 'a', 3.45, 44);

    aq();

    auto q = p(1, 'a', 3.14); // outputs 1a3.14

    q();

    auto genericFunc = [](auto&& tt)
    {
        tt();
    };

    cout << "as an arg to func " << endl;
    genericFunc(q);

    vectorfunc();

    cout << "\n\n" << endl << endl;
}