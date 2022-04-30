// 共享指针的简易实现
#include <iostream>

#include "shared_ptr.h"

int main() {
    my_smart_pointer::shared_ptr<std::string> ptr1(new std::string("hello"));
    std::cout << ptr1.get_use_count() << std::endl;

    decltype(ptr1) ptr2(ptr1);
    std::cout << ptr1.get_use_count() << std::endl;

    ptr1.~shared_ptr();
    std::cout << ptr1.get_use_count() << std::endl;

    ptr2.~shared_ptr();
    return 0;
}