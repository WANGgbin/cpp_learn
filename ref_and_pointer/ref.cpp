#include<iostream>
using namespace std;

// 引用本质上就是指针，考虑到使用指针容易出现各种错误，编译器对指针进行一层封装。
// 尽量使用引用。

// int main() {
//     int tmp = 0;
//     int& ref = tmp;

//     // 注意：引用变量的地址与被引用的变量地址是相同的。
//     std::cout << "address of tmp: " << &tmp << std::endl;
//     std::cout << "address of ref: " << &ref << std::endl;

//     return 0;
// }


// 临时变量
double cube(const double&);

int main() {
    double side = 3.0;
    long edge = 5L;

    double c1 = cube(5.0);
    cout << c1 << endl;
    return 0;
}

double cube(const double& ra) {
    return ra * ra * ra;
}

// 函数参数引用的隐式转化
// 条件
    // 1. 常量引用
    // 2. 生成临时变量
        // 1. 类型相同，但是是右值（常量、表达式）
        // 2. 左值，但是类型不同，可以进行类型转化
// 原因
    // 之所以必须是常量引用，是因为这里会生成一个右值（临时变量），对临时变量的修改是没有任何意义的 所以禁止将右值赋值给
    // 非常量引用。


// 引用作为函数返回值
// 为什么要使用引用作为函数返回值
    // 1. 性能考虑
    // 2. 值返回，因为是临时变量，所以只能是右值，如果是引用返回可以作为左值
// 需要注意什么问题
    // 注意不能引用函数作用域的变量。最常见的使用方式是：返回作为参数传递给函数的引用
// 返回值什么时候使用 const
    // 不希望被改变


// 什么时候必须使用指针
// 传递数组到函数的时候，只能使用指针！