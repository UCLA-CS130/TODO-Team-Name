CXXFLAGS=-std=c++0x -Wall -Werror
SRC_FILES=src/*.cpp nginx-configparser/config_parser.cc

all: webserver 

webserver: $(SRC_FILES)
	g++ $(CXXFLAGS) -I$(BOOST_PATH) -I. $(SRC_FILES) -o webserver -lpthread -lboost_system \
	-L$(BOOST_PATH)

clean:
	rm -rf *.o webserver 

dist:
	tar -czvf webserver.tar.gz *
