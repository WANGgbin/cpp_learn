#include <iostream>

/**
 * 问题:
 * 1. enabled_shared_from_this 实现原理
 * 2. make_shared() 实现原理
 * 3. shared_ptr 内部的 count 是如何更新的
 */
namespace my_smart_pointer {
typedef int atomic_t;

// 特别注意 _count_base 是如何跟类型 T 解耦的。通过多态方式实现。_count_base 只要定义好接口，
// 由子类完成具体的实现，包括 T 类型的感知。
class _count_base{
private:
    atomic_t shared_count_;
    atomic_t weak_count_;
public:
    _count_base() {
        shared_count_ = 1;
        weak_count_  = 0;
    }

    // TODO: 暂时不考虑竞争
    void incr_shared_count() {
        ++shared_count_;
    }
    
    void decr_shared_count() {
        --shared_count_;
    }

    void incr_weak_count() {
        ++weak_count_;
    }
    
    int get_shared_count() {
        return shared_count_;
    }

    virtual void dispose() = 0;
    virtual void destroy() = 0;

    void release_resource() {
        if (--shared_count_ == 0) {
            dispose();
            if (--weak_count_ == 0) {
                destroy();
            }
        }
    }
};

template<typename deleter, typename T>
class count_deleter: public _count_base {
private:
    deleter del_;
    T* ptr_;
public:
    count_deleter(const deleter& del, T* ptr): _count_base(), del_(del), ptr_(ptr){}
    
    virtual void dispose() {
        del_(ptr_);
    }

    virtual void destroy() {
        delete this;
    }
};


template<typename T>
void default_deleter(T* pointer) {
    std::cout << "delete pointer " << pointer << std::endl;
    delete pointer;
}


template<typename T>
class shared_ptr{
private:
    T*  raw_pointer;
    _count_base* counter;

public:
    shared_ptr(){
        raw_pointer = nullptr;
        counter = nullptr;
    }

    shared_ptr(T* pointer) {
        raw_pointer = pointer;
        counter = new count_deleter<void(*)(T*), T>(default_deleter<T>, pointer);
    }

    template<typename deleter>
    shared_ptr(T* pointer, deleter del) {
        raw_pointer = pointer;
        counter = new count_deleter<deleter, T>(del, pointer);
    }

    shared_ptr(const shared_ptr& ptr) {
        raw_pointer = ptr.raw_pointer;
        counter = ptr.counter;
        counter->incr_shared_count();
    }

    int get_use_count() {
        return counter->get_shared_count();
    }

    ~shared_ptr(){
        counter->release_resource();
    }
};

} // my_smart_pointer