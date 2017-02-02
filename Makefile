# Building the server
CXXFLAGS=-std=c++0x -Wall -Werror
SRC_FILES=src/*.cpp nginx-configparser/config_parser.cc

# Running unit tests
GTEST_PATH=nginx-configparser/googletest/googletest
GMOCK_PATH=nginx-configparser/googletest/googlemock
GTEST_IMPORT=-I$(GTEST_PATH)/include -I./src $(GTEST_PATH)/src/gtest_main.cc
GMOCK_IMPORT=-I$(GMOCK_PATH)/include
TEST_FILES=test/*.cpp
COVERAGE_FLAGS=-fprofile-arcs -ftest-coverage
BOOST_FLAGS=-lpthread -lboost_system -L$(BOOST_PATH)
RESULTS_TEST_DIR=results-unit-tests
RESULTS_COVERAGE_DIR=results-coverage

all: webserver 

webserver: $(SRC_FILES)
	g++ $(CXXFLAGS) -I$(BOOST_PATH) -I. $(SRC_FILES) -o \
	webserver $(BOOST_FLAGS)

test: $(SRC_FILES) $(TEST_FILES)
	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/server_test.cpp \
	src/server.cpp src/connection.cpp src/connection_manager.cpp src/request_handler.cpp src/reply.cpp \
	test/libgtest.a -o \
	server_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/request_handler_test.cpp \
	src/reply.cpp src/request_handler.cpp \
	test/libgtest.a -o \
	request_handler_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) \
	test/reply_test.cpp \
	src/reply.cpp src/request_handler.cpp \
	test/libgtest.a -o \
	reply_test $(BOOST_FLAGS)
  
	g++ $(CXXFLAGS) $(GTEST_IMPORT) $(GMOCK_IMPORT) \
	test/connection_test.cpp \
	src/connection.cpp src/connection_manager.cpp src/request_handler.cpp src/reply.cpp \
	test/libgtest.a -o \
	connection_test $(BOOST_FLAGS)

	g++ $(CXXFLAGS) $(GTEST_IMPORT) $(GMOCK_IMPORT) \
	test/connection_manager_test.cpp \
	src/connection.cpp src/connection_manager.cpp src/request_handler.cpp src/reply.cpp \
	test/libgtest.a -o \
	connection_manager_test $(BOOST_FLAGS)

coverage: CXXFLAGS+=$(COVERAGE_FLAGS)
coverage: test
	mkdir $(RESULTS_TEST_DIR); \
	mkdir $(RESULTS_COVERAGE_DIR); \
	./reply_test > $(RESULTS_TEST_DIR)/reply_test_info; \
	./request_handler_test > $(RESULTS_TEST_DIR)/request_handler_test_info; \
	./server_test > $(RESULTS_TEST_DIR)/server_test_info; \
	./connection_test > $(RESULTS_TEST_DIR)/connection_test_info; \
	./connection_manager_test > $(RESULTS_TEST_DIR)/connection_manager_test_info; \
	gcov -r reply.cpp > $(RESULTS_COVERAGE_DIR)/reply_coverage; \
	gcov -r request_handler.cpp > $(RESULTS_COVERAGE_DIR)/request_handler_coverage; \
	gcov -r server.cpp > $(RESULTS_COVERAGE_DIR)/server_coverage; \
	gcov -r connection.cpp > $(RESULTS_COVERAGE_DIR)/connection_coverage; \
	gcov -r connection_manager.cpp > $(RESULTS_COVERAGE_DIR)/connection_manager_coverage;
	

integration:
	./integration_test.sh

all-tests:
	./run_tests.sh

clean:
	rm -rf *.o webserver *_test $(RESULTS_TEST_DIR) $(RESULTS_COVERAGE_DIR) *.gcov *.gcda *.gcno

dist:
	tar -czvf webserver.tar.gz *

.PHONY: all test clean