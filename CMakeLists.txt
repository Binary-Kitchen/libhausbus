project(libhausbus)

add_definitions(-std=c++1y)

set(LIBHAUSBUS_HEADERS
	crc8.h
	data.h
	hausbus.h
	moodlights.h)

set(LIBHAUSBUS_SRCS
	crc8.cpp
	hausbus.cpp
	moodlights.cpp)

add_library(hausbus STATIC ${LIBHAUSBUS_HEADERS} ${LIBHAUSBUS_SRCS})
