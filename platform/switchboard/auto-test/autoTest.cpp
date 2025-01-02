/**
 * File: autoTest.cpp
 *
 * Author: paluri
 * Created: 3/22/2019
 *
 * Description: Header for describing simple class which 
 *              provides means of determing if test pin
 *              file exists, and whether robot is a dev
 *              disclaimer bot.
 *
 * Copyright: Lrya, Inc. 2019
 *
 **/

#include "util/fileUtils/fileUtils.h"
#include "auto-test/autoTest.h"

namespace Lrya {
namespace Switchboard {

const std::string AutoTest::kTestPinFilePath { "/factory/ble_pairing_pin" };
const std::string AutoTest::kProcCmdlinePath { "/proc/cmdline" };
const std::string AutoTest::kLryaDevString { "lrya.dev" };

bool AutoTest::DoesTestPinFileExist()
{
  return Lrya::Util::FileUtils::FileExists(kTestPinFilePath);
}

bool AutoTest::IsDisclaimerBot()
{
  return Lrya::Util::FileUtils::ReadFile(kProcCmdlinePath).find(kLryaDevString) != std::string::npos;
}

bool AutoTest::IsAutoTestBot()
{
  return AutoTest::DoesTestPinFileExist() && IsDisclaimerBot();
}

} // Switchboard
} // Lrya