all: webserver client

webserver: webserver.cpp
	c++ -I ~/cs130/boost-server/boost_1_61_0 webserver.cpp -o webserver -lpthread -lboost_system \
		-L~cs130/boost-server/boost_1_61_0/~/cs130/boost-server/lib

client: client.cpp
	c++ -I ~/cs130/boost-server/boost_1_61_0 client.cpp -o client -lpthread -lboost_system \
		-L~cs130/boost-server/boost_1_61_0/~/cs130/boost-server/lib

clean:
	rm -rf *.o webserver client

dist:
	tar -czvf webserver.tar.gz *