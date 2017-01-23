all: webserver 

webserver: main.cpp server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp mime_types.cpp request_parser.cpp
	c++ -I ~/cs130/boost-server/boost_1_61_0 main.cpp server.cpp connection.cpp connection_manager.cpp request_handler.cpp reply.cpp mime_types.cpp request_parser.cpp -o webserver -lpthread -lboost_system \
		-L~cs130/boost-server/boost_1_61_0/~/cs130/boost-server/lib

clean:
	rm -rf *.o webserver 

dist:
	tar -czvf webserver.tar.gz *