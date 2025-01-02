/**
 * File: messageProfiler
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "util/global/globalDefinitions.h"

#ifndef LRYA_MESSAGE_PROFILER_ENABLED
// enabled in all builds by default, disabled for shipping builds in deskmate_build_shipping.sh
#define LRYA_MESSAGE_PROFILER_ENABLED 1
#endif

#ifndef __Util_MessageProfiler_MessageProfiler_H__
#define __Util_MessageProfiler_MessageProfiler_H__

#include <string>
#include <time.h>

namespace Lrya {
namespace Util {

#if defined(LRYA_MESSAGE_PROFILER_ENABLED)

  // ================================================================================
  // MessageProfiler
  
  class MessageProfiler {
  public:
    MessageProfiler(const std::string& prefix);
    void Update(int msg, size_t size);
    void ReportOnFailure();
    void Report();

  private:
    static const constexpr int kMaxMessages = 256;

    std::string m_prefix;
    clock_t m_start;
    bool m_started;
    bool m_failed;

    int m_count[kMaxMessages];
    size_t m_size[kMaxMessages];
  };

#else // LRYA_MESSAGE_PROFILER_ENABLED

  class MessageProfiler {
  public:
    MessageProfiler(const std::string& prefix);
    void Update(int msg, size_t size);
    void ReportOnFailure();
    void Report();
  };

#endif // LRYA_MESSAGE_PROFILER_ENABLED

} // end namespace Util
} // end namespace Lrya

#endif // __Util_MessageProfiler_MessageProfiler_H__
