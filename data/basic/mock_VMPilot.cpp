#include <iostream>

void VMPilot_Begin(char const* name) {
    std::cout << "VMPilot_Begin: " << name << std::endl;
}

void VMPilot_End(char const* name) {
    std::cout << "VMPilot_End: " << name << std::endl;
}
