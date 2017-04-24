#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <Windows.h>

enum LogLevel { LogNone, LogError, LogNormal, LogVerbose, LogVeryVerbose };

class Log {
public:
	Log(std::string filename, LogLevel logging_level);
	void SetLoggingLevel(LogLevel logging_level);
	void Write(std::string string, LogLevel level);
	void Close();
private:
	std::ofstream file_object_;
	std::string line_to_log;
	LogLevel logging_level_;
	ULONGLONG log_time_last_;
	ULONGLONG log_time_delta_;
};
#endif // LOG_H