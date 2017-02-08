#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "request_handler_echo.hpp"
#include "reply.hpp"
#include "request.hpp"

class RequestHandlerEcho : public ::testing::Test {
protected:
	void HandleEchoRequest() {
		handler.handle_request(req, rep);
	}
	http::server::request_handler_echo handler;
	http::server::request req;
	http::server::reply rep;
};

TEST_F(RequestHandlerEcho, SimpleRequest) {
	std::string simple_request = "testing";
	req.full_header = simple_request;
	HandleEchoRequest();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
 	EXPECT_EQ(rep.headers[0].value, std::to_string(simple_request.length()));
  	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");
}

TEST_F(RequestHandlerEcho, EmptyRequest) {
	HandleEchoRequest();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
 	EXPECT_EQ(rep.headers[0].value, "0");
 	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");
}

TEST_F(RequestHandlerEcho, BigRequest) {
	std::string big_request = "";
	for (int i = 0; i < 50; i++) {
		big_request += "EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho";
	}
	req.full_header = big_request;
	HandleEchoRequest();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
 	EXPECT_EQ(rep.headers[0].value, std::to_string(big_request.length()));
  	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");
}
