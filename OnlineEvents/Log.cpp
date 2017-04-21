#include "Log.h"
#include <fstream>
#include <string>
#include <Windows.h>

Log::Log(std::string filename) {
	log_time_start = GetTickCount64();
	file_object = std::ofstream(filename);
	if (file_object.is_open())
		file_object << log_time_start << "	Starting Log:" << std::endl;
}

void Log::Write(std::string string) {
	log_time_delta = log_time_start - GetTickCount64();
	if (file_object.is_open())
		file_object << log_time_delta << "	" << string << std::endl;
}

void Log::Close() {
	log_time_delta = log_time_start - GetTickCount64();
	if (file_object.is_open())
		file_object << log_time_delta << "	Closing Log:" << std::endl;
	file_object.close();
}