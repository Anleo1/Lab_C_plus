#include "Functions.h"

std::vector<QuadraticEquation> readEquationsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<QuadraticEquation> equations;

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << "\n";
        return equations;
    }

    double a, b, c;
    while (file >> a >> b >> c) {
        equations.emplace_back(a, b, c);
    }

    if (equations.empty()) {
        std::cerr << "Предупреждение: файл не содержит уравнений\n";
    }

    file.close();
    return equations;
}

double discriminant(double k[]) {
    return (k[1] * k[1]) - (4 * k[0] * k[2]);
}

double* solve(double k[], double res[]) {
    double dis = discriminant(k);

    if (dis < 0) {
        res[0] = res[1] = 0.0;
    }
    else if (dis == 0) {
        res[0] = res[1] = (-k[1]) / (2 * k[0]);
    }
    else {
        double sqrt_dis = sqrt(dis);
        res[0] = (-k[1] + sqrt_dis) / (2 * k[0]);
        res[1] = (-k[1] - sqrt_dis) / (2 * k[0]);
    }
    return res;
}