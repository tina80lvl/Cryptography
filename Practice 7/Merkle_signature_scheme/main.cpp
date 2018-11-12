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

class SHA256 {
protected:
    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    const static uint32 sha256_k[];
    static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);
public:
    void init();
    void update(const unsigned char *message, unsigned int len);
    void final(unsigned char *digest);
    static const unsigned int DIGEST_SIZE = (256 / 8);

protected:
    void transform(const unsigned char *message, unsigned int block_nb);
    unsigned int m_tot_len;
    unsigned int m_len;
    unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
    uint32 m_h[8];
};

std::string sha256source(std::string input);

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)                 \
{                                             \
    *((str) + 3) = (uint8) ((x)      );       \
    *((str) + 2) = (uint8) ((x) >>  8);       \
    *((str) + 1) = (uint8) ((x) >> 16);       \
    *((str) + 0) = (uint8) ((x) >> 24);       \
}
#define SHA2_PACK32(str, x)                   \
{                                             \
    *(x) =   ((uint32) *((str) + 3)      )    \
           | ((uint32) *((str) + 2) <<  8)    \
           | ((uint32) *((str) + 1) << 16)    \
           | ((uint32) *((str) + 0) << 24);   \
}

const unsigned int SHA256::sha256_k[64] = //UL = uint32
        { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
          0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
          0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
          0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
          0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
          0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
          0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
          0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
          0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
          0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
          0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
          0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
          0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
          0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
          0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
          0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

void SHA256::transform(const unsigned char *message, unsigned int block_nb) {
    uint32 w[64];
    uint32 wv[8];
    uint32 t1, t2;
    const unsigned char *sub_block;
    int i;
    int j;
    for (i = 0; i < (int)block_nb; i++) {
        sub_block = message + (i << 6);
        for (j = 0; j < 16; j++) {
            SHA2_PACK32(&sub_block[j << 2], &w[j]);
        }
        for (j = 16; j < 64; j++) {
            w[j] = SHA256_F4(w[j - 2]) + w[j - 7] + SHA256_F3(w[j - 15]) + w[j - 16];
        }
        for (j = 0; j < 8; j++) {
            wv[j] = m_h[j];
        }
        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6])
                 + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }
        for (j = 0; j < 8; j++) {
            m_h[j] += wv[j];
        }
    }
}

void SHA256::init() {
    m_h[0] = 0x6a09e667;
    m_h[1] = 0xbb67ae85;
    m_h[2] = 0x3c6ef372;
    m_h[3] = 0xa54ff53a;
    m_h[4] = 0x510e527f;
    m_h[5] = 0x9b05688c;
    m_h[6] = 0x1f83d9ab;
    m_h[7] = 0x5be0cd19;
    m_len = 0;
    m_tot_len = 0;
}

void SHA256::update(const unsigned char *message, unsigned int len) {
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;
    tmp_len = SHA224_256_BLOCK_SIZE - m_len;
    rem_len = len < tmp_len ? len : tmp_len;
    memcpy(&m_block[m_len], message, rem_len);
    if (m_len + len < SHA224_256_BLOCK_SIZE) {
        m_len += len;
        return;
    }
    new_len = len - rem_len;
    block_nb = new_len / SHA224_256_BLOCK_SIZE;
    shifted_message = message + rem_len;
    transform(m_block, 1);
    transform(shifted_message, block_nb);
    rem_len = new_len % SHA224_256_BLOCK_SIZE;
    memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
    m_len = rem_len;
    m_tot_len += (block_nb + 1) << 6;
}

void SHA256::final(unsigned char *digest) {
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    int i;
    block_nb = (1 + ((SHA224_256_BLOCK_SIZE - 9)
                     < (m_len % SHA224_256_BLOCK_SIZE)));
    len_b = (m_tot_len + m_len) << 3;
    pm_len = block_nb << 6;
    memset(m_block + m_len, 0, pm_len - m_len);
    m_block[m_len] = 0x80;
    SHA2_UNPACK32(len_b, m_block + pm_len - 4);
    transform(m_block, block_nb);
    for (i = 0; i < 8; i++) {
        SHA2_UNPACK32(m_h[i], &digest[i << 2]);
    }
}

std::string sha256source(std::string input) {
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest, 0, SHA256::DIGEST_SIZE);

    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update((unsigned char*)input.c_str(), input.length());
    ctx.final(digest);

    char buf[2 * SHA256::DIGEST_SIZE + 1];
    buf[2 * SHA256::DIGEST_SIZE] = 0;
    for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
        sprintf(buf + i * 2, "%02x", digest[i]);
    return std::string(buf);
}

std::vector<char> sha256(std::string input) {
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest, 0, SHA256::DIGEST_SIZE);

    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update((unsigned char*)input.c_str(), input.length());
    ctx.final(digest);

    std::vector<char> res;
    res.assign(digest, digest + 32);
    return res;
}

/////////////////////////////////////////////////////////////////////////////////////

char zero = 0, one = 1, two = 2;

std::string to_string(std::vector<byte> const& v) {
    std::string s = "";
    for (int i = 0; i < v.size(); i++) {
        s += (char)v[i];
    }
    return s;
}
std::string to_string(std::vector<char> const& v) {
    std::string s;
    for (int i = 0; i < v.size(); i++) {
        s += (char)v[i];
    }
    return s;
}

bool check_proof(std::string hash, int id, std::string pk) {
    int h = 8;
    hash = to_string(base64::decode(hash));

    int q = 1;
    for (int block = 0; block < q; block++) {
        std::string data = pk;

        if (data == "null") {
            data = "";
        } else {
            data = to_string(sha256(zero + to_string(base64::decode(data))));
        }

        for (int neigbour = 0; neigbour < h; neigbour++) {
            std::string n_data;
            std::cin >> n_data;
            if (n_data == "null") {
                n_data = "";
            } else {
                n_data = to_string(base64::decode(n_data));
            }

            if (n_data != "" || data != "") {
                if (id % 2) {
                    data = to_string(sha256(one + n_data + two + data));
                } else {
                    data = to_string(sha256(one + data + two + n_data));
                }
            }
            id /= 2;
        }

        if (data == hash) {
            return true;
        } else {
            return false;
        }
    }
}

int main() {
//    freopen("input.txt", "r", stdin);
    std::string hash;
    std::cin >> hash;

    std::vector<std::vector<byte>> is_signed(2, std::vector<byte>(256));
    std::vector<bool> used(256, false);
    while (true) {
        int id;
        std::cin >> id;

        if (!used[id]) {
            for (size_t i = 0; i < 256; ++i) {
                std::cout << 0;
            }
            std::cout << std::endl;
        } else {
            for (size_t i = 0; i < 256; ++i) {
                std::cout << 1;
            }
            std::cout << std::endl;
        }

        bool lie = true;
        std::string sign, public_key;
        std::cin >> sign;
        std::cin >> public_key;

        //// check proof -> lie
        if (!check_proof(hash, id, public_key)) {
            lie = false;
        }

        //// check is sign correct
        size_t col = 0;
        if (used[id]) {
            col = 1;
        }
        for (size_t i = 0; i < 256; ++i) {
            std::string y = public_key.substr((col * 256 + i) * 32, 32);
            if (to_string(sha256(sign.substr(i * 32, 32))) != y) {
                lie = false;
                break;
            }
        }

        std::vector<byte> dec_sign = base64::decode(sign), dec_pub_key = base64::decode(public_key);

        std::string to_answer;
        std::cin >> to_answer;

        if (lie) {
            std::cout << "YES\n";
            if (!used[id]) {
                for (size_t i = 0; i < 256; ++i) {
                    is_signed[0][i] = dec_sign[i];
                }
                used[id] = true;
                std::cout << "NO\n";
            } else {
                for (size_t i = 0; i < 256; ++i) {
                    is_signed[1][i] = dec_sign[i];
                }
                std::cout << "YES\n";
                std::vector<byte> answer;
                //// encode
                for (size_t i = 0; i < to_answer.length(); ++i) {
                    if (to_answer[i] == '0') {
//                        answer.push_back(is_signed[0][i]);
                        for (size_t j = 0; j < 32; ++j) {
                            answer.push_back(is_signed[0][i * 32 + j]);
                        }
                    } else {
                        for (size_t j = 0; j < 32; ++j) {
                            answer.push_back(is_signed[1][i * 32 + j]);
                        }
                    }
//                    answer.push_back(is_signed[to_answer[i] - '0'][i]);
                }
                std::cout << base64::encode(answer) << std::endl;
                break;
            }
        } else {
            std::cout << "NO\nNO\n";
        }

    }


    return 0;
}

