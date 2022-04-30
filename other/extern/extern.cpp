#include<iostream>

int main() {

    extern int u;
    int u = 3;
    std::cout << u << std::endl;
    return 0;
}