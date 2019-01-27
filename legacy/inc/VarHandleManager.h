#pragma once

#include "opengl.h"
#include "glm.h"
#include <stdio.h>
#include <vector>
#include "VarHandle.h"

namespace gfx
{
	namespace engine
	{
		class VarHandleManager
		{
		public:
			void add_handle(VarHandle var_handle);

			void load_handles();

			void link_handles(GLuint prog);

			VarHandle get_last_handle();

		private:
			std::vector<VarHandle> m_handles;
		};
	}
}