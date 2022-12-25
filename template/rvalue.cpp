#include<iostream>

using std::endl;
using std::cout;

class A{
public:
    A(int a=0) {m_a = a;}
    A(const A& ref) {m_a = ref.m_a; cout << "call A(const A&)" << endl;}
public:
    int m_a;
};

// 对于rvalue, func(A obj) 与 func(A&& obj) 不能同时定义
// 对于lvalue, func(A obj) 与 func(A& obj) func(const A& obj) 不能同时定义
// 但是 func(A&& obj) 与 func(A& obj) 以及 func(const A& obj) 是可以同时定义的,会根据优先级匹配合适的函数
void func(A obj) {
    cout << "func(A)" << endl;
}

// void func(A&& obj) {
//     // 注意：进入到函数内部之后，此时被右值引用的变量也不再是临时变量，这与 A&& rref = A(); 是类似的
//     cout << "func(&&)" << endl;
// }


// void func(A& obj) {
//     cout << "func(&)" << endl;
// }

// void func(const A& obj) {
//     cout << "func(const A&)" << endl;
// }

// 临时变量
//  1. 为什么是临时变量呢？ 因为表达式结束的时候，变量的生命期就结束了。
//  2. 临时变量是无法取地址的。
//  3. 如果将一个临时变量变为非临时变量呢？答案是更改其生命期，如果更改生命期？，通过右值引用的方式


// int main() {
//     // 这里是临时变量
//     func(A());  // "func(&&)"

//     // 通过右值引用的方式，修改临时变量的生命期为引用的生命期，引用结束的时候，变量生命期随之结束
//     // 此时不再是个临时变量
//     A&& rref = A();

//     func(rref);  // "func(&)"
//     return 0;
// }



// 万能引用（引用折叠）
//  1. 含义
//  仅针对于模板参数 T 而言， T&& 就是万能引用，万能引用意味这能够适配左值引用和右值引用
//  2. 特性存在的意义？
//  提供一种机制，使得模板能够接受任意类型的参数，且 deduce(推导)出的 T 与参数类型一致。
// 万能引用是基于引用折叠实现的，引用折叠规则如下：
//  T& && -> T&
//  T&& && -> T&&
//  T && -> T&&
// T&& & -> T&
// T& & -> T&

// // @Description: 测试后两种引用折叠
// template<typename T>
// void test(T&& ref) {
//     func(static_cast<T&>(ref));
// }

// template<typename T>
// void move(T&& ref) {
//     // do something
//     return;
// }


// template<typename T>
// void value(T ref) {
//     return;
// }

// template<typename T>
// void lvalue_ref(T& ref) {
//     func(ref);
//     return;
// }

// int main() {
//     // // 模板实例化为：move(A&& ref), T: A&&
//     // ::move(A());

//     // A obj;
//     // // 模板实例化为: move(T& ref), T: A&
//     // ::move(obj);
//     A obj;
//     A& ref = obj;
//     ::value(ref);  // 特别注意，这里实例化 deduce 出的 T 是 A，而不是 A&!!! 这里调用了拷贝构造函数 output: call A(const A&)

//     const A& const_ref = A();
//     ::lvalue_ref(const_ref);  // 这里 deduce 出的 T 是 const A!!! output: func(const A&)


//     // 验证
//     // T&& & -> T&
//     // T& & -> T&
//     ::test(A());  // output: func(&)
//     return 0;
// }

// 再看下 std::move 的实现
/**
   *  @brief  Convert a value to an rvalue.
   *  @param  __t  A thing of arbitrary type.
   *  @return The parameter cast to an rvalue-reference to allow moving it.
  */
//   template<typename _Tp>
//     constexpr typename std::remove_reference<_Tp>::type&&
//     move(_Tp&& __t) noexcept
//     { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }

// // 如果是右值引用，则对应的实例化为：
// A&& move(A&& t) {
//     return static_cast<A&&>(t);
// }

// // 如果是左值引用，则对应的实例化为：
// A&& move(A& t) {
//     return static_cast<A&&>(t);
// }



// // 右值引用是可以隐式转化为左值引用的

// int main() {
//     // 右值引用是可以隐式转化为左值引用的T
//     A&& rref = A();
//     A& ref = rref;
//     cout << &ref << endl;

//     // 因此可以通过 static_cast<> 将左值引用转化为右值引用
//     A obj;
//     A& ref1 = obj;
//     A&& rref1 = static_cast<A&&>(ref1); 
//     return 0;
// }


// 函数模板
// forward 是转发的意思，这里的转发本质就是类型转化 左右值引用之间的相互转换
// 使用方式：
// 模板参数制定要转化的类型即可
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{
    cout << "call forward(&)" << endl;
    return static_cast<_Tp&&>(__t); 
}

template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
{
    static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
        " substituting _Tp is an lvalue reference type");
    cout << "call forward(&&)" << endl;
    return static_cast<_Tp&&>(__t);
}


int main() {
    // 观察这里的两个 forward 发现，因为函数参数类型本身也依赖于模板参数，所以无法通过参数类型
    // 推到出模板参数的类型。
    // ::forward(A());
    // 报错信息如下：
    //     rvalue.cpp:136:18: error: no matching function for call to ‘forward(A)’
    //   136 |     ::forward(A());
    //       |                  ^
    // rvalue.cpp:118:1: note: candidate: ‘template<class _Tp> constexpr _Tp&& forward(typename std::remove_reference<_Tp>::type&)’
    //   118 | forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    //       | ^~~~~~~
    // rvalue.cpp:118:1: note:   template argument deduction/substitution failed:
    // rvalue.cpp:136:18: note:   couldn’t deduce template parameter ‘_Tp’
    //   136 |     ::forward(A());
    
    // 那么该如何使用函数模板呢？通过显示实例化而不是隐式实例化。
    // ::forward<A&&>(A());  // 指定模板参数 _Tp = A&&，对应实例化函数为： A&& forward(A&&)
    // A obj;
    // ::forward<A&>(obj);   // 指定模板参数 _Tp = A&，对应实例化函数为： A& forward(A&)

    // A&& rref = A();
    // ::forward<A&&>(rref); // 指定模板参数 _Tp = A&&，对应实例化函数为： A&& forward(A&) 注意！！！ 实例化并不是 A&& forward(A&&)

    A obj;
    func(::forward<A&&>(obj));
    return 0;
}


// template<typename T>
// struct remove_reference{
//     typedef T value_type;
//     static void print() {
//         cout << "T" << endl;
//     };
// };

// template<typename T>
// struct remove_reference<T&>{
//     typedef T value_type;
//     static void print() {
//         cout << "T&" << endl;
//     };
// };

// // 在类模板中，T&& 就是右值引用的意思，并不是万能引用
// template<typename T>
// struct remove_reference<T&&> {
//     typedef T value_type;
//     static void print() {
//         cout << "T&&" << endl;
//     };
// };


// int main() {
//     remove_reference<A&&>::print();  // output: T&&
//     remove_reference<A&>::print();  // output: T&
//     remove_reference<A>::print();  // output: T
//     return 0;
// }