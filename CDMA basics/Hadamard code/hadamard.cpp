#define _USE_MATH_DEFINES

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>

int** walshhadamardCode(int n) {
    int** hadamard = new int* [n];
    for (int i = 0; i < n; i++) {
        hadamard[i] = new int[n];
    }
    hadamard[0][0] = 1;

    for (int size = 2; size <= n; size *= 2) {
        for (int i = 0; i < size / 2; ++i) {
            for (int j = 0; j < size / 2; ++j) {
                hadamard[i + size / 2][j] = hadamard[i][j];
                hadamard[i][j + size / 2] = hadamard[i][j];
                hadamard[i + size / 2][j + size / 2] = -hadamard[i][j];
            }
        }
    }

    return hadamard;
}

void deleteH(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void saveGold(const std::string& filename, const std::vector<uint32_t>& gold) {
    std::ofstream out(filename);
    if (out.is_open()) {
        for (size_t i = 0; i < gold.size(); i++) {
            if (i % 10 == 0 && i != 0) {
                out << '\n';
            }
            out << gold[i];
        }
        out.close();
    }
}
void printMatrix(int** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << matrix[i][j] << "";
        }
        std::cout << std::endl;
    }
}

void saveHadamard(const std::string& filename, int**& hadamard, int n) {
    std::ofstream out(filename);
    if (out.is_open()) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                out << hadamard[i][j] << " ";
            }
            out << std::endl;
        }
        out.close();
    }
}

void test() {
    int n = 128;
    int** hadamardMatrix = walshhadamardCode(n);

    if (hadamardMatrix != nullptr) {
        std::cout << "Hadamard Matrix for N = " << n << ":\n";
        printMatrix(hadamardMatrix, n);
        saveHadamard("hadamard.txt", hadamardMatrix, n);
        deleteH(hadamardMatrix, n);
    }
}

int main() {
    test();
    return 0;
}