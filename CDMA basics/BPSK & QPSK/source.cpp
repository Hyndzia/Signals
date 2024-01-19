#define _USE_MATH_DEFINES



#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cmath>



std::vector<double> BPSK(double fs, double tc, std::string bstream, bool isPlot) {
    double B = bstream.size();  double tb = tc / B; double W = 2.0; double fn = W * (1 / tb);
    std::vector<double> X;
    uint16_t it = 0;
    for (const auto& bit : bstream) {
        if (bit == '0') {
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(sin(2 * M_PI * fn * t));
            }
            continue;
        }
        if (bit == '1') {
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(sin(2 * M_PI * fn * t + M_PI));
            }
            continue;
        }
    }
    if (isPlot) {
        std::vector<double> t;
        for (int i = 0; i < X.size(); i++) {
            t.push_back(i / fs);
        }
        //plt::plot(t, X);
    }

    return X;
}

std::vector<double> QPSK(double fs, double tc, std::string bstream, bool isPlot) {
    double B = bstream.size();  double tb = tc / B; double W = 2.0; double fn = W * (1 / tb);
    std::vector<double> X;
    uint16_t it = 0;
    char prev = 'x';
    std::vector<double> fi_vec = { 0.5 * M_PI, M_PI, 1.5 * M_PI };
    std::string bitblock = "xx";
    std::map<std::string, double> mapPSK = {
        {"00", 0.0},
        {"01", fi_vec[1]},
        {"10", fi_vec[2]},
        {"11", fi_vec[0]}
    };

    for (const auto& bit : bstream) {
        if (it == bstream.size()) {
            break;
        }
        if (prev == 'x') {
            prev = bit;
            ++it;
            continue;
        }
        if (prev != 'x') {
            bitblock[0] = prev; bitblock[1] = bit;
            double fi = mapPSK[bitblock];
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(sin(2 * M_PI * fn * t + fi));
            }
            prev = 'x';
            bitblock = "xx";
            ++it;
            continue;
        }
    }
    if (isPlot) {
        std::vector<double> t;
        for (int i = 0; i < X.size(); i++) {
            t.push_back(i / fs);;
        }
        //plt::plot(t, X);
    }
    return X;
}

void test() {
    std::string bitstr = "01001110";
    std::cout << bitstr;

    double tc = 8.0; double B = bitstr.size(); double W = 2.0; double tb = tc / B; double fn = W / tb; double fs = 20000; 

    auto ft = QPSK(fs, tc, bitstr, true);
    auto ft2 = BPSK(fs, tc, bitstr, true);

}
int main() {
    test();
    return 0;
}