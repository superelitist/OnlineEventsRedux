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
	const float f = 1.0 + 1.5 + 1.75;
	unsigned char b[sizeof(float)];
	memcpy(b, &f, sizeof(f));
	unsigned long result = ::hash(b);
	file.open(".\\OnlineEventsRedux.xyz", fstream::in | fstream::out | fstream::app);
	srand(GetTickCount64());
	string line;
	vector<string> vector;
	vector.push_back("120");
	vector.push_back("130");
	vector.push_back("140");
	vector.push_back("150");

	for (auto guard : vector) {
		//cout << "enter a number: ";
		//cin >> line;
		//guard = guard + line;
		//cout << "for (int guard : ints) " << to_string(guard) << endl;
		//cout << " you entered: " << line << endl;
	}
	//repeat(12, [] {cout << "l"; });
	//cout << "vector size: " << std::to_string(vector.size()) << endl;
	//cout << "GetFromUniformIntDistribution(): " << vector[GetFromUniformIntDistribution(0,vector.size()-1)];
	//cout << vector[GetFromUniformIntDistribution(0, static_cast<int>(vector.size() - 1))] << endl;


}