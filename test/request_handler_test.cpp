#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "request_handler.hpp"
#include "reply.hpp"


class RequestHandlerTest : public ::testing::Test {
protected:
	void HandleIt() {
		request_handler_.handle_request(buf, rep);
	}
	http::server::request_handler request_handler_;
	http::server::reply rep;
	char buf[8192];
};

TEST_F(RequestHandlerTest, SimpleRequest) {
	std::string s = "Echo... echo... echo.";
	std::strcpy(buf, s.c_str());
	HandleIt();
	EXPECT_EQ(s, rep.content);
}

