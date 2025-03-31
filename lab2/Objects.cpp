#include "Functions.h"
#include <map>
#include <ctime>
#include <iomanip>

class Student {
protected:
    std::string name;

public:
    Student(const std::string& name) : name(name) {}
    virtual ~Student() = default;
    virtual EquationSolution solveEquation(const QuadraticEquation& eq) = 0;
    std::string getName() const { return name; }
};

class GoodStudent : public Student {
public:
    GoodStudent(const std::string& name) : Student(name) {}

    EquationSolution solveEquation(const QuadraticEquation& eq) override {
        double k[3];
        eq.getCoefficients(k);
        double res[2];
        solve(k, res);
        double dis = discriminant(k);

        if (dis < 0) {
            return EquationSolution(0, 0, 0, name);
        }
        else if (dis == 0) {
            return EquationSolution(1, res[0], 0, name);
        }
        else {
            return EquationSolution(2, res[0], res[1], name);
        }
    }
};

class AverageStudent : public Student {
public:
    AverageStudent(const std::string& name) : Student(name) {}

    EquationSolution solveEquation(const QuadraticEquation& eq) override {
        if ((rand() % 100) < 60) { // 60%
            double k[3];
            eq.getCoefficients(k);
            double res[2];
            solve(k, res);
            double dis = discriminant(k);

            if (dis < 0) {
                return EquationSolution(0, 0, 0, name);
            }
            else if (dis == 0) {
                return EquationSolution(1, res[0], 0, name);
            }
            else {
                return EquationSolution(2, res[0], res[1], name);
            }
        }
        else {
            int num_roots = rand() % 3;
            double root1 = (rand() % 100)/10.0;
            double root2 = 0;
            if (num_roots == 2) {
                root2 = (rand() % 100)/10.0;
            }
            else {
                root2 = 0;
            }
            return EquationSolution(num_roots, root1, root2, name);
        }
    }
};

class BadStudent : public Student {
public:
    BadStudent(const std::string& name) : Student(name) {}

    EquationSolution solveEquation(const QuadraticEquation& eq) override {
        return EquationSolution(1, 0, 0, name);
    }
};

class Teacher {
private:
    std::vector<std::pair<QuadraticEquation, EquationSolution>> submissions;
    std::map<std::string, int> results;

public:
    bool checkSolution(const QuadraticEquation& eq, const EquationSolution& sol) {
        double k[3];
        eq.getCoefficients(k);
        double correct_res[2];
        solve(k, correct_res);
        double dis = discriminant(k);

        if (dis < 0) {
            return sol.num_roots == 0;
        }
        else if (dis == 0) {
            if (sol.num_roots != 1) return false;
            return std::abs(correct_res[0] - sol.root1) < 1e-6;
        }
        else {
            if (sol.num_roots != 2) return false;

            bool match1 = (std::abs(correct_res[0] - sol.root1) < 1e-6 &&
                std::abs(correct_res[1] - sol.root2) < 1e-6);
            bool match2 = (std::abs(correct_res[0] - sol.root2) < 1e-6 &&
                std::abs(correct_res[1] - sol.root1) < 1e-6);

            return match1 || match2;
        }
    }

    void addSubmission(const QuadraticEquation& eq, const EquationSolution& sol) {
        submissions.emplace_back(eq, sol);
    }

    void gradeAllSubmissions(const std::vector<std::unique_ptr<Student>>& students) {
        
        for (const auto& student : students) {
            results[student->getName()] = 0;
        }
        for (const auto& submission : submissions) {
            const auto& eq = submission.first;
            const auto& sol = submission.second;

            if (checkSolution(eq, sol)) {
                results[sol.student_name]++;
            }
        }
    }

    void publishResults() const {
        setlocale(LC_ALL, "Russian");
        std::cout << "=== Результаты зачета ===\n";
        std::cout << std::setw(25) << std::left << "Студент" << "Решено задач\n";
        std::cout << "---------------------------\n";

        for (const auto& entry : results) {
            std::cout << std::setw(25) << std::left << entry.first
                << entry.second << std::endl;
        }
    }
};