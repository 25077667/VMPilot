#include <iostream>

extern void VMPilot_Begin(char const*);
extern void VMPilot_End(char const*);

int foo(int data) {
    VMPilot_Begin(__FUNCTION__);
    int result = data * 2;
    VMPilot_End(__FUNCTION__);
    return result;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << foo(10) << std::endl;
    return 0;
}