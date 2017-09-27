// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <random>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>

#pragma warning(disable : 4244 4305) // double <-> float conversions

using namespace std;
//using string_int = std::variant<std::string, int>;

template <typename F>
void repeat(unsigned n, F f) {
	while (n--) f();
}

std::ofstream file;

std::random_device random_device; 
std::mt19937 generator(::random_device()); // init a standard mersenne_twister_engine

int GetFromUniformIntDistribution(int first, int second) {
	std::uniform_int_distribution<> uniform_int_distribution(first, second);
	return uniform_int_distribution(generator);
}

inline double GetDistanceBetween2DCoords(float ax, float ay, float bx, float by) {
	double result = std::hypot(bx - ax, by - ay);
	//Logger.Write("GetDistanceBetween2DCoords( " + std::to_string(ax) + ", " + std::to_string(ay) + ", " + std::to_string(bx) + ", " + std::to_string(by) + " ): " + std::to_string(result), LogVerbose);
	return result;
}

inline double GetDistanceBetween3DCoords(float ax, float ay, float az, float bx, float by, float bz) {
	float result = std::sqrt( std::pow(ax - bx,2) + std::pow(ay - by,2) + std::pow(az - bz, 2));
	return result;
}

unsigned long hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

int main()
{
	string temp;
	double two_distance = GetDistanceBetween2DCoords(3, 0, 0, 0);
	double three_distance = GetDistanceBetween3DCoords(3, 0, 0, 0, 0, 0);

	cout << "two_distance: " << two_distance << endl;
	cout << "three_distance: " << three_distance << endl;

	system("pause");
}