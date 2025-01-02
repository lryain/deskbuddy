#include "coretech/common/shared/math/matrix.h"

#include "util/logging/logging.h"
#include "util/logging/printfLoggerProvider.h"

#include "gtest/gtest.h"

using namespace Lrya;

int main(int argc, char ** argv)
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
