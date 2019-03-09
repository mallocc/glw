#pragma once

#include "opengl.h"

#include <string>
#include <sstream>

namespace glw
{
  namespace engine
  {
    class GKeyboard
    {
    public:
      
      GKeyboard() {}

      GKeyboard(GLFWwindow * window,
                         GLFWkeyfun key_callback = NULL,
                         GLFWcharfun character_callback = NULL):
        m_window(window)
      {
        glfwSetKeyCallback(window, key_callback);
        glfwSetCharCallback(window, character_callback);
      }
      
      ~GKeyboard()
      {
      }
      
      void updateChar(unsigned int codepoint)
      {
        unsigned char c = (unsigned char)codepoint;
        std::stringstream ss;
        ss << c;
        m_typedInput.append(ss.str());
      }

      void updateKey(unsigned int key)
      {
        std::stringstream ss;
        switch (key)
        {
        case GLFW_KEY_ENTER:
          ss << '\n';
          m_typedInput.append(ss.str());
          break;
        case GLFW_KEY_TAB:
          ss << ' ' << ' ';
          m_typedInput.append(ss.str());
          break;
        }
      }

      std::string getTypedInput()
      {
        return m_typedInput;
      }

      unsigned char popTypedInputChar()
      {
        unsigned char c = '\0';
        if(m_typedInput != "")
        {
          c = m_typedInput[0];
          m_typedInput.erase(m_typedInput.begin());
        }
        return c;
      }

      std::string popTypedInput()
      {
        std::string str = m_typedInput;
        m_typedInput = "";
        return str;
      }

      bool isKeyDown(int key)
      {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
      }


    private:
      GLFWwindow * m_window;

      std::string m_typedInput;

    };
  }
}
