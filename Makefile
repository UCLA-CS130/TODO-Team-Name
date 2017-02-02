CXXFLAGS=-std=c++0x -Wall -Werror
SRC_FILES=src/*.cpp nginx-configparser/config_parser.cc
GTEST_PATH=nginx-configparser/googletest/googletest
TEST_FILES=test/*.cpp

all: webserver 

webserver: $(SRC_FILES)
	g++ $(CXXFLAGS) -I$(BOOST_PATH) -I. $(SRC_FILES) -o webserver -lpthread -lboost_system \
	-L$(BOOST_PATH)

test: $(SRC_FILES) $(TEST_FILES)
	g++ $(CXXFLAGS) -I$(GTEST_PATH)/include -I./src $(GTEST_PATH)/src/gtest_main.cc test/server_test.cpp \
	src/server.cpp src/connection.cpp src/connection_manager.cpp src/request_handler.cpp src/reply.cpp \
	test/libgtest.a -o server_test -lpthread -lboost_system -L$(BOOST_PATH)

	g++ $(CXXFLAGS) -I$(GTEST_PATH)/include -I./src $(GTEST_PATH)/src/gtest_main.cc test/request_handler_test.cpp \
	src/reply.cpp src/request_handler.cpp test/libgtest.a -o request_handler_test -lpthread -lboost_system -L$(BOOST_PATH)

	g++ $(CXXFLAGS) -I$(GTEST_PATH)/include -I./src $(GTEST_PATH)/src/gtest_main.cc test/reply_test.cpp \
	src/reply.cpp src/request_handler.cpp test/libgtest.a -o reply_test -lpthread -lboost_system -L$(BOOST_PATH)

integration:
	./integration_test

clean:
	rm -rf *.o webserver server_test

dist:
	tar -czvf webserver.tar.gz *

.PHONY: all test clean