/**
 * File: globalDefinitions.cpp
 *
 * Author: mwesley
 * Created: 06/10/2015
 * 
 * Description: Implementations for any global definitions
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/


#include "util/global/globalDefinitions.h"
#include "util/console/consoleInterface.h"


namespace Lrya {
namespace Util {
  
CONSOLE_VAR(bool, kForceDisableLryaDevFeatures, "Dev", false);

} // Lrya::Util namespace
} // Lrya namespace


bool NativeLryaUtilAreDevFeaturesEnabled()
{
  return ((LRYA_DEV_CHEATS != 0) && !Lrya::Util::kForceDisableLryaDevFeatures);
}
