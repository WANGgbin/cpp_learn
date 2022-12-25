#include<unistd.h>

int main() {
    daemon(0, 0);
    for(;;) {
        sleep(1);
    }
    return 0;
}