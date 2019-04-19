#pragma once

#include <vector>
#include <string>
#include <thread>
#include <map>
#include "Logger.h"


namespace glw
{
  namespace meta
  {
    // Normal function pointer
    typedef void(*GFuncPtr)();

    // Normal templated class
    template<typename T>
    struct GTemplateStruct
    {
      typedef void (T::*GFuncType)();
      typedef T GVarType;
    };
    // Typedef the templated class's function
    template<typename T>
    using GMemberFunc = typename GTemplateStruct<T>::GFuncType;
    // Typedef the templated class's member variable
    template<typename T>
    using GMemberVar = typename GTemplateStruct<T>::GVarType;



    // Interface for action class
    class IGMetaAction
    {
    public:
      virtual void callback() = 0;
      virtual void* getObject() = 0;
      virtual void* getFunc() = 0;
    };
    // templated action class
    template<typename T>
    class GMetaAction : public IGMetaAction
    {
    public:
      GMetaAction(GFuncPtr func)
      {
        m_func = func;
        m_isStatic = true;
      }
      GMetaAction(T* object, GMemberFunc<T> func)
      {
        m_object = object;
        m_memberFunc = func;
        m_isStatic = false;
      }

      void callback()
      {
        if (m_isStatic)
        {
          m_func();
        }
        else
        {
          if (NULL != m_object)
          {
            if (NULL != m_memberFunc)
            {
              (m_object->*m_memberFunc)();
            }
          }
        }
      }

      void* getObject()
      {
        return m_object;
      }

      void* getFunc()
      {
        return &m_memberFunc;
      }
    protected:
      bool m_isStatic;
      GFuncPtr m_func;
      GMemberFunc<T> m_memberFunc;
      T* m_object;
    };

    // Typedef the interface
    typedef IGMetaAction* GAction;

    // static function to create an action
    static GAction __action(GFuncPtr func)
    {
      return new GMetaAction<class GNULL>(func);
    }
    // static function to create an action
    template<typename T>
    static GAction __action(T& object, GMemberFunc<T> func)
    {
      return new GMetaAction<T>(&object, func);
    }
    template<typename T>
    static GAction __action(T * object, GMemberFunc<T> func)
    {
      return new GMetaAction<T>(object, func);
    }


    // Trigger id
    typedef int Trigger;
    // Templated linker class
    class GLinker
    {
    public:
      GLinker()
        : m_links() {}

      void link(Trigger trigger, GAction action)
      {
        m_links[trigger] = action;
      }

    protected:
      void callTrigger(Trigger trigger)
      {
        std::map<Trigger, GAction>::iterator link = m_links.find(trigger);
        if (link != m_links.end())
        {
          if (NULL != link->second)
          {
            link->second->callback();
          }
        }
      }
    private:
      std::map<Trigger, GAction> m_links;
    };



    // static function to link linker's with trigger and action
    static void __link(GLinker* linker, Trigger trigger, GAction action)
    {
      linker->link(trigger, action);
    }
    // static function to link linker's with trigger and action
    static void __link(GLinker& linker, Trigger trigger, GAction action)
    {
      linker.link(trigger, action);
    }

  }
}


#define PPCAT_NX(A, B)  A ## B
#define PPCAT(A, B)     PPCAT_NX(A, B)

#define DEFINE_TRIGGER_PREFIX   T_
#define DEFINE_TRIGGER(e)       PPCAT(DEFINE_TRIGGER_PREFIX, e)
#define DEFINE_TRIGGER_COUNT(c) DEFINE_TRIGGER(PPCAT(c, _COUNT))
#define DEFINE_TRIGGER_START(c) DEFINE_TRIGGER(PPCAT(c, _START))
#define DEFINE_TRIGGER_NONE     DEFINE_TRIGGER(NONE)

#define START_DEF_BASE_TRIGGERS(c) enum PPCAT(DEFINE_TRIGGER_PREFIX, c) {
#define END_DEF_TRIGGERS(c) , DEFINE_TRIGGER_COUNT(c) };
#define START_DEF_DERIVED_TRIGGERS(d,b) START_DEF_BASE_TRIGGERS(d) DEFINE_TRIGGER_START(d) = DEFINE_TRIGGER_COUNT(b),

#define TRIGGERS_BASE(c, ...) START_DEF_BASE_TRIGGERS(c) __VA_ARGS__ END_DEF_TRIGGERS(c) public
#define TRIGGERS_DERIVED(d, b, ...) START_DEF_DERIVED_TRIGGERS(d, b) __VA_ARGS__ END_DEF_TRIGGERS(d) public

#define trigger void

#define LINKER_CALL(t)        GLinker::callTrigger(DEFINE_TRIGGER(t))
#define LINKER_LINK(...)      GLinker::link(__VA_ARGS__)
#define LINKER_NEW_LINK(...)  glw::meta::__link(__VA_ARGS__)

#define ACTION(...) glw::meta::__action(__VA_ARGS__)
