#pragma once
#include <chrono>
#include <iostream>

template<typename T>
void log_error(const T& msg) {
	std::cerr << std::chrono::utc_clock::now() << ": [ERROR] " << msg;
}
