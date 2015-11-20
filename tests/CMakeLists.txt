################################
# Unit tests
################################
add_library(unit_test_main main.cpp)

macro(add_unit_test test_name)
  set(target_name run_test_${test_name})
  add_executable(${target_name} ${test_name}.cpp)
  target_link_libraries(${target_name} gtest unit_test_main libzmq broked)
  add_test(NAME test_${test_name} COMMAND ${target_name})
endmacro(add_unit_test)

add_unit_test(version)
add_unit_test(task)
add_unit_test(result)
add_unit_test(serialization)
add_unit_test(message)
add_unit_test(async)
#add_unit_test(socket)
#add_unit_test(subscriber)
#add_unit_test(broked)
