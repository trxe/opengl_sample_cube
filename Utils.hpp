#pragma once
#include <chrono>
#include <iostream>

#define DEFAULT_RES_W 640 
#define DEFAULT_RES_H 640 

template<typename T>
void log_error(const T& msg) {
	std::cerr << std::chrono::utc_clock::now() << ": [ERROR] " << msg;
}
