#include <iostream>
#include <functional>

using namespace std;

std::function<int()> return_lambda() {
    int x = 0, y = 1;
    return [=]() mutable -> int {
        ++x;
        ++y;
        cout << "x: " << x << endl;
        cout << "y: " << y << endl;
        return x * y;
    };
}

int main() {
    std::function<int()> f = return_lambda();
    cout << f() << endl;
    cout << f() << endl;
    cout << f() << endl;
    return 0;
}