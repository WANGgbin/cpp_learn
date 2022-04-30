#include<iostream>

using std::endl;
using std::cout;

class ABC {
public:
    virtual void output() = 0;
};

// 纯虚函数仍然是可以定义的！
// void ABC::output() {
//     cout << "ABC::output" << endl;
// }

// class impl1: public ABC {
// public:
//     virtual void output() {
//         ABC::output();
//     }
// };
// class impl2: public ABC {
// public:
//     virtual void output() {
//         ABC::output();
//     }
// };


// int main() {
//     impl1 obj1;
//     impl2 obj2;

//     obj1.output();
//     obj2.output();
//     return 0;
// }