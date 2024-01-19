#define _USE_MATH_DEFINES

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cmath>
#include <complex>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

std::vector<double> QAM_16(double fn, double fs, double tb, double tc, std::string bstream, bool isPlot) {
    std::vector<double> X;
    double A1 = 1.0, A2 = 0.5;
    std::vector<double> fi_vec = { (M_PI / 4), (M_PI / 2), (0.75 * M_PI), M_PI, (1.25 * M_PI), (1.5 * M_PI), (1.75 * M_PI) };
    std::map<std::string, std::pair<double, double>> mapQAM = {
        {"0000", {0.0, A1}},
        {"0001", {fi_vec[0], A1}},
        {"0011", {fi_vec[1], A1}},
        {"0010", {fi_vec[2], A1}},
        {"0110", {fi_vec[3], A1}},
        {"0111", {fi_vec[4], A2}},
        {"0101", {fi_vec[5], A2}},
        {"0100", {fi_vec[6], A2}},
        {"1100", {0.0, A1}},
        {"1101", {fi_vec[0], A1}},
        {"1111", {fi_vec[1], A1}},
        {"1110", {fi_vec[2], A1}},
        {"1010", {fi_vec[3], A2}},
        {"1011", {fi_vec[4], A2}},
        {"1001", {fi_vec[5], A2}},
        {"1000", {fi_vec[6], A2}},
    };
    char prev1 = 'x', prev2 = 'x', prev3 = 'x';
    std::string bitblock = "xxxx";
    for (const auto& bit : bstream) {
        if (prev3 == 'x') {
            prev3 = bit;
            continue;
        }
        if (prev2 == 'x') {
            prev2 = bit;
            continue;
        }
        if (prev1 == 'x') {
            prev1 = bit;
            continue;
        }
        if (prev3 != 'x' && prev2 != 'x' && prev1 != 'x') {
            bitblock[0] = prev3; bitblock[1] = prev2; bitblock[2] = prev1; bitblock[3] = bit;
            double fi = mapQAM[bitblock].first;
            double A = mapQAM[bitblock].second;
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(A * sin(2 * M_PI * fn * t + fi));
            }
            prev3 = 'x';
            prev2 = 'x';
            prev1 = 'x';
            continue;
        }
    }
    if (isPlot) {
        std::vector<double> t;
        for (int i = 0; i < X.size(); i++) {
            t.push_back(i / fs);
        }
        plt::plot(t, X);
    }
    return X;
}

std::vector<double> QAM_8(double fn, double fs, double tb, double tc, std::string bstream, bool isPlot) {
    std::vector<double> X;
    double A1 = 1.0, A2 = 0.5;
    std::vector<double> fi_vec = { (M_PI / 2),  M_PI, (3 * M_PI / 2) };
    std::map<std::string, std::pair<double, double>> mapQAM = {
        {"000", {0.0, A1}},
        {"001", {fi_vec[0], A1}},
        {"010", {fi_vec[1], A1}},
        {"011", {fi_vec[2], A1}},
        {"100", {0.0, A2}},
        {"101", {fi_vec[0], A2}},
        {"110", {fi_vec[1], A2}},
        {"111", {fi_vec[2], A2}},
    };
    char prev1 = 'x', prev2 = 'x';
    std::string bitblock = "xxx";
    for (const auto& bit : bstream) {
        if (prev2 == 'x') {
            prev2 = bit;
            continue;
        }
        if (prev1 == 'x') {
            prev1 = bit;
            continue;
        }
        if (prev2 != 'x' && prev1 != 'x') {
            bitblock[0] = prev2; bitblock[1] = prev1; bitblock[2] = bit;
            double fi = mapQAM[bitblock].first;
            double A = mapQAM[bitblock].second;
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(A * sin(2 * M_PI * fn * t + fi));
            }
            prev2 = 'x';
            prev1 = 'x';
            continue;
        }
    }
    if (isPlot) {
        std::vector<double> t;
        for (int i = 0; i < X.size(); i++) {
            t.push_back(i / fs);
        }
        plt::plot(t, X);
    }
    return X;
}

std::vector<double> ASK(double fn, double fs, double tb, double tc, std::string bstream, bool isPlot) {
    std::vector<double> X;
    double A1 = 1;
    double A2 = 0.5;
    double A = 0.0;
    for (const auto& bit : bstream) {
        if (bit == '0') {
            A = A2;
        }
        if (bit == '1') {
            A = A1;
        }
        for (double t = 0; t < tb; t += 1.0 / fs) {
            X.push_back(A * sin(2 * M_PI * fn * t));
        }
    }
    if (isPlot) {
        std::vector<double> t;
        for (int i = 0; i < X.size(); i++) {
            t.push_back(i / fs);;
        }
        plt::plot(t, X);
    }
    return X;
}

std::vector<double> PSK(double fs, double tc, std::string bstream, bool isPlot) {
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
        plt::plot(t, X);
    }

    return X;
}

std::vector<double> FSK(double W, double fs, double tb, double tc, std::string bstream, bool isPlot) {
    std::vector<double> X;
    double fn1 = (W + 1) / tb;
    double fn2 = (W + 2) / tb;
    for (const auto& bit : bstream) {
        if (bit == '0') {
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(sin(2 * M_PI * fn1 * t));
            }
        }
        if (bit == '1') {
            for (double t = 0; t < tb; t += 1.0 / fs) {
                X.push_back(sin(2 * M_PI * fn2 * t));
            }
        }
    }
    if (isPlot) {
        std::vector<double> t(X.size());
        for (int i = 0; i < X.size(); i++) {
            t[i] = i / fs;
        }
        plt::plot(t, X);
    }
    return X;
}

std::vector<std::complex<double>> fft(std::vector<std::complex<double>>& x, uint16_t N) {
    std::vector<std::complex<double>> X(N);
    if (N == 1) {
        X[0] = x[0];
        return X;
    }
    else {
        std::vector<std::complex<double>> xn(N / 2);
        std::vector<std::complex<double>> xp(N / 2);
        for (int i = 0; i < N / 2; i++) {
            xp[i] = x[2 * i];
            xn[i] = x[2 * i + 1];
        }
        std::vector<std::complex<double>> Xp = fft(xp, N / 2);
        std::vector<std::complex<double>> Xn = fft(xn, N / 2);

        for (int k = 0; k < N / 2; k++) {
            X[k] = Xp[k] + std::exp(std::complex<double>(0, -2 * M_PI * k / N)) * Xn[k];
            X[k + N / 2] = Xp[k] - std::exp(std::complex<double>(0, -2 * M_PI * k / N)) * Xn[k];
        }
    }
    return X;
}

std::vector<double> db_scale(std::vector<std::complex<double>> X, uint16_t N) {
    std::vector<double> M;
    std::vector<double> db_M;
    uint64_t k = 0;
    for (const auto& y : X) {
        if (k == ((N / 2) - 1)) break;
        M.push_back(abs(y));
        ++k;
    }
    M.front() = abs(X.front());
    for (const auto& x : M) {
        db_M.push_back(20 * log10(x));
    }
    return db_M;
}

std::vector<double> freq_domain(double fs, uint16_t N) {
    std::vector<double> Fk;
    for (uint16_t k = 0; k < (N / 2) - 1; k++) {
        Fk.push_back(((k * fs) / N));
    }
    return Fk;
}

void freq_spectrum(std::vector<double> ft, double fs, bool isPlot) {
    std::vector<std::complex<double>> x1;
    for (double val : ft) {
        x1.push_back(std::complex<double>(val, 0));
    }
    uint16_t N2 = pow(2, 15);
    std::vector<std::complex<double>> X1 = fft(x1, N2);
    auto M = db_scale(X1, N2);
    auto Fk = freq_domain(fs, N2);
    if (isPlot) plt::plot(Fk, M);
}




void test() {
    std::string bitstr = "011100111000101100001001000011101000100011110011011100111000101100001001000011101000100011110011";
    std::cout << bitstr;

    double tc = 15.0; double B = bitstr.size(); double W = 2.0; double tb = tc / B; double fn = W / tb; double fs = 20000; //zmienne sygnalu

    //auto ft = QAM_8(fn, fs, tb, tc, bitstr, true);
    auto ft1 = QAM_16(fn, fs, tb, tc, bitstr, true);
    //auto ft2 = ASK(fn, fs, tb, tc, bitstr, true);
    //auto ft3 = PSK(fs,tc, bitstr, true);
    //auto ft4 = FSK(fn, fs, tb, tc, bitstr, true);

    freq_spectrum(ft1, fs, false);

    //plt::grid(true);
    //plt::xlabel("Czestotliwosc [Hz]");
    //plt::ylabel("Magnituda [dB]");
    plt::show();
}
int main() {
    test();
    return 0;
}