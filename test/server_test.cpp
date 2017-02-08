#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "server.hpp"
#include "server_options.hpp"
#include <signal.h>

// Server options for reference:
// struct server_options {
//   std::string port;
//   std::string echo_handler;
//   std::map<std::string, std::string> static_files_map;
// };

TEST(ServerTest, PortTooBig) {
	http::server::server_options server_options_;
	server_options_.port = "12312312";
	std::string addr = "0.0.0.0";

	http::server::server server_(addr, &server_options_);
	EXPECT_FALSE(server_.isValid(addr, server_options_.port));
}

TEST(ServerTest, PortIsNegative) {
	http::server::server_options server_options_;
	server_options_.port = "-1";
	std::string addr = "0.0.0.0";
	
	EXPECT_ANY_THROW(http::server::server server_(addr, &server_options_));	
}

TEST(ServerTest, NoPort) {
	http::server::server_options server_options_;
	server_options_.port = "";
	std::string addr = "0.0.0.0";

	http::server::server server_(addr, &server_options_);
	EXPECT_FALSE(server_.isValid(addr, server_options_.port));
}
