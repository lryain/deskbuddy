#
# Declare compile definitions shared by all Lrya C++ projects
#

set(LRYA_BUILD_CXX_COMPILE_DEFINITIONS
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Debug>:_DEBUG>
  $<$<CONFIG:Release>:DEBUG>
  $<$<CONFIG:Release>:_DEBUG>
)

