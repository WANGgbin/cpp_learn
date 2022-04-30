#include<iostream>

using std::cout;
using std::endl;

template<typename T>
class my_friend {
private:
    T item;
public:
    my_friend(T par = 0): item(par) {}
    template<typename T1, typename T2>
    friend void output_info(const T1& obj1, const T2& obj2);
};

my_friend<double> d2 = 1.111;

template<typename T1, typename T2>
void output_info(const T1& obj1, const T2& obj2) {
    cout << obj1.item << endl;
    cout << obj2<< endl;
    cout << d2.item << endl;
}


class with_item {
private:
    int item;
};


int main() {
    my_friend<int> obj1;
    int i = 1;

    output_info(obj1, i);

    return 0;
}