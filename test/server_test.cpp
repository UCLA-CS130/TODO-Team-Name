#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "server.hpp"
#include <signal.h>


TEST(ServerTest, PortTooBig) {
	std::string addr = "0.0.0.0";
	std::string port = "12312312";

	http::server::server server_(addr, port);
	EXPECT_FALSE(server_.isValid(addr, port));
}

TEST(ServerTest, PortIsNegative) {
	std::string addr = "0.0.0.0";
	std::string port = "-1";

	try{
		http::server::server server_(addr, port);	
	} catch (std::exception& e) {
    	std::cout << "Exception: " << "Negative Port. \n";
  	}
}

TEST(ServerTest, NoPort) {
	std::string addr = "0.0.0.0";
	std::string port = "";

	http::server::server server_(addr, port);
	EXPECT_FALSE(server_.isValid(addr, port));
}
