#pragma once

#include <vector>

namespace gfx
{
	namespace gui
	{
		typedef void(*GFXFuncPtr)();

		template<typename T>
		struct GFXTemplateStruct
		{
			typedef void (T::*GFXFuncType)();
			typedef T GFXVarType;
		};
		template<typename T>
		using GFXMemberFunc = typename GFXTemplateStruct<T>::GFXFuncType;
		template<typename T>
		using GFXMemberVar = typename GFXTemplateStruct<T>::GFXVarType;

		class IGFXMetaAction
		{
		public:
			virtual void callback() = 0;
		};
		template<typename T>
		class GFXAction : public IGFXMetaAction
		{
		public:
			void callback()
			{
				if (m_isStatic)
					m_func();
				else
					(m_object->*m_memberFunc)();
			}

			GFXAction(GFXFuncPtr func)
			{
				m_func = func;
				m_isStatic = true;
			}
			GFXAction(T * object, GFXMemberFunc<T> func)
			{
				m_object = object;
				m_memberFunc = func;
				m_isStatic = false;
			}
		protected:
			bool m_isStatic;
			GFXFuncPtr m_func;
			GFXMemberFunc<T> m_memberFunc;
			T * m_object;
		};
		typedef IGFXMetaAction* GFXMetaAction;
		static GFXMetaAction ACTION(GFXFuncPtr func)
		{
			return new GFXAction<class GFXNULL>(func);
		}
		template<typename T>
		static GFXMetaAction ACTION(T * object, GFXMemberFunc<T> func)
		{
			return new GFXAction<T>(object, func);
		}

		template<typename T>
		struct GFXTriggerPtr
		{
			typedef void (T::*GFXTriggerFunc)(gfx::engine::GLContent *);
		};
		template<typename T>
		using GFXTrigger = typename GFXTriggerPtr<T>::GFXTriggerFunc;

		template<typename T>
		using GFXLink = std::pair<GFXTrigger<T>, GFXMetaAction>;
		template<typename T>
		class GFXLinker
		{
		public:
			void link(GFXTrigger<T> trigger, GFXMetaAction action)
			{
				m_links.push_back(std::make_pair(trigger, action));
			}

			GFXLinker() {}
		protected:
			void callTrigger(GFXTrigger<T> trigger)
			{
				for (GFXLink<T>& link : m_links)
					if (trigger == link.first)
						link.second->callback();
			}

			std::vector<GFXLink<T>> m_links;
		};
	}
}