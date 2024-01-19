#define _USE_MATH_DEFINES

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <fstream>

std::vector<uint32> LFSR_L(std::vector<uint32> bits) {
    std::vector<uint32> bitout;
    uint32 x1 = 0;
    for (uint32 i = 0; i < MAX_N; i++) {//mozna zamienic na bits.size() zeby rozmiar byl taki sam jak wejsciowych bitów
        x1 = bits[0] ^ bits[2] ^ bits[3] ^ bits[4] ^ bits[5] ^ bits[6] ^ bits[7] ^ bits[9];
        bitout.push_back(bits.back());
        std::shift_right(begin(bits), end(bits), 1);
        bits.push_back(x1);
        std::swap(bits.front(), bits.back());
        bits.pop_back();
    }
    return bitout;
}

std::vector<uint32> LFSR_R(std::vector<uint32> bits) {
    std::vector<uint32> bitout;
    uint32 x1 = 0;
    for (uint32 i = 0; i < MAX_N; i++) {
        x1 = bits[0] ^ bits[3] ^ bits[5] ^ bits[6] ^ bits[8] ^ bits[9];
        bitout.push_back(bits.back());
        std::shift_right(begin(bits), end(bits), 1);
        bits.push_back(x1);
        std::swap(bits.front(), bits.back());
        bits.pop_back();
    }
    return bitout;
}

std::vector<uint32> goldCode(std::vector<uint32> bitstream, std::vector<uint32> bitstream2, uint32 N) { //N = dlugosc kodu golda
    if (bitstream.size() == bitstream2.size()) {
        size_t bitsize = bitstream.size();
        std::vector<uint32> bitout1 = LFSR_L(bitstream);
        std::vector<uint32> bitout2 = LFSR_R(bitstream2);
        std::vector<uint32> gold;
        for (int i = 0; i < N; i++) {
            gold.push_back(bitout1[i] ^ bitout2[i]);
        }
        return gold;
    }
    else abort();
}

void test() {
    std::string bitstr = "01101";
    uint32 N = 1023;
    std::vector<uint32> bits = { 0, 1, 1, 0, 1, 1, 0, 0, 1, 0 };
    std::vector<uint32> bits2 = { 1, 1, 0, 0, 0, 1, 1, 0, 1, 1 };
    std::vector<uint32> bitout1 = LFSR_L(bits);
    std::vector<uint32> bitout2 = LFSR_R(bits2);
    std::vector<uint32> gold = goldCode(bits, bits2, N);
    std::cout << "out bit1: " << "\n";
    for (int i = 0; i < bitout1.size(); i++) {
        std::cout << bitout1[i];
    }
    std::cout << "\nout bit2: " << "\n";
    for (int i = 0; i < bitout2.size(); i++) {
        std::cout << bitout2[i];
    }
    std::cout << "\ngold: ";

    for (int i = 0; i < gold.size(); i++) {
        if (i % 10 == 0) std::cout << std::endl;
        std::cout << gold[i];
    }
    saveGold("gold_code.txt", gold);

}

int main() {
    test();
    return 0;
}