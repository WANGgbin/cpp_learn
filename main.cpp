#include <iostream>

using namespace std;

class Parents{
private:
    Parents(){
        cout << "Call parents\n";
    }
};

class Children: public Parents{
public:
    Children(){
        cout << "Call children\n";
    }
};

int main() {
    Children child;
    return 0;
}