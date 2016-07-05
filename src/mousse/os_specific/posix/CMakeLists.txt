cmake_minimum_required(VERSION 3.5)
project(Test CXX)

include_directories(".")

set(OS_SPECIFIC_SOURCES
	clock_time.cpp
	cpu_time.cpp
	dummy_print_stack.cpp
	file_monitor.cpp
	file_stat.cpp
	mem_info.cpp
	mem_info_io.cpp
	posix.cpp
	print_stack.cpp
	reg_exp.cpp
	sig_fpe.cpp
	sig_int.cpp
	sig_quit.cpp
	sig_segv.cpp
	sig_stop_at_write_now.cpp
	sig_write_now.cpp
	timer.cpp)

set(OS_SPECIFIC_HEADERS
	clock_time.hpp
	cpu_time.hpp
	file_monitor.hpp
	file_stat.hpp
	mem_info.hpp
	posix.hpp
	reg_exp.hpp
	sig_fpe.hpp
	sig_int.hpp
	sig_quit.hpp
	sig_segv.hpp
	sig_stop_at_write_now.hpp
	sig_write_now.hpp
	timer.hpp)

add_library(libmousse_os_specific SHARED ${OS_SPECIFIC_SOURCES})

find_file(INOTIFY inotify.h)

if(NOT (${INOTIFY} STREQUAL "INOTIFY-NOTFOUND"))
	message(STATUS "Found inotify.h -> " ${INOTIFY})
	add_definitions(-DMOUSSE_USE_INOTIFY)
endif(NOT (${INOTIFY} STREQUAL "INOTIFY-NOTFOUND"))
