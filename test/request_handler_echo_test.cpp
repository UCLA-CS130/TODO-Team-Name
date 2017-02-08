#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "request_handler_echo.hpp"
#include "reply.hpp"
#include "request.hpp"
// struct request {
//   std::string method;
//   std::string uri;
//   int http_version_major;
//   int http_version_minor;
//   std::vector<header> headers;
//   std::string full_header;
//   enum request_type_options {
//     echo = 0,
//     static_file = 1
//   } request_type;
// };
class RequestHandlerEcho : public ::testing::Test {
protected:
	void HandleIt() {
		handler.handle_request(req, rep);
	}
	http::server::request_handler_echo handler;
	http::server::request req;
	http::server::reply rep;
};

TEST_F(RequestHandlerEcho, SimpleRequest) {
	std::string s = "Echo... echo... echo.";
	req.full_header = s;
	HandleIt();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.content, s);

	EXPECT_EQ(rep.headers.size(), 2);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
	EXPECT_EQ(rep.headers[0].value, std::to_string(s.length()));
	EXPECT_EQ(rep.headers[1].name, "Content-Type");
   	EXPECT_EQ(rep.headers[1].value, "text/plain");
}

TEST_F(RequestHandlerEcho, EmptyRequest) {
	std::string s = "";
	rep.content = s;
	HandleIt();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.content, s);

	EXPECT_EQ(rep.headers.size(), 2);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
	EXPECT_EQ(rep.headers[0].value, std::to_string(s.length()));
	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");
}

TEST_F(RequestHandlerEcho, BigRequest) {
	std::string s = "EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho"
	"EchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEchoEcho";
	
	rep.content = s;
	HandleIt();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_NE(rep.content, s);

	EXPECT_EQ(rep.headers.size(), 2);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
	EXPECT_NE(rep.headers[0].value, std::to_string(s.length()));
	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");
}
