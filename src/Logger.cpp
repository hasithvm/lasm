#include <iostream>
#include <fstream>
#include "Logger.hpp"

Logger::Logger() :
m_out(new NullStream()),
owns(true)
{
}

Logger& Logger::Instance()
{
    static Logger m_logger;
    return m_logger;
}

void Logger::SetOutput(output_t output_mode)
{
	free_stream();

	switch (output_mode)
	{
		case (OUTPUT_NONE):
		m_out = new NullStream();
		owns = true;
		break;

		case (OUTPUT_CLOG):
		m_out = &std::clog;
		owns = false;
		break;

		case (OUTPUT_FILE):
		m_out = new std::ofstream("debug.log", std::ofstream::out);
		owns = true;
		break;

		default:
		break;
	}

}

void Logger::free_stream()
{
	if (owns)
	{
		delete m_out;
	}
}

Logger::~Logger()
{ 
	Instance() << "Cleaning up";
	free_stream();

}

Logger& operator<<(Logger &l, std::ostream & (*manip)(std::ostream &)) {
	manip(*l.m_out);
	return l;
}


