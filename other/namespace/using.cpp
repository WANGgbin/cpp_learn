#include<iostream>

namespace space1 {
    int i = 1;
}

namespace space2 {
    int i = 2;
}

// // using 声明 等价与 extern

// int main() {
//     using space1::i;
//     int i = 3;  // ‘int i’ conflicts with a previous declaration
//     std::cout << i << std::endl;
// }

// // 等价与 
// int main() {
//     extern int u;
//     int u = 3;  // ‘int u’ conflicts with a previous declaration
//     std::cout << u << std::endl;
//     return 0;
// }


// using 编译指令
// 含义：修改 using 编译指令所在的作用域的变量的搜寻顺序。当局部作用域找不到符号的时候
// 开始在全局 和 using 编译指令指定的 namespace 中寻找。如果存在多个可选的值，则
// 编译错误： reference to ‘i’ is ambiguous

// int main() {
//     using namespace space1;
//     using namespace space2;

//     std::cout << i << std::endl;  // reference to ‘i’ is ambiguous
// }


// 最佳实践
// 1. 开发函数库和类库的时候，将其放在一个命名空间中。
// 2. 尽量使用 using 声明 而不是 using 编译指令
// 3. using 声明尽量局部而不是全局