#ifndef TRACING_H
#define TRACING_H
#if defined(USE_LRYATRACE)
#include <lttng/tracelog.h>
#include "platform/lrya-trace/lrya_ust.h"
#define LRYATRACE_ENABLED 1
#else
#define tracepoint(...)
#define tracelog(e,m,...)
#define LRYATRACE_ENABLED 0
#endif
#endif
