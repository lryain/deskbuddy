#include "util/helpers/includeGTest.h" // Used in place of gTest/gTest.h directly to suppress warnings in the header
#include <iostream>
#include "util/logging/logging.h"
#include "util/logging/printfLoggerProvider.h"

Lrya::Util::PrintfLoggerProvider* loggerProvider = nullptr;

GTEST_API_ int main(int argc, char * argv[])
{
  ::testing::InitGoogleTest(&argc, argv);

  int rc = 0;
  {
    Lrya::Util::PrintfLoggerProvider printfLoggerProvider;
    printfLoggerProvider.SetMinLogLevel(Lrya::Util::LOG_LEVEL_DEBUG);
    Lrya::Util::gLoggerProvider = &printfLoggerProvider;
    rc = RUN_ALL_TESTS();
    Lrya::Util::gLoggerProvider = nullptr;
  }
  
  return rc;
}
