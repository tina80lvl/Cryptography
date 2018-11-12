#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <memory.h>
#include <cstdlib>
#include <cstdio>

using byte = int8_t;
using ubyte = uint8_t;

class base64 {
    static ubyte to_ubyte(byte x) {
        return *reinterpret_cast<ubyte *>(&x);
    }

    static void fill_inds(std::array<ubyte, 3> const &buff, std::array<ubyte, 4> &inds) {
        inds[0] = static_cast<ubyte>((buff[0] & 0b11111100) >> 2);
        inds[1] = static_cast<ubyte>(((buff[0] & 0b11) << 4) + ((buff[1] & 0b11110000) >> 4));
        inds[2] = static_cast<ubyte>(((buff[1] & 0b1111) << 2) + ((buff[2] & 0b11000000) >> 6));
        inds[3] = static_cast<ubyte>(buff[2] & 0b111111);
    }

public:
    static std::string encode(std::vector<byte> const &src) {
        static char encode_chars[] = {
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                '4', '5', '6', '7', '8', '9', '+', '/'
        };

        std::string res;
        std::array<ubyte, 3> buff{0, 0, 0};
        std::array<ubyte, 4> inds{};
        size_t i = 0;
        for (i = 0; i + 2 < src.size(); i += 3) {
            for (size_t j = 0; j < 3; ++j) {
                buff[j] = to_ubyte(src[i + j]);
            }
            fill_inds(buff, inds);
            for (size_t j = 0; j < 4; ++j) {
                res.push_back(encode_chars[inds[j]]);
            }
        }
        buff.fill(0);
        for (size_t j = 0; i + j < src.size(); ++j) {
            buff[j] = to_ubyte(src[i + j]);
        }
        fill_inds(buff, inds);
        switch (src.size() - i) {
            case 1: {
                res.push_back(encode_chars[inds[0]]);
                res.push_back(encode_chars[inds[1]]);
                res.append("==");
                break;
            }
            case 2: {
                res.push_back(encode_chars[inds[0]]);
                res.push_back(encode_chars[inds[1]]);
                res.push_back(encode_chars[inds[2]]);
                res.push_back('=');
                break;
            }
            default: {
                break;
            }
        }
        return res;
    }

    static std::vector<byte> decode(std::string const &src) {
        static constexpr unsigned char decode_table[] = {
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
                64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
                64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
        };

        std::array<uint32_t, 4> buff{};
        if (src.size() % 4 != 0) {
            assert(false && "incorrect_data");
        }
        auto out_size = src.size() / 4 * 3;
        if (!src.empty()) {
            out_size -= (src[src.size() - 1] == '=');
            out_size -= (src[src.size() - 2] == '=');
        }
        std::vector<byte> res(out_size);
        for (size_t i = 0, j = 0; i < src.size(); i += 4) {
            uint32_t triple = 0;
            for (size_t k = 0; k < 4; k++) {
                buff[k] = (src[i + k] == '=' ? 0 : decode_table[src[i + k]]);
                triple += buff[k] << ((3 - k) * 6);
            }

            if (j < out_size) {
                res[j++] = static_cast<byte>((triple >> 2 * 8) & 0xFF);
            }
            if (j < out_size) {
                res[j++] = static_cast<byte>((triple >> 1 * 8) & 0xFF);
            }
            if (j < out_size) {
                res[j++] = static_cast<byte>((triple >> 0 * 8) & 0xFF);
            }
        }
        return res;
    }
};

int main() {
//    freopen("input.txt", "r", stdin);

    std::string AES, IV;
    std::cin >> AES >> IV;

    std::vector <byte> iv_dec = base64::decode(IV);
    byte old_v = iv_dec[2]++;

    std::cout << "NO\n" << AES << std::endl << base64::encode(iv_dec) << std::endl;

    std::string verdict;
    std::cin >> verdict;
//    std::getline(std::cin, verdict);

    if (verdict[0] == 'O') {
        iv_dec[2] = old_v;
        iv_dec[2] ^= 's' ^ 14;
        for (size_t i = 3; i < 16; ++i) {
            iv_dec[i] ^= 14 ^ 13;
        }
        std::cout << "NO\n" << AES << std::endl << base64::encode(iv_dec) << std::endl;

        std::cin >> verdict;
//        std::getline(std::cin, verdict);
        std::cout << "YES\n";
        if (verdict[0] == 'O') {
            std::cout << "Yes\n";
        } else {
            std::cout << "N/A\n";
        }
    } else {
        std::cout << "YES\nNo\n";
    }

    return 0;
}