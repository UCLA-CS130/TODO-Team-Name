all: webserver 

webserver: main.cpp nginx-configparser/config_parser.cc server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp mime_types.cpp request_parser.cpp
	g++ -std=c++0x -I $(BOOST_PATH) main.cpp nginx-configparser/config_parser.cc server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp mime_types.cpp request_parser.cpp -o webserver -lpthread -lboost_system \
	-L$(BOOST_PATH)

clean:
	rm -rf *.o webserver 

dist:
	tar -czvf webserver.tar.gz *