// 本文件主要介绍 “函数模板”

#include<iostream>

using std::cout;
using std::endl;


template<typename T1, typename T2>
void print(T1 obj1, T2 obj2){
    cout << "print(T1, T2)" << endl;
}

// 偏特化
template<typename T>
void print(T obj1, T obj2) {
    cout << "print(T, T)" << endl;
}

// 完全特化
template<>
void print<int>(int, int) {
    cout << "print(int, int)" << endl;
}

int main() {
    print(1, 1.0);
    print(1, 1);

    print<double>(2.0, 3.0);  // 显示实例化
    return 0;
}