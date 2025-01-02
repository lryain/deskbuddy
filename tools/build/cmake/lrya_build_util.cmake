# exclude target from being automatically built;
# can still be built by manually specifying it as a target
macro(lrya_disable_default_build target_name)
  set_target_properties(${target_name} PROPERTIES EXCLUDE_FROM_ALL TRUE)
endmacro()

# sets the input var to the output location of the given target;
# will use LRYA_OUT_LOCATION if property is set, otherwise $<TARGET_FILE>
macro(lrya_get_output_location outvar target_name)
  get_target_property(__tempvar ${target_name} LRYA_OUT_PATH)
  if (__tempvar MATCHES "NOTFOUND$")
    set(${outvar} $<TARGET_FILE:${target_name}>)
  else()
    set(${outvar} ${__tempvar})
  endif()
endmacro()
