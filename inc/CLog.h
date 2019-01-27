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
	namespace clog
	{
		class CLogger
    {
    public:
    
      // STATIC //
    
      static CLogger& getInstance()
      {
          static CLogger instance;
          return instance;
      }
      
      static void COpenFile(const std::string& filename)
      {
        getInstance().openFile(filename.c_str());
      }
      
      static void CCloseFile()
      {
        getInstance().closeFile();
      }
      
      static void CAppendFile(const char * line)
      {
        if (getInstance().isOpen())
        {
          getInstance().appendFile(line);
        }
      }
      
      static std::string CDATE_TIME()
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
		  
		  static std::string CFILENAME(std::string str)
		  {
			  return str.substr(str.find_last_of("/\\") + 1);
		  }
		  
		  static void CError(
		      const char * traceGroup, std::string str, 
		      const char * file, int line, 
		      const char * classname, const char * func)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [ERROR  ] (%s::%s) %s <%s:%i>\n", 
			      CDATE_TIME().c_str(), traceGroup,
			      classname, func, 
			      str.c_str(),
			      CFILENAME(file).c_str(), line);
        CAppendFile(output);
        printf("%s", output);
		  }
		  
		  static void CWarning(
		      const char * traceGroup, std::string str, 
		      const char * file, int line, 
		      const char * classname, const char * func)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [WARNING] (%s::%s) %s <%s:%i>\n", 
			      CDATE_TIME().c_str(), traceGroup,
			      classname, func, 
			      str.c_str(),
			      CFILENAME(file).c_str(), line);
        CAppendFile(output);     
        printf("%s", output);
		  }

		  static void CDebug(
		      const char * traceGroup, std::string str, 
		      const char * classname, const char * func)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [DEBUG  ] (%s::%s) %s\n", 
			      CDATE_TIME().c_str(), traceGroup, 
			      classname, func, 
			      str.c_str());
        CAppendFile(output);     
        printf("%s", output);
		  }
		  
		  static void CInfo(const char * traceGroup, std::string str)
		  {
		    char output[MAX_LINE];
			  sprintf(output, 
			      "[%s] :%s [INFO   ] %s\n", 
			      CDATE_TIME().c_str(), traceGroup, 
			      str.c_str());
        CAppendFile(output);     
        printf("%s", output);
		  }

      // NON STATIC //

      void openFile(const std::string& filename)
      {
        char file[100];
        sprintf(file, "%s__%s.log", filename.c_str(), CDATE_TIME().c_str());
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
      CLogger() {}
            
      std::ofstream m_file;
    };
    
	}
}

// GLOBAL DEFINES //

#define CERROR(trg, str, file, line, classname, func) util::clog::CLogger::CError(trg, str,file,line,classname,func)
#define CWARNING(trg, str, file, line, classname, func) util::clog::CLogger::CWarning(trg, str,file,line,classname,func)
#define CDEBUG(trg, str, classname, func) util::clog::CLogger::CDebug(trg, str,classname,func)
#define CINFO(trg, str) util::clog::CLogger::CInfo(trg, str)

#define CSTARTLOGGER(file) util::clog::CLogger::COpenFile(file)
#define CENDLOGGER(file) util::clog::CLogger::CCloseFile()
