#include "Log.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>

Log::Log(std::string filename) {
	log_time_last_ = GetTickCount64();
	file_object_ = std::ofstream(filename);
	if (file_object_.is_open())
		file_object_ << log_time_last_ << "	Starting Log:" << std::endl;
}

void Log::Write(std::string string, LogLevel level) {
	log_time_delta_ = GetTickCount64() - log_time_last_;
	log_time_last_ = GetTickCount64();
	if (file_object_.is_open())
		file_object_ << log_time_delta_ << "	" << string << std::endl;
	//std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void Log::Close() {
	log_time_delta_ = GetTickCount64() - log_time_last_;
	if (file_object_.is_open())
		file_object_ << log_time_delta_ << "	Closing Log:" << std::endl;
	file_object_.close();
}