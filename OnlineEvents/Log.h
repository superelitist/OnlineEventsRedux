#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <Windows.h>

class Log
{
public:
	Log(std::string filename);
	void Write(std::string string);
	void Close();
private:
	std::string line_to_log;
	std::ofstream file_object;
	ULONGLONG log_time_start;
	ULONGLONG log_time_delta;
};
#endif // LOG_H