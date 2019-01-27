#pragma once

#include <stdio.h>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>

namespace alib
{
	namespace CLog
	{
		static std::string CDATE_TIME()
		{
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			std::stringstream ss;
			ss << (now->tm_hour) << ":";
			ss << (now->tm_min) << ":";
			ss << (now->tm_sec) << " ";
			ss << (now->tm_mday) << "-";
			ss << (now->tm_mon + 1) << "-";
			ss << (now->tm_year + 1900);
			return ss.str();
		}
		static std::string CFILENAME(std::string str)
		{
			return str.substr(str.find_last_of("/\\") + 1);
		}
		
		static void CError(std::string str, const char * file, int line, const char * classname, const char * func)
		{
			printf("%s :ERROR: <%s:%i> (%s::%s) %s\n", CDATE_TIME().c_str(), CFILENAME(file).c_str(), line, classname, func, str.c_str());
		}

		static void CDebug(std::string str, const char * classname, const char * func)
		{
			printf("%s :DEBUG: (%s::%s) %s\n", CDATE_TIME().c_str(), classname, func, str.c_str());
		}
		static void CInfo(std::string str)
		{
			printf("%s :INFO : %s\n", CDATE_TIME().c_str(), str.c_str());
		}
	}
}

#define CERROR(str, file, line, classname, func) alib::CLog::CError(str,file,line,classname,func)
#define CDEBUG(str, classname, func) alib::CLog::CDebug(str,classname,func)
#define CINFO(str) alib::CLog::CInfo(str)
