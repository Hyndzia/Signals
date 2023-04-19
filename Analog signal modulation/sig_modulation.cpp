#define _USE_MATH_DEFINES

#include <cmath>
#include "matplotlibcpp.h"
#include <complex>
#include <vector>

namespace plt = matplotlibcpp;


std::vector<std::complex<double>> fft(std::vector<double> x, int N) {
    std::vector<std::complex<double>> X(N);
    if (N == 1) {
        X[0] = x[0];
        return X;
    }
    else {
        std::vector<double> xn(N / 2);
        std::vector<double> xp(N / 2);
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

std::vector<double> sampled_signal(double fm, double fs, double tc, uint64_t samples) {
    std::vector<double> ts(samples);
    std::vector<double> X(samples);
    for (uint64_t i = 0; i < samples; ++i) {
        ts[i] = i * tc / (samples - 1);
        double value_mt = sin(2 * M_PI * fm * ts[i]); //simple tone
        X[i] = value_mt;
    }
    return X;
}

std::vector<double> ASK_modulation(double fm, double fn, double fs, double tc, uint64_t samples, std::vector<double> sig) {

    std::vector<double> ts(samples);
    std::vector<double> X(samples);
    for (uint64_t i = 0; i < samples; ++i) {
        ts[i] = i * tc / (samples - 1);
        double value_za = (0.7 * sig[i] + 1) * cos(2 * M_PI * fn * ts[i]);
        X[i] = value_za;
    }
    return X;
}

std::vector<double> PSK_modulation(double fm, double fn, double fs, double tc, uint64_t samples, std::vector<double> sig) {

    std::vector<double> ts(samples);
    std::vector<double> X(samples);
    for (uint64_t i = 0; i < samples; ++i) {
        ts[i] = i * tc / (samples - 1);
        double value_zp = cos(2 * M_PI * fn * ts[i] + (0.6) * sig[i]);
        X[i] = value_zp;
    }
    return X;
}
std::vector<double> FSK_modulation(double fm, double fn, double fs, double tc, uint64_t samples, std::vector<double> sig) {

    std::vector<double> ts(samples);
    std::vector<double> X(samples);
    for (uint64_t i = 0; i < samples; ++i) {
        ts[i] = i * tc / (samples - 1);
        double value_zf = cos(2 * M_PI * fn * ts[i] + ((0.6) / fm) * sig[i]);
        X[i] = value_zf;
    }
    return X;
}

std::vector<double> dB_scale(std::vector<std::complex<double>> X, uint64_t N) {
    std::vector<double> M((N / 2) - 1);
    M[0] = abs(X[0]);
    for (uint64_t k = 0; k < (N / 2) - 1; k++) {
        M[k] = sqrt(pow(real(X[k]), 2) + pow(imag(X[k]), 2));
        M[k] = 10 * log10(M[k]);
    }
    return M;
}

std::vector<double> freq_domain(double fs, uint64_t N) {
    std::vector<double> Fk((N / 2) - 1);
    for (uint64_t k = 0; k < (N / 2) - 1; k++) {
        Fk[k] = k * ((fs) / N);
    }
    return Fk;
}

double bandwidth(double range, std::vector<double> M, std::vector<double> Fk, uint64_t N) {
    double max_mag = M[0];
    for (uint64_t i = 1; i < M.size(); i++) {
        if (M[i] > max_mag) {
            max_mag = M[i];
        }
    }
    double xdB = max_mag - range;
    double min_f = 0;
    double max_f = 0;
    for (uint64_t i = 0; i < (N / 2) - 1; i++) {
        if (M[i] >= xdB) {
            if (min_f == 0) {
                min_f = Fk[i];
            }
            max_f = Fk[i];
        }
    }
    double BxdB = max_f - min_f;
    return BxdB;
}

int main() {
    double fm = 751.0;
    double fn = 7516.0;
    double fs = 44100.0;
    double tc = 2.0;
    uint64_t samples = tc * fs;
    auto sig = sampled_signal(fm, fs, tc, samples);
    auto ft = ASK_modulation(fm, fn, fs, tc, samples, sig);
    auto ft2 = PSK_modulation(fm, fn, fs, tc, samples, sig);
    auto ft3 = FSK_modulation(fm, fn, fs, tc, samples, sig);
    uint64_t N = 4096;
    std::vector<std::complex<double>> X = fft(ft, N);
    std::vector<std::complex<double>> X2 = fft(ft2, N);
    std::vector<std::complex<double>> X3 = fft(ft3, N);
    auto M = dB_scale(X, N);
    auto M2 = dB_scale(X2, N);
    auto M3 = dB_scale(X3, N);
    auto Fk = freq_domain(fs, N);
    double range = 12.0; //change for different bandwidth
    auto bxdB = bandwidth(range, M, Fk, N);
    auto bxdB2 = bandwidth(range, M2, Fk, N);
    auto bxdB3 = bandwidth(range, M3, Fk, N);
    std::cout << "ASK Bandwidth " << range << "dB :" << bxdB << "\n";
    std::cout << "PSK Bandwidth " << range << "dB :" << bxdB2 << "\n";
    std::cout << "FSK Bandwidth " << range << "dB :" << bxdB3 << "\n";
    //plt::semilogx(Fk, M);
    plt::plot(Fk, M);
    plt::plot(Fk, M2);
    plt::plot(Fk, M3);
    plt::grid(true);
    plt::xlabel("Czestotliwosc [Hz]");
    plt::ylabel("Magnituda [dB]");
    plt::show();
    return 0;
}