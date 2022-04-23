#include <iostream>
#include <vector>
#include <random>
#include <numeric>

#include "gnuplot-iostream.h"

std::vector<double> cumsum(const std::vector<double>& vec)
{
	std::vector<double> output;
	output.resize(vec.size());

	std::partial_sum(vec.begin(), vec.end(), output.begin());
	return output;
}

std::vector<double> generate_normal(double mean, double variance, size_t size, uint32_t seed)
{
	std::mt19937 mt(seed);
	std::normal_distribution<double> normaldist(mean, variance);
	std::vector<double> vec;
	for (size_t i = 0u; i < size; ++i) {
		vec.push_back(normaldist(mt));
	}
	return vec;
}

std::vector<double> generate_brownian(double var, double time, double dt, uint32_t seed = 42)
{
	size_t n = time / dt;
	std::mt19937 mt(seed);
	std::normal_distribution<double> normaldist(0, var);
	std::vector<double> vec;
	// B_0 = 0
	vec.push_back(0);
	vec.resize(n + 1);
	for (size_t i = 1u; i < n + 1; ++i) {
		vec[i] = normaldist(mt);
	}
	return cumsum(vec);
}

std::vector<double> generate_x_axis(double time, double dt, bool inclusive = true)
{
	
	const size_t n = time / dt + (inclusive ? 1 : 0);
	std::vector<double> vec;
	vec.resize(n);
	for (int i = 0; i < n; ++i) {
		vec[i] = dt* i;
	}
	return vec;
}

int main()
{

	Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
	//Gnuplot gp(stdout);
	std::random_device rd;
	std::mt19937 mt(42);
	std::normal_distribution<double> normdist(0., 1.);
	 
	std::vector<double> v0, v1;
	v0 = generate_brownian(1, 1, 0.0001, 42);
	v1 = generate_brownian(1, 1, 0.0001, 43);
	const auto x_axis = generate_x_axis(1, 0.0001);
	
	
	std::tuple graph = { x_axis, v0};
	gp << "set title 'Graph of two random lines'\n ";
	//gp << "plot '-' with lines title 'v0' \n ";
	gp << "plot '-' using 1:2 with lines title 'v0' \n ";
		//<< "'-' with lines title 'v1' \n ";
	gp.send(graph);
	//gp.send1d(v1);

	std::cin.get(); 

	return 0;
}