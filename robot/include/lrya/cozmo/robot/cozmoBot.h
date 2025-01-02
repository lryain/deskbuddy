#ifndef LRYA_COZMOBOT_H
#define LRYA_COZMOBOT_H

#include "coretech/common/shared/types.h"

namespace Lrya {

  namespace Vector {

    namespace Robot {

      Result Init(const int * shutdownSignal);

      void Destroy();

      Result step_MainExecution();

    } // namespace Robot

  } // namespace Vector

} // namespace Lrya

#endif // LRYA_COZMOBOT_H
