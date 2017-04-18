#include "Logger.h"
#include <stdio.h>
#include <cstdarg>

bool Logger::LoggerEnabled;
LoggerLevel Logger::LoggingLevel;

Logger::Logger()
{
	LoggerEnabled = false;
	LoggingLevel = DebugPrint;
}

Logger::~Logger()
{
}

void Logger::Log(const char* message, LoggerLevel level = DebugPrint, ...)
{
	if (LoggingLevel == level)
	{
		va_list args;
		va_start(args, message);
		Log(message, args);
		va_end(args);
	}
}
