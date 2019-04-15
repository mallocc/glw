#include <stdio.h>
#include "Logger.h"
#include "StringFormat.h"

#include "GLinker.h"

using glw::meta::GLinker;
using util::StringFormat;


namespace
{
  const char * TRG = "MAIN";
  const char * __CLASSNAME__ = "main";
}

class Test : public GLinker<Test>
{
public:

  Test() {}

  void onTrigger()
  {
    m_level++;
    LINFO(TRG, StringFormat("onTrigger call #%0").arg(m_level).str());
    callTrigger(&Test::onTrigger);
  }

  void testFunction()
  {
    LINFO(TRG, "This is the real test.");
  }

  int m_level = 0;
};


void testFunction()
{
  LINFO(TRG, "This is a test.");
}

int main()
{
  LSTARTLOGGER("../logs/GLW");

  LINFO(TRG, "Program started.");

  Test test;
  LINK(TRIGGER(test, &Test::onTrigger), ACTION(testFunction));
  test.onTrigger();

  LINK(TRIGGER(test, &Test::onTrigger), ACTION(test, &Test::testFunction));
  test.onTrigger();

  // I dare you
//  LINK(TRIGGER(test, &Test::onTrigger), ACTION(test, &Test::onTrigger));
//  test.onTrigger();

  LINFO(TRG, "Program exit.");

  LENDLOGGER();

  return 0;
}
