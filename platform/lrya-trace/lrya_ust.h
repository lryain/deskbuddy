#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER lrya_ust

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "platform/lrya-trace/lrya_ust.h"

#if !defined(LRYA_UST_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define LRYA_UST_H

#if defined(USE_LRYATRACE)
#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
    lrya_ust,
    lrya_fault_code,
    TP_ARGS(int , code),
    TP_FIELDS(ctf_integer(int, c, code)
    )
)

TRACEPOINT_EVENT(
    lrya_ust,
    mate_anim_loop_duration,
    TP_ARGS(long long, duration),
    TP_FIELDS(ctf_integer(long long, duration, duration))
)

TRACEPOINT_EVENT(
    lrya_ust,
    mate_robot_loop_duration,
    TP_ARGS(long long, duration),
    TP_FIELDS(ctf_integer(long long, duration, duration))
)

TRACEPOINT_EVENT(
    lrya_ust,
    mate_robot_robot_loop_period,
    TP_ARGS(long long, delay),
    TP_FIELDS(ctf_integer(long long, delay, delay))
)

TRACEPOINT_EVENT(
    lrya_ust,
    mate_engine_loop_duration,
    TP_ARGS(long long, duration),
    TP_FIELDS(ctf_integer(long long, duration, duration))
)

#endif /* LRYATRACE */
#endif /* LRYA_UST_H */

#include <lttng/tracepoint-event.h>

