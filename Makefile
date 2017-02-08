# Building the server
CXXFLAGS=-std=c++0x -Wall -Werror
SRC_FILES=src/*.cpp nginx-configparser/config_parser.cc

# Running unit tests
GTEST_PATH=nginx-configparser/googletest/googletest
GMOCK_PATH=nginx-configparser/googletest/googlemock
GTEST_IMPORT=-I$(GTEST_PATH)/include -I./src $(GTEST_PATH)/src/gtest_main.cc
GMOCK_IMPORT=-I$(GMOCK_PATH)/include
TEST_FILES=test/*.cpp
BOOST_FLAGS=-lpthread -lboost_system -L$(BOOST_PATH)
RESULTS_TEST_DIR=results-unit-tests
RESULTS_COVERAGE_DIR=results-coverage

# Test file dependencies
REQUEST_HANDLER_ECHO_DEPENDENCIES=src/request_handler_echo.cpp src/reply.cpp src/request.hpp
SERVER_DEPENDENCIES=src/server.cpp src/connection.cpp src/connection_manager.cpp src/request_handler_echo.cpp src/request_handler_static.cpp src/reply.cpp src/request_parser.cpp src/mime_types.cpp
REQUEST_PARSER_DEPENDENCIES=src/request_parser.cpp
REPLY_DEPENDENCIES=src/reply.cpp src/request.hpp src/request_handler_echo.cpp src/request_handler_static.cpp src/mime_types.cpp
CONNECTION_DEPENDENCIES=src/connection.cpp src/connection_manager.cpp src/request_handler_static.cpp src/request_handler_echo.cpp src/reply.cpp src/mime_types.cpp src/request_parser.cpp

all: webserver 

webserver: $(SRC_FILES)
	g++ $(CXXFLAGS) -I$(BOOST_PATH) -I. $(SRC_FILES) -o \
	webserver $(BOOST_FLAGS)

test:
	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/request_handler_echo_test.cpp $(REQUEST_HANDLER_ECHO_DEPENDENCIES) \
	test/libgtest.a -o request_handler_echo_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/server_test.cpp $(SERVER_DEPENDENCIES) \
	test/libgtest.a -o server_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/request_parser_test.cpp $(REQUEST_PARSER_DEPENDENCIES) \
	test/libgtest.a -o request_parser_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/reply_test.cpp $(REPLY_DEPENDENCIES) \
	test/libgtest.a -o reply_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) $(GMOCK_IMPORT) \
	test/connection_test.cpp $(CONNECTION_DEPENDENCIES) \
	test/libgtest.a -o connection_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) $(GMOCK_IMPORT) \
	test/connection_manager_test.cpp $(CONNECTION_DEPENDENCIES) \
	test/libgtest.a -o connection_manager_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/request_handler_static_test.cpp \
	src/request_handler_static.cpp src/mime_types.cpp src/reply.cpp \
	test/libgtest.a -o \
	request_handler_static_test $(BOOST_FLAGS)

coverage: CXXFLAGS+=-coverage
coverage:
	mkdir $(RESULTS_TEST_DIR)
	mkdir $(RESULTS_COVERAGE_DIR)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/request_handler_echo_test.cpp $(REQUEST_HANDLER_ECHO_DEPENDENCIES) \
	test/libgtest.a -o request_handler_echo_test $(BOOST_FLAGS)
	./request_handler_echo_test > $(RESULTS_TEST_DIR)/request_handler_echo_test_info
	gcov -r request_handler_echo.cpp > $(RESULTS_COVERAGE_DIR)/request_handler_echo_coverage

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/server_test.cpp $(SERVER_DEPENDENCIES) \
	test/libgtest.a -o server_test $(BOOST_FLAGS)
	./server_test > $(RESULTS_TEST_DIR)/server_test_info
	gcov -r server.cpp > $(RESULTS_COVERAGE_DIR)/server_coverage

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/request_parser_test.cpp $(REQUEST_PARSER_DEPENDENCIES) \
	test/libgtest.a -o request_parser_test $(BOOST_FLAGS)
	./request_parser_test > $(RESULTS_TEST_DIR)/request_parser_test_info
	gcov -r request_parser.cpp > $(RESULTS_COVERAGE_DIR)/request_parser_coverage

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/reply_test.cpp $(REPLY_DEPENDENCIES) \
	test/libgtest.a -o reply_test $(BOOST_FLAGS)
	./reply_test > $(RESULTS_TEST_DIR)/reply_test_info
	gcov -r reply.cpp > $(RESULTS_COVERAGE_DIR)/reply_coverage

	g++ $(CXXFLAGS) $(GTEST_IMPORT) $(GMOCK_IMPORT) \
	test/connection_test.cpp $(CONNECTION_DEPENDENCIES) \
	test/libgtest.a -o connection_test $(BOOST_FLAGS)
	./connection_test > $(RESULTS_TEST_DIR)/connection_test_info
	gcov -r connection.cpp > $(RESULTS_COVERAGE_DIR)/connection_coverage

	g++ $(CXXFLAGS) $(GTEST_IMPORT) $(GMOCK_IMPORT) \
	test/connection_manager_test.cpp $(CONNECTION_DEPENDENCIES) \
	test/libgtest.a -o connection_manager_test $(BOOST_FLAGS)
	./connection_manager_test > $(RESULTS_TEST_DIR)/connection_manager_test_info
	gcov -r connection_manager.cpp > $(RESULTS_COVERAGE_DIR)/connection_manager_coverage

integration:
	./integration_test.sh

run-tests:
	./run_tests.sh

clean:
	rm -rf *.o webserver *_test $(RESULTS_TEST_DIR) $(RESULTS_COVERAGE_DIR) *.gcov *.gcda *.gcno

dist:
	tar -czvf webserver.tar.gz *

.PHONY: all test clean
