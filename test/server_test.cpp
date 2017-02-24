#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "server.hpp"
#include "server_options.hpp"
#include <signal.h>

class ServerTest : public ::testing::Test {
protected:
	bool portValid(std::string port) {
        server_options_.port = port;
        http::server::Server server_("0.0.0.0", &server_options_);
        return server_.isValid();
    }
    http::server::server_options server_options_;
};

TEST_F(ServerTest, PortTooBig) {
    EXPECT_EQ(portValid("12312312"), false);
}

TEST_F(ServerTest, NegativePort) {
    EXPECT_EQ(portValid("-1"), false);
}

TEST_F(ServerTest, NoPortSpecified) {
    EXPECT_EQ(portValid(""), false);
}