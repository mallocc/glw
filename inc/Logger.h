#pragma once

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>

#define MAX_LINE 300

namespace util
{
  namespace log
	{
    class Logger
    {
    public:
    
      // STATIC //
    
      static Logger& getInstance()
      {
          static Logger instance;
          return instance;
      }
      
      static void LOpenFile(const std::string& filename)
      {
        getInstance().openFile(filename.c_str());
      }
      
      static void LCloseFile()
      {
        getInstance().closeFile();
      }
      
      static void LAppendFile(const char * line)
      {
        if (getInstance().isOpen())
        {
          getInstance().appendFile(line);
        }
      }
      
      static std::string LDATE_TIME()
		  {
			  std::time_t t = std::time(0);
			  std::tm* now = std::localtime(&t);
			  std::stringstream ss;
			  ss                                      << (now->tm_year + 1900) << "_";
			  ss << std::setfill('0') << std::setw(2) << (now->tm_mon + 1)  << "_";
			  ss << std::setfill('0') << std::setw(2) << (now->tm_mday)     << "__";
			  ss << std::setfill('0') << std::setw(2) << (now->tm_hour)     << "_";
			  ss << std::setfill('0') << std::setw(2) << (now->tm_min)      << "_";
			  ss << std::setfill('0') << std::setw(2) << (now->tm_sec);
			  return ss.str();
		  }
		  
      static std::string LFILENAME(std::string str)
		  {
			  return str.substr(str.find_last_of("/\\") + 1);
		  }
		  
      static void LError(
		      const char * traceGroup, std::string str, 
		      const char * file, int line, 
		      const char * classname, const char * func)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [ERROR  ] (%s::%s) %s <%s:%i>\n", 
            LDATE_TIME().c_str(), traceGroup,
			      classname, func, 
			      str.c_str(),
            LFILENAME(file).c_str(), line);
        LAppendFile(output);
        printf("%s", output);
		  }
		  
      static void LWarning(
		      const char * traceGroup, std::string str, 
		      const char * file, int line, 
		      const char * classname, const char * func)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [WARNING] (%s::%s) %s <%s:%i>\n", 
            LDATE_TIME().c_str(), traceGroup,
			      classname, func, 
			      str.c_str(),
            LFILENAME(file).c_str(), line);
        LAppendFile(output);
        printf("%s", output);
		  }

      static void LDebug(
		      const char * traceGroup, std::string str, 
		      const char * classname, const char * func)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [DEBUG  ] (%s::%s) %s\n", 
            LDATE_TIME().c_str(), traceGroup,
			      classname, func, 
			      str.c_str());
        LAppendFile(output);
        printf("%s", output);
		  }
		  
      static void LInfo(const char * traceGroup, std::string str)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [INFO   ] %s\n", 
            LDATE_TIME().c_str(), traceGroup,
			      str.c_str());
        LAppendFile(output);
        printf("%s", output);
		  }

      // NON STATIC //

      void openFile(const std::string& filename)
      {
        char file[100];
        sprintf(file, "%s__%s.log", filename.c_str(), LDATE_TIME().c_str());
        m_file.open(file);
      }
      
      void closeFile()
      {
        m_file.close();
      }
      void appendFile(const char * line)
      {
        m_file << line;
      }
      
      bool isOpen()
      {
        return m_file.is_open();
      }

    private:    
      Logger() {}
            
      std::ofstream m_file;
    };
    
	}
}

// GLOBAL DEFINES //

#define LERROR(trg, str, file, line, classname, func) util::log::Logger::LError(trg, str,file,line,classname,func)
#define LWARNING(trg, str, file, line, classname, func) util::log::Logger::LWarning(trg, str,file,line,classname,func)
#define LDEBUG(trg, str, classname, func) util::log::Logger::LDebug(trg, str,classname,func)
#define LINFO(trg, str) util::log::Logger::LInfo(trg, str)

#define LSTARTLOGGER(file) util::log::Logger::LOpenFile(file)
#define LENDLOGGER(file) util::log::Logger::LCloseFile()
