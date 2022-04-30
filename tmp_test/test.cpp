#include<iostream>
#include<vector>

using std::cout;
using std::endl;

class base {
public:
    base(int p=0) {i = p; cout << "call base()" << endl;}
    virtual void output() {
        cout << "output()" << endl;
    }

    virtual void output(int) {
        cout << "output(int)" << endl;
    }

    virtual ~base() {cout << "call ~base()" << endl;}

    int get_i() {return i;}
    static base& get_instance() {return instance;}
public:
    int i;
    static base instance;
};

base base::instance = base(1);

class derive: public base {
public:
    // virtual void output() {
    //     cout << "derive::output()" << endl;
    // }
    int get_i() {return i;}
public:
    // int i;
    static int count;
};

int derive::count = 0;

class A{
public:
    explicit A(int m = 0) {
        m_ = m;
    }
private:
    int m_;
};


int main() {
    A obj(1);
    A obj1 = A(2);
    A obj2 {3};

    return 0;
}