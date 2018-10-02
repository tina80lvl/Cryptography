#include <iostream>
#include <vector>

int main() {
    freopen("058.cipher", "r", stdin);
//    freopen("058.decoded", "w", stdout);

    std::string s;
    std::cin >> s;

    for (int l = 6; l <= 10; l++) { // перебираем длину ключа
        std::string dec = s, key;
        for (int mv = 0; mv < l; mv++) { // сдвиг в ключе
            int max_f = -1;
            std::vector <int> freq (124);
            for (int i = mv; i < s.size(); i += l) { // считаем частоту по 1 букве через длину ключа
                ++freq[s[i]];
            }
            char letter;
            for (int i = 'a'; i < 'z'; i++) {
                if (freq[i] > max_f) {
                    letter = i;
                    max_f = freq[i];
                }
            }

//            std::cout << "most frequant " << letter << std::endl;
            int move = letter - 'e'; // определяем сдвиг ключом
            key += (move < 0 ? move + 26 : move) + 'a' - 1;
            for (int i = mv; i < s.size(); i += l) { // заменяем
                int g = dec[i] - move;
                if (g < 'a') {
                    dec[i] = g + 26; // less then 26
                } else if (g > 'z') {
                    dec[i] = g - 26;
                } else {
                    dec[i] = g;
                }
            }
        }
        std::cout << l << " " << key << std::endl  << dec << std::endl;
    }

    return 0;
}