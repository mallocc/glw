#include <stdio.h>

#include "StringFormat.h"
#include "CLog.h"
#include "glm.h"
#include "TestClass.h"
#include "TestClass1.h"

using alib::StringFormat;
using alib::atest::TestClass;
using alib::atest::TestClass1;

int main()
{
  CINFO("==================");
  glm::vec3 vec(1.0f,2.0f,3.142f);
  CINFO(StringFormat("%1 [%2 %3]")
    .arg("This is a test of StringFormat and GLM")
    .arg(vec.z)
    .arg(99U).str()
    );
  CDEBUG("Test of CDEBUG", "CLASS", __func__);
  CERROR("Test of CERROR", __FILE__, __LINE__, "CLASS", __func__);
  
  TestClass testClass;
  
  TestClass1 testClass1;
  
  return 0;
}
