#include "Functions.h"
#include "Objects.cpp"

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian");

    std::vector<std::unique_ptr<Student>> students;
    students.emplace_back(std::make_unique<GoodStudent>("Иван Иванов (отличник)"));
    students.emplace_back(std::make_unique<AverageStudent>("Петр Петров (середнячок)"));
    students.emplace_back(std::make_unique<AverageStudent>("Николай Николаев (середнячок)"));
    students.emplace_back(std::make_unique<BadStudent>("Сергей Сергеев (двоечник)"));

    auto equations = readEquationsFromFile("equations.txt");
    if (equations.empty()) {
        std::cout << "Используем уравнения по умолчанию...\n";
        equations = {
            {1, 2, 1},   
            {1, 0, 1},     
            {1, -4, 3},
        };
    }

    Teacher teacher;
    for (const auto& eq : equations) {
        for (const auto& student : students) {
            auto solution = student->solveEquation(eq);
            teacher.addSubmission(eq, solution);
        }
    }

    teacher.gradeAllSubmissions(students);
    teacher.publishResults();
    return 0;
}
