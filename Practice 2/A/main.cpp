#include <iostream>

int main() {
    int z0 = 0, z1 = 0, o0 = 0, o1 = 0;
    for (int i = 0; i < 10000; i++) {
        int b;
        std::cin >> b;
        bool loc;
        if (!b) {
            if (z0 > z1) {
                std::cout << 0 << std::endl;
                loc = 0;
            } else {
                std::cout << 1 << std::endl;
                loc = 1;
            }
            std::string ans;
            std::cin >> ans;
            if (ans == "YES") {
                if (!loc) {
                    ++z0;
                } else {
                    ++z1;
                }
            } else {
                if (loc) {
                    ++z0;
                } else {
                    ++z1;
                }
            }
        } else {
            if (o0 > o1) {
                std::cout << 0 << std::endl;
                loc = 0;
            } else {
                std::cout << 1 << std::endl;
                loc = 1;
            }
            std::string ans;
            std::cin >> ans;
            if (ans == "YES") {
                if (!loc) {
                    ++o0;
                } else {
                    ++o1;
                }
            } else {
                if (loc) {
                    ++o0;
                } else {
                    ++o1;
                }
            }
        }
    }

    return 0;
}