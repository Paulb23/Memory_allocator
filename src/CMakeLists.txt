FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (project_LIBS m)
SET (project_BIN ${PROJECT_NAME})

add_library(memory_allocator SHARED ${project_SRCS})
add_library(memory_allocator_static STATIC ${project_SRCS})

TARGET_LINK_LIBRARIES(memory_allocator ${project_LIBS})
TARGET_LINK_LIBRARIES(memory_allocator_static ${project_LIBS})
