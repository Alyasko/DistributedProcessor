#pragma once
#include "LoggerLevel.h"

class Logger
{
public:
	Logger();
	~Logger();

	static void Log(const char* message, LoggerLevel level = DebugPrint, ...);

	static bool LoggerEnabled;
	static LoggerLevel LoggingLevel;
private:

};

