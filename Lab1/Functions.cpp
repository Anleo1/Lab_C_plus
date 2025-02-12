#include "Functions.h"

double* input(double k[]) {
	bool flag = true;
	while (flag)
	{
		std::cout << "Введите коэффициенты(через enter):" << std::endl;
		if (std::cin >> k[0] && std::cin >> k[1] && std::cin >> k[2])
		{
			flag = false;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(1, '\n');
			std::cout << "Вы ввели неверные коэффициенты!" << std::endl;
		}
	}
	return k;
}
double discriminant(double k[]) {
	double dis = 0.0;
	dis = ((k[1] * k[1]) - (4 * k[0] * k[2]));
	if (dis < 0) {
		std::cout << "Нет корней\n";
		exit(0);
	}
	else {
		return sqrt(dis);
	}
}
double* solve(double k[], double res[]) {
	double dis = discriminant(k);
	if (dis == 0) {
		std::cout << "Два идентичных корня - вершина параболы\n";
	}
	res[0] = ((-k[1] + dis) / (2 * k[0]));
	res[1] = ((-k[1] - dis) / (2 * k[0]));

	return res;
}
void output() {
	setlocale(LC_ALL, "Rus");
	double coefficients[3];
	double res[2];
	input(coefficients);
	solve(coefficients, res);
	std::cout << "Корни:\nx1 = " << res[0] << "\nx2 = " << res[1] << std::endl;
	return;
}
