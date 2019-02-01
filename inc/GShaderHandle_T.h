#pragma once

#include "GShaderVariableHandle.h"

using glw::engine::glsl::GShaderVariableHandle;

namespace glw
{
  namespace engine
  {
    namespace glsl
    {
      struct GShaderHandle_T
      {
        GShaderVariableHandle * textureHandle;
        GShaderVariableHandle * colorHandle;
        GShaderVariableHandle * flagHandle;
        GShaderVariableHandle * modelMatHandle;
        GShaderVariableHandle * viewMatHandle;
        GShaderVariableHandle * projMatHandle;
      };
    }
  } 
}
