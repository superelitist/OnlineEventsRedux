#include "Log.h"
#include <fstream>
#include <string>

Log::Log(std::string filename) {
	file_object = std::ofstream(filename);
	if (file_object.is_open())
		file_object << "Starting Log:" << std::endl;
}

void Log::Write(std::string string) {
	if (file_object.is_open())
		file_object << string << std::endl;
}

void Log::Close() {
	if (file_object.is_open())
		file_object << "Closing Log:" << std::endl;
	file_object.close();
}