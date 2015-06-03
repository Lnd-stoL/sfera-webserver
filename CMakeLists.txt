cmake_minimum_required(VERSION 3.1)
project(sfera_webserver)

#SET(Boost_USE_STATIC_LIBS ON)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.54 COMPONENTS log log_setup thread date_time filesystem system program_options REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS} ${INCLUDE_DIRECTORIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LIBANCILLARY_SRC libancillary.cpp)
set(SOURCE_FILES http_server_config.h main.cpp http_server.cpp http_server.h http_request.cpp http_request.h asio_headers.h http_response.cpp http_response.h get_request_processor.cpp get_request_processor.h request_processor.cpp request_processor.h file_info.cpp file_info.h head_request_processor.cpp head_request_processor.h post_request_processor.cpp post_request_processor.h http_standard_replies.cpp http_standard_replies.h client_connection.cpp client_connection.h worker.cpp worker.h master_listner.cpp master_listener.h)
add_executable(wwwd ${SOURCE_FILES} ${LIBANCILLARY_SRC} logger.hpp)
target_link_libraries(wwwd ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
