#include "Logger.h"
#include <stdio.h>
#include <cstdarg>

bool Logger::LoggerEnabled = true;
LoggerLevel Logger::LoggingLevel = ProductionPrint;

Logger::Logger()
{
	LoggerEnabled = true;
	LoggingLevel = DebugPrint;
}

Logger::~Logger()
{
}

void Logger::Log(const char* message, LoggerLevel level, ...)
{
	if (LoggerEnabled == true) {
		if (LoggingLevel == level)
		{
			va_list args;
			va_start(args, level);
			vprintf(message, args);
			va_end(args);
		}
	}
}
