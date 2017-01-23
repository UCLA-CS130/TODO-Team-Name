all: webserver 

webserver: main.cpp server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp mime_types.cpp request_parser.cpp
	c++ -I $(BOOST_PATH) main.cpp server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp mime_types.cpp request_parser.cpp -o webserver -lpthread -lboost_system

clean:
	rm -rf *.o webserver 

dist:
	tar -czvf webserver.tar.gz *