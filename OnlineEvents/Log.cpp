#include "Log.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>

Log::Log(std::string filename, LogLevel logging_level) {
	log_time_last_ = GetTickCount64();
	file_object_ = std::ofstream(filename);
	logging_level_ = logging_level;
	if (file_object_.is_open())
		file_object_ << log_time_last_ << "	Starting Log:" << std::endl;
}

void Log::SetLoggingLevel(LogLevel logging_level) {
	logging_level_ = logging_level;
}

void Log::Write(std::string string, LogLevel level) {
	if (file_object_.is_open()) {
		//if (level <= logging_level_) {
		if (logging_level_ & level) {
			log_time_delta_ = GetTickCount64() - log_time_last_;
			log_time_last_ = GetTickCount64();
			file_object_ << log_time_delta_ << "	" << string << std::endl;
		}
	}
}

void Log::Close() {
	log_time_delta_ = GetTickCount64() - log_time_last_;
	if (file_object_.is_open())
		file_object_ << log_time_delta_ << "	Closing Log:" << std::endl;
	file_object_.close();
}