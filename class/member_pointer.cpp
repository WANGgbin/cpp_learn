// 本源文件主要介绍 类成员指针 和 类成员函数指针的概念
// 详情可以参考：https://www.cnblogs.com/qrxqrx/articles/6729576.html?ivk_sa=1024320u、

#include <iostream>
#include <cstdio>

using std::endl;
using std::cout;

class base1 {
public:
    base1(float x = 0.0, float y = 0.0){x_ = x; y_ = y;}
    virtual ~base1() {}
public:
    float x_;
    float y_;
};

class base2 {
public:
    base2(int m = 0) {m_ = m;}
    virtual ~base2() {}
public:
    int m_;
};

class point3d: public base1, public base2 {
public:
    point3d(float z = 0.0): base1(0.0, 0.0), base2(0) {z_ = z;}
    void info() {
        cout << z_ << endl;
    }
    static int get_count() {return count;}
public:
    float z_;
    static int count;
};

int point3d::count = 0;

// 基类成员指针类型 可以直接隐式转化为子类成员指针
void func(int point3d::* p, const point3d& obj) {
    printf("p: %d\n", p);  //
    cout << obj.*p << endl;
}

// 类成员指针三点
// 1. 表示的是成员在类中的偏移量
// 2. 基类成员指针类型 可以隐式转化为 子类成员指针类型
// 3. 虚继承，因为设计到虚基类偏移量的间接查找，所以性能会稍微差点，不重要

// int main() {
//     // 类成员指针，表示成员在类中的偏移量
//     float point3d::* pz = &point3d::z_;
//     point3d obj3d;
//     cout << obj3d.*pz << endl;

//     // 注意：如果是基类中的成员指针，在派生类中也可以直接访问，只不过存在内部转换
//     int base2::* pm = &base2::m_;
//     printf("p: %d\n", pm);  // output: 8 m_在 base2 内部偏移量为 8
//     func(pm, obj3d);  // output: 24 m_ 在 point3d 内部偏移量为 24

//     return 0;
// }



// // 类成员函数指针
// int main() {
//     // 注意成员函数指针的类型
//     typedef void(point3d::* info)();
//     info func = &point3d::info;
//     point3d obj3d;
//     point3d* p = &obj3d;
//     // 注意调用方式
    
//     (obj3d.*func)();
//     (p->*func)();

//     return 0;
// }

// 注意，静态成员 和 静态函数的表现则完全不同
int main() {
    // 静态成员地址
    int* p = &point3d::count;

    // 静态成员函数与普通函数完全一致
    typedef int(* functor)();
    functor func = point3d::get_count;
    cout << p << endl;
    cout << func() << endl;
    return 0;
}