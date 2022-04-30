// 学习 c++ 中的智能模板指针的使用


// 1.使用

#include <iostream>
#include <memory>
#include <string>

using std::endl;
using std::cout;
using std::shared_ptr;

int main() {
    std::shared_ptr<std::string> ptr(new std::string("name"));
    ptr.~shared_ptr();

    std::shared_ptr<std::string> ptr1(new std::string("name"), [](std::string* ptr){
        delete ptr;
    });
    ptr1.~shared_ptr();
    return 0;
}

// 2.如何实现
//  make_xx 系列函数工作原理
// 3.注意问题

