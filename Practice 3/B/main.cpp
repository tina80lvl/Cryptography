#include <iostream>
#include <vector>

int main() {
    std::vector <int> keys(10000);
    for (int i = 0; i < 2000; i++) {
        int c;
        std::cin >> c;
        std::cout << 1 << std::endl;
        std::string ans;
        std::cin >> ans;
        if (ans == "YES") {
            keys[i] = (c^1);
        } else {
            keys[i] = (c^0);
        }
    }

    int t = 1;
    for (int ti = 1; ti != 1001; ++ti) {
        bool flg = true;
        for (int i = 0; i < 2000 - ti; i++) {
            if (keys[i] != keys[i + ti]) {
                flg = false;
            }
        }
        if (flg) {
            t = ti;
            break;
        }
    }

    for (int i = 2000; i != 10000; i++) {
        int c;
        std::cin >> c;
        keys[i] = keys[i - t];
        std::cout << (c^keys[i]) << std::endl;
        std::string ans;
        std::cin >> ans;
    }

    return 0;
}