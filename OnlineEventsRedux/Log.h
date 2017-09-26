#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <Windows.h>

enum LogLevel { LogNone			= 0, // No Log.Write() should ever use this. This effectively turns logging off.
				LogError		= 1, // Only things that should never have happened.
				LogNormal		= 2, // Some informational things, usually will only log once. Should get a couple lines a minute on this.
				LogVerbose		= 4, // Some informational things that repeat a lot.
				LogDebug		= 8, // Mostly to follow program flow, but spammy.
				LogLudicrous	= 16 // These will probably send a hundred times a second. This should almost always be off.
}; // I could do bitwise eval, but there's not much point...

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