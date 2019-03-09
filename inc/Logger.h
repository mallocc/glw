#pragma once

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>

#define MAX_LINE 300

#define __FORMATED_DATE__ (char const[]){ __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10], ' ', __DATE__[0], __DATE__[1], __DATE__[2], ' ', __DATE__[4], __DATE__[5], '\0' }

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
        std::stringstream ss;
        ss << __FORMATED_DATE__ << "   " << __TIME__;
        std::string dateTime = ss.str();
        std::replace(dateTime.begin(), dateTime.end(), ' ', '_');
        std::replace(dateTime.begin(), dateTime.end(), ':', '_');
        return dateTime;
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

      static void LTrace()
      {
        char output[MAX_LINE];
        sprintf(output,
            "[%s] :%s [TRACE  ] %s\n",
            LDATE_TIME().c_str(), "NLL",
            "");
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

#define LERROR(...) util::log::Logger::LError(__VA_ARGS__)
#define LWARNING(...) util::log::Logger::LWarning(__VA_ARGS__)
#define LDEBUG(...) util::log::Logger::LDebug__VA_ARGS__)
#define LINFO(...) util::log::Logger::LInfo(__VA_ARGS__)
#define LTRACE() util::log::Logger::LTrace()

#define LSTARTLOGGER(...) util::log::Logger::LOpenFile(__VA_ARGS__)
#define LENDLOGGER(...) util::log::Logger::LCloseFile()
