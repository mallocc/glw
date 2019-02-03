#pragma once

#include "opengl.h"
#include "glm.h"

namespace glw
{
  namespace engine
  {
    class GMouse
    {
    public:
      
      GMouse() {}

      GMouse(GLFWwindow * window,
             GLFWmousebuttonfun mouse_button_callback = NULL,
             GLFWcursorposfun cursor_pos_callback = NULL,
             GLFWscrollfun scroll_callback = NULL):
        m_window(window)
      {
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_pos_callback);
        glfwSetScrollCallback(window, scroll_callback);
      }
      
      ~GMouse()
      {
      }
      
      bool isLeftButtonDown()
      {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
      }

      bool isRightButtonDown()
      {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
      }

      bool isMiddleButtonDown()
      {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
      }

      glm::vec2 getMousePosition()
      {
        return m_mousePosition;
      }

      glm::vec2 getMouseDelta()
      {
        return m_mouseDelta;
      }

      glm::vec2 getScrollDelta()
      {
        return m_scrollDelta;
      }

      glm::vec2 popMouseDelta()
      {
        glm::vec2 delta = m_mouseDelta;
        m_mouseDelta = glm::vec2();
        return delta;
      }

      glm::vec2 popScrollDelta()
      {
        glm::vec2 delta = m_scrollDelta;
        m_scrollDelta = glm::vec2();
        return delta;
      }

      void updatePosition(double xpos, double ypos)
      {
        glm::vec2 pos(xpos, ypos);
        m_mouseDelta += pos - m_mousePosition;
        m_mousePosition = pos;
      }

      void updateScroll(double xoffset, double yoffset)
      {
        m_scrollDelta += glm::vec2(xoffset, yoffset);
      }

    private:
      GLFWwindow * m_window;

      glm::vec2 m_mousePosition;
      glm::vec2 m_mouseDelta;
      glm::vec2 m_scrollDelta;

    };
  }
}
