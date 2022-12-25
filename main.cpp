#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>                                                                                                                                                                                                                                                                                                                                                                >
class memb_func {
public:
    memb_func(){
        std::cout << "memb_func::default_construc" << std::endl;
    }

    int plus(int a, int b) const {
        return a + b;
    }
};

template<typename T>
class test{};

// 注意指向成员函数指针的类型声明 与 普通函数指针声明的区别
// template<>
// class test<int(memb_func::*)(int, int)const> {
// public:
//     void operator()() {
//         std::cout << "member func spelization1" << std::endl;
//     }
// };

template<typename member, typename _class>
class test<member _class::*> {
public:
    void operator()() {
        std::cout << "member func spelization2" << std::endl;
    }
};

int main() {
    int i = 'a';
    std::to_string()
    return 0;
}