#pragma once

#include <string>
#include <iostream>

class Console {
public:
	static void Log(std::string message)
	{
		std::cout << message << std::endl;
	}

	static void Error(std::string err)
	{
		std::cerr << err << std::endl;
	}

	//static void Warn(std::string warn)
	//{
	//	std::ct
	//}
};