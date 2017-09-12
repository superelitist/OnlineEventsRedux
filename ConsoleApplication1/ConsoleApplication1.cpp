// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
//using string_int = std::variant<std::string, int>;

template <typename F>
void repeat(unsigned n, F f) {
	while (n--) f();
}


int main()
{
	string line;
	vector<string> chars;
	chars.push_back("120");
	chars.push_back("130");
	chars.push_back("140");
	chars.push_back("150");

	for (auto guard : chars) {
		cout << "enter a number: ";
		cin >> line;
		//guard = guard + line;
		//cout << "for (int guard : ints) " << to_string(guard) << endl;
		cout << " you entered: " << line << endl;
	}
	repeat(12, [] {cout << "l"; });
}