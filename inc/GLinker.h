#pragma once

#include <vector>
#include <string>

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
    };
    // templated action class
    template<typename T>
    class GMetaAction : public IGMetaAction
    {
    public:
      void callback()
      {
        if (m_isStatic)
          m_func();
        else
          (m_object->*m_memberFunc)();
      }

      GMetaAction(GFuncPtr func)
      {
        m_func = func;
        m_isStatic = true;
      }
      GMetaAction(T * object, GMemberFunc<T> func)
      {
        m_object = object;
        m_memberFunc = func;
        m_isStatic = false;
      }
    protected:
      bool m_isStatic;
      GFuncPtr m_func;
      GMemberFunc<T> m_memberFunc;
      T * m_object;
    };
    // Typedef the interface
    typedef IGMetaAction* GAction;



    // Template class for a trigger
    template<typename T>
    struct GTriggerPtr
    {
      typedef void (T::*GTriggerFunc)();
    };
    // Typedef the templated trigger's function
    template<typename T>
    using GTriggerFunction = typename GTriggerPtr<T>::GTriggerFunc;



    // Templated pair of trigger and action
    template<typename T>
    using GLink = std::pair<GTriggerFunction<T>, GAction>;
    // Templated linker class
    template<typename T>
    class GLinker
    {
    public:
      void link(GTriggerFunction<T> trigger, GAction action)
      {
        m_links.push_back(std::make_pair(trigger, action));
      }

      GLinker() {}
    protected:
      void callTrigger(GTriggerFunction<T> trigger)
      {
        for (GLink<T>& link : m_links)
          if (trigger == link.first)
            link.second->callback();
      }
  private:
      std::vector<GLink<T>> m_links;
    };



    // Interface for action class
    class IGMetaTrigger
    {
    public:
      virtual void link(GAction action) = 0;
    };
    // templated action class
    template<typename T>
    class GMetaTrigger : public IGMetaTrigger
    {
    public:
      GMetaTrigger(GLinker<T> * linker, GTriggerFunction<T> trigger)
      {
        m_linker = linker;
        m_trigger = trigger;
      }

      void link(GAction action)
      {
        m_linker->link(m_trigger, action);
      }
    protected:
      GLinker<T> * m_linker;
      GTriggerFunction<T> m_trigger;
    };
    // Typedef the interface
    typedef IGMetaTrigger* GTrigger;



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

    // static function to create a trigger
    template<typename T>
    static GTrigger __trigger(GLinker<T>& linker, GTriggerFunction<T> trigger)
    {
      return new GMetaTrigger<T>(&linker, trigger);
    }

    // static function to link trigger and action
    static void __link(GTrigger trigger, GAction action)
    {
      trigger->link(action);
    }
    // static function to link linker's with trigger and action
    template<typename T>
    static void __link(GLinker<T>& linker, GTriggerFunction<T> trigger, GAction action)
    {
      linker.link(trigger, action);
    }

  }
}

#define TRIGGER(...) glw::meta::__trigger(__VA_ARGS__)
#define ACTION(...) glw::meta::__action(__VA_ARGS__)
#define LINK(...) glw::meta::__link(__VA_ARGS__)
