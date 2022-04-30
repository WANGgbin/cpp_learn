#include <iostream>
#include <functional>
#include <new>

using std::endl;
using std::cout;


// template<typename func>
// class func_wrapper {
// public:
//     func task;
// public:
//     func_wrapper(func par): task(par){}
// };

// int plus(int a, int b) {
//     return a + b;
// }

// auto lam_func = [](int a, int b) -> int {
//     return a + b;
// };


// class functor {
// public:
//     int operator()(int a, int b){
//         return a + b;
//     }
// };

// class memb_func {
// public:
//     int plus(int a, int b) const {
//         return a + b;
//     }
// };

// int main() {
//     std::function<int(int, int)> obj_func(plus);
//     cout << obj_func(1, 2) << endl;
//     cout << typeid(plus).__is_function_p() << endl;
//     std::function<int(int, int)> obj_lam_func(lam_func);
//     cout << obj_lam_func(1, 2) << endl;

//     std::function<int(int, int)> obj_functor = functor();
//     cout << obj_functor(1, 2) << endl;

//     std::function<int(const memb_func&, int, int)> obj_memb_func(&memb_func::plus);
//     cout << obj_memb_func(memb_func(), 1, 2) << endl;
// }



// c++ 模板中函数指针类型需要特别注意
// template<typename T>
// void func(T par) {
//     cout << typeid(T).name() << endl;  // output:PFvvE 这里 T 是函数指针类型
// }


// template<typename T>
// void func(const T& par) {
//     cout << typeid(T).name() << endl;  // output:Fvve 这里 T 是函数类型
// }

template<typename T>
void func(T& par) {
    cout << typeid(T).name() << endl;  // output:Fvve 这里 T 是函数类型
}

void print(){}

int main() {
    func(print);
    return 0;
}

