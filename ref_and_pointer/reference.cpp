#include<iostream>

using std::endl;
using std::cout;


int global = 1;

// 引用类似于指针一样，也是可以强转的
float& ref_transter1() {
    return (float&)global;
}


// float& ref_transter2() {
//     return (float)global;  // 这里会生成临时变量，转化为 float& 会报错
// }


int main() {
    float& ref = ref_transter1();
    cout << ref << endl;
    cout << &ref << endl;
    cout << &global << endl;
    return 0;
}