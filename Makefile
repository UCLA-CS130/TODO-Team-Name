CXXFLAGS=-std=c++0x -Wall -Werror
SRC_FILES=main.cpp nginx-configparser/config_parser.cc server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp


all: webserver 

webserver: $(SRC_FILES)
	g++ $(CXXFLAGS) -I $(BOOST_PATH) $(SRC_FILES) -o webserver -lpthread -lboost_system \
	-L$(BOOST_PATH)

clean:
	rm -rf *.o webserver 

dist:
	tar -czvf webserver.tar.gz *
