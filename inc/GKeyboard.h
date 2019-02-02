#pragma once

#include "opengl.h"

#include <string>


namespace glw
{
  namespace engine
  {
    class GKeyboard
    {
    public:
      
      GKeyboard();
      
      ~GKeyboard();
      
      void queueChar(unsigned int codepoint);
      void queueKey(unsigned int key);

      unsigned char popTypedInputChar();
      std::string getTypedInput();

      bool isKeyDown(GLFWwindow* window, int key);

    private:

      std::string m_typedInput;

    };
  }
}
