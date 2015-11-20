add_library(task OBJECT task.cpp)
add_library(result OBJECT result.cpp)
add_library(transport OBJECT transport.cpp)
add_library(serialization OBJECT serialization.cpp)
add_library(message OBJECT message.cpp)

add_library(broked $<TARGET_OBJECTS:task>
                   $<TARGET_OBJECTS:result>
                   $<TARGET_OBJECTS:transport>
                   $<TARGET_OBJECTS:serialization>
                   $<TARGET_OBJECTS:message>)
