#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>

class Log
{
public:
	Log(std::string filename);
	void Write(std::string string);
	void Close();
private:
	std::string line_to_log;
	std::ofstream file_object;
};
#endif // LOG_H