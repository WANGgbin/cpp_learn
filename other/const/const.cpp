#include<iostream>

/**
 * 介绍类相关常量
 * 1. 类对象成员常量: const type symbol;
 * 2. 类常量: static const type symbol; static 表示该成员是对象无关的.
 */

class A{
private:
    const int a_;  // 类对象成员常量 
    static const int b_ = 2; // 类常量
public:
    A(int a = 0):a_(a){}
    int get_a() {
        return a_;
    }
    int get_b() {
        return b_;
    }
};

int main() {
    A a;
    std::cout << a.get_a() << std::endl;
    std::cout << a.get_b() << std::endl;

    A a1(1);
    std::cout << a1.get_a() << std::endl;
    std::cout << a1.get_b() << std::endl;

    return 0;
}