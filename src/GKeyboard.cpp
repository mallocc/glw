#include "GKeyboard.h"

#include <sstream>

using glw::engine::GKeyboard;


namespace
{
  const char * __CLASSNAME__ = "GKeyboard";
}


GKeyboard::GKeyboard()
{
}


GKeyboard::~GKeyboard()
{
}


void GKeyboard::queueChar(unsigned int codepoint)
{
  unsigned char c = (unsigned char)codepoint;
  std::stringstream ss;
  ss << c;
  m_typedInput.append(ss.str());
}

void GKeyboard::queueKey(unsigned int key)
{
  std::stringstream ss;
  switch (key)
  {
  case GLFW_KEY_ENTER:
    ss << '\n';
    m_typedInput.append(ss.str());
    break;
  case GLFW_KEY_TAB:
    ss << '\t';
    m_typedInput.append(ss.str());
    break;
  }
}

unsigned char GKeyboard::popTypedInputChar()
{
  unsigned char c = '\0';
  if(m_typedInput != "")
  {
    c = m_typedInput[0];
    m_typedInput.erase(m_typedInput.begin());
  }
  return c;
}

std::string GKeyboard::getTypedInput()
{
  std::string str = m_typedInput;
  m_typedInput = "";
  return str;
}

bool GKeyboard::isKeyDown(GLFWwindow* window, int key)
{
  return glfwGetKey(window, key) == GLFW_PRESS;
}
