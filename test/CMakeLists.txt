enable_testing()
INCLUDE(CTest)

LINK_DIRECTORIES(${MAINFOLDER}/lib)
SET (test_LIBS ${PROJECT_NAME} m)

SET( test_SRCS
	main_test
)

foreach(next_ITEM ${test_SRCS})
   ADD_EXECUTABLE(${next_ITEM} ${next_ITEM}.c)
   TARGET_LINK_LIBRARIES(${next_ITEM} ${test_LIBS})
   add_test(NAME ${next_ITEM} COMMAND ${next_ITEM})
endforeach(next_ITEM ${test_SRCS})
