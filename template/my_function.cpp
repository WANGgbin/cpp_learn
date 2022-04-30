#include <cstdio>
#include <iostream>
#include <new>

using std::endl;
using std::cout;

/*
 * 1、std::function 如何将不同类型的对象统一起来？
 * 2、function 的多态性(调用不同类型的对象)是通过模板的特化实现的
*/

// 我们自己来实现一个简易版本的 function 类模板
template<typename signature>
class function;

class any_class{};

union any_data{
    void* ptr;
    const void* const_prt;
    void(* function)();
    void(any_class:: *member_function)();

    // 涉及到大量的从 any_data 到 functor 的转化，那么就把转化收敛到 any_data 内部，外部调用即可。
    // 转化是通过 void* 到 T* 的隐式转化实现的
    void* _M_access() {return this;}
    const void* _M_access() const {return this;}

    template<typename T>
    T& _M_access() {
        return *static_cast<T*>(_M_access());
    }

    template<typename T>
    const T& _M_access() const {
        return *static_cast<const T*>(_M_access());
    }
};

// 判断 functor 的存储方式
template<typename functor>
struct stored_locally{
    static const bool is_stored_locally = sizeof(any_data) >= sizeof(functor);
};

// data_manager 负责 any_data 的初始化、访问、释放
template<typename functor>
class data_manager{
public:
    static void init_data(any_data& data, const functor& f);
    static void destroy_data(any_data& data);
    static functor& access_data(const any_data& data);
private:
    static void _init_data_locally(any_data& data, const functor& f);
    static void _init_data_not_locally(any_data& data, const functor& f);

    static void _destroy_data_locally(any_data& data){
        data._M_access<functor>().~functor();
    }

    static void _destroy_data_not_locally(any_data& data){
        delete data._M_access<functor*>();
    }

    static const functor& _access_data_locally(const any_data& data) {
        return data._M_access<functor>();
    }

    static functor& _access_data_not_locally(const any_data& data) {
        return *data._M_access<functor*>();
    }
};

template<typename functor>
void data_manager<functor>::init_data(any_data& data, const functor& f) {
    if(stored_locally<functor>::is_stored_locally) {
        _init_data_locally(data, f);
    } else {
        _init_data_not_locally(data, f);
    }
}

template<typename functor>
void data_manager<functor>::_init_data_locally(any_data& data, const functor& f) {
    ::new (data._M_access()) functor(std::move(f));
}

template<typename functor>
void data_manager<functor>::_init_data_not_locally(any_data& data, const functor& f) {
    data._M_access<functor*>() = new functor(f);
}

template<typename functor>
void data_manager<functor>::destroy_data(any_data& data) {
    if(stored_locally<functor>::is_stored_locally) {
        data_manager::_destroy_data_locally(data); // 必须使用 data::manager 显示指定，否则：there are no arguments to ‘_destroy_data_locally’ that depend on a template parameter, so a declaration of ‘_destroy_data_locally’ must be available
    } else {
        _destroy_data_not_locally(data);
    }
}

template<typename functor>
functor& data_manager<functor>::access_data(const any_data& data) {
    if(stored_locally<functor>::is_stored_locally) {
        return const_cast<functor&>(_access_data_locally(data));
    }
    return _access_data_not_locally(data);
}

// handler 在 manager 基础上扩展了对 data 的操作，主要是 invoke 操作，完成多态调用

// 默认版本（普通函数、仿函数使用）
template<typename ret_type, typename functor, typename... arg_types>
class handler: public data_manager<functor> {
    typedef data_manager<functor> manager;
public:
    static ret_type invoke(const any_data& data, arg_types... args) {
        return manager::access_data(data)(args...);
    }
};

// 为了让偏特化生效，必须声明通用声明
template<typename member_pointer>
class memb_func;


// 注意类成员函数指针的表达方式
// 非 const 成员函数的偏特化版本
template<typename ret_type, typename _class, typename... arg_types>
class memb_func<ret_type(_class::*)(arg_types...)> {
    typedef ret_type(_class::* functor)(arg_types...);
private:
    functor f_;
public:
    memb_func(const functor& f): f_(f){}
    ret_type operator()(_class& obj, arg_types... args) {
        // 注意成员函数指针的表达方式
        return (obj.*f_)(args...);
    }
};

// const 成员函数的偏特化版本
template<typename ret_type, typename _class, typename... arg_types>
class memb_func<ret_type(_class::*)(arg_types...)const> {
    typedef ret_type(_class::* functor)(arg_types...)const;
private:
    functor f_;
public:
    memb_func(const functor& f): f_(f){}
    ret_type operator()(const _class& obj, arg_types... args) {
        // 注意成员函数指针的表达方式
        return (obj.*f_)(args...);
    }
};

// 成员函数指针的特化版本
template<typename ret_type, typename member, class _class, typename... arg_types>
class handler<ret_type, member _class::*, arg_types...>: public data_manager<member _class::*> {
private:
    typedef member _class::* functor;
    typedef data_manager<functor> manager;
public:
    static ret_type invoke(const any_data& data, arg_types... args) {
        // 注意这里是如何把 args 拆成 obj, args 的
        memb_func<functor> m_f(manager::access_data(data));
        return m_f(args...);
    }
};


// function 设计讨论
// 实际上 function 的整个流程中都涉及到了 functor 类型，所以应该在 function 模板参数中指定 functor。
// 但是这样带来的问题是，用户使用的时候到要手动指定 functor 类型，不方便。
// 但是只在构造函数中制定 functor 类型带来的问题是，其他的成员函数行为涉及到 functor 的时候，但又不可见 functor，如何解决？
// 解决方法是，再创建一个中间类，这个类根据 functor 类型有着不同的特化，由构造函数实例化。
// 然后，在 function 中声明一系列函数指针类型，这些指针类型在不同实例化的中间类中的声明
// 都是一致的，这样在 function 的其他函数中，便可以隐藏 functor 的不同类别，只需要调用
// 对应的函数指针即可。

template<typename ret_type, typename... arg_types>
class function<ret_type(arg_types...)>{
private:
    typedef ret_type(*invoker_type)(const any_data&, arg_types...);
    typedef void (* destroy_type)(any_data&);
    invoker_type invoker;
    destroy_type destroyer;
private:
    any_data data_;

public:
    template<typename functor>
    function(functor f);
    ~function();
    ret_type operator()(arg_types... args) const;
};

template<typename ret_type, typename... arg_types>
template<typename functor>
function<ret_type(arg_types...)>::function(functor f) {
    cout << typeid(functor).name() << endl;
    typedef handler<ret_type, functor, arg_types...> handler;
    invoker = handler::invoke;
    destroyer = handler::destroy_data;
    // 初始化 data_
    handler::init_data(data_, f);
}

template<typename ret_type, typename... arg_types>
function<ret_type(arg_types...)>::~function() {
    destroyer(data_);
}

template<typename ret_type, typename... arg_types>
ret_type function<ret_type(arg_types...)>::operator()(arg_types... args) const {
    // 不同类型的对象的调用方式不同，通过一个模板类的特化，完成不同类型对象的调用
    return invoker(data_, args...);
}

int plus(int a, int b) {
    return a + b;
}

auto lam_func = [](int a, int b) -> int {
    return a + b;
};


class functor {
public:
    int operator()(int a, int b){
        return a + b;
    }
};

class A {
public:
    A(){count_ = 0;}
    int plus(int a, int b) const {
        return a + b;
    }
    int add_count_and_print() {
        return ++count_;
    }

private:
    int count_;
};

int main() {
    ::function<int(int, int)> func1(plus);
    cout << func1(1, 2) << endl;

    ::function<int(int, int)> func2 = functor();
    cout << func2(1, 2) << endl;

    ::function<int(int, int)> func3(lam_func);
    cout << func3(1, 2) << endl;

    ::function<int(const A&, int, int)> func4(&A::plus);
    cout << func4(A(), 1, 2) << endl;

    ::function<int(A&)> func5(&A::add_count_and_print);
    A obj;
    cout << func5(obj) << endl;

    return 0;
}

// 暴露问题：
// 1、对 const/ not const 意识不到位，经常出现 discards qualifiers 错误，即 const -> not const
// 2、类模板应该声明哪些类型参数，逻辑比较模糊。只有 明确类功能，才能明确类涉及到的参数。刚开始可能考虑到的
// 类参数不全面，可以继续开发，在开发类的过程中，不断完善类模板参数。


// 涉及知识点：
// 1、成员函数指针在类模板中的指定方式
// 2、成员函数指针的调用方式
// 3、union 类型及其成员函数的使用
// 4、中间类完成类模板参数在外部类中的隐藏
// 5、replace new 的使用
// 6、函数指针 在类模板中的推倒。详情见 function.cpp
// 7、注意类模板中多态的实现