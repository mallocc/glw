#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>


namespace alib
{
	namespace FLog
	{
		struct Log_T
		{
			std::string filename;
			std::vector<std::string> messageQueue;
			volatile bool terminated;
			volatile bool queueing;
			volatile bool free;
			volatile bool LOG_WATCHDOG_TIMER;
		};

		static std::string LFILENAME()
		{
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			std::stringstream ss;
			ss << "logs/DEBUGLOG__";
			ss << (now->tm_mday) << "_";
			ss << (now->tm_mon + 1) << "_";
			ss << (now->tm_year + 1900) << "__";
			ss << (now->tm_hour) << "_";
			ss << (now->tm_min) << "_";
			ss << (now->tm_sec);
			ss << ".log";
			return ss.str();
		}
		static std::string LDATE_TIME()
		{
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			std::stringstream ss;
			ss << (now->tm_hour) << ":";
			ss << (now->tm_min) << ":";
			ss << (now->tm_sec) << " ";
			ss << (now->tm_mday) << "-";
			ss << (now->tm_mon + 1) << "-";
			ss << (now->tm_year + 1900) << " > ";
			return ss.str();
		}
		
		static void LQUEUE_MSG(Log_T * log_t, std::string msg)
		{
			while (!log_t->free)
				if (log_t->LOG_WATCHDOG_TIMER++ > 1000)
				{
					printf("LOG ERROR: LOG_WATCHDOG_TIMER\n");
					return;
				}
			log_t->queueing = true;
			log_t->messageQueue.push_back(msg);
			log_t->queueing = false;
		}
		static void LQUEUE(Log_T * log_t, std::string msg)
		{
			std::thread(std::bind(LQUEUE_MSG, log_t, msg)).detach();
		}
		
		static void LINFO(Log_T * log_t, const char * functionName, const char * varName, const char * value, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[INFO ]: ";
			ss << functionName << " " << varName << " = " << value << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LINFO(Log_T * log_t, const char * functionName, const char * varName, float value, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[INFO ]: ";
			ss << functionName << " " << varName << " = " << value << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LINFO(Log_T * log_t, const char * functionName, const char * varName, int value, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[INFO ]: ";
			ss << functionName << " " << varName << " = " << value << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LINFO(Log_T * log_t, const char * functionName, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[INFO ]: ";
			ss << functionName << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LINFO(Log_T * log_t, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[INFO]: ";
			ss << msg;
			LQUEUE(log_t, ss.str());
		}

		static void LERROR(Log_T * log_t, const char * functionName, const char * varName, const char * value, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[ERROR]: ";
			ss << functionName << " " << varName << " = " << value << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LERROR(Log_T * log_t, const char * functionName, const char * varName, float value, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[ERROR]: ";
			ss << functionName << " " << varName << " = " << value << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LERROR(Log_T * log_t, const char * functionName, const char * varName, int value, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[ERROR]: ";
			ss << functionName << " " << varName << " = " << value << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LERROR(Log_T * log_t, const char * functionName, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[ERROR]: ";
			ss << functionName << " - " << msg;
			LQUEUE(log_t, ss.str());
		}
		static void LERROR(Log_T * log_t, const char * msg)
		{
			std::stringstream ss;
			ss << LDATE_TIME() << "[ERROR]: ";
			ss << msg;
			LQUEUE(log_t, ss.str());
		}

		static void LWRITER(Log_T * log_t)
		{
			std::ofstream file;
			while (!log_t->terminated)
			{
				if (!log_t->messageQueue.empty())
				{
					file.open(log_t->filename, std::ofstream::app);
					if (!log_t->queueing)
					{
						log_t->free = false;
						file << log_t->messageQueue.front() << std::endl;
						log_t->messageQueue.erase(log_t->messageQueue.begin());
						log_t->free = true;
					}
					file.close();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			std::stringstream ss;
			ss << LDATE_TIME() << "[INFO ]: ";
			ss << "LOG ENDED.";
			file.open(log_t->filename, std::ofstream::app);
			file << ss.str() << std::endl;
			file.close();
		}
		static void LTERMINATE(Log_T * log_t)
		{
			log_t->terminated = true;
		}
		static void LSTART(Log_T * log_t)
		{
			log_t->messageQueue = std::vector<std::string>();
			log_t->filename = LFILENAME();
			log_t->terminated = false;
			log_t->queueing = false;
			log_t->free = true;
			log_t->LOG_WATCHDOG_TIMER = 0;
			std::thread(std::bind(LWRITER, log_t)).detach();
			LINFO(log_t, "file logger has started.");
		}		
	}
}