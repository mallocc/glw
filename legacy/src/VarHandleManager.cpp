#include "VarHandleManager.h"

using gfx::engine::VarHandleManager;

void VarHandleManager::add_handle(gfx::engine::VarHandle var_handle)
{
	m_handles.push_back(var_handle);
}

void VarHandleManager::load_handles()
{
	for (VarHandle v : m_handles)
		v.load();
}

void VarHandleManager::link_handles(GLuint prog)
{
	for (VarHandle v : m_handles)
		v.init(prog);
}

gfx::engine::VarHandle VarHandleManager::get_last_handle()
{
	return m_handles[m_handles.size() - 1];
}