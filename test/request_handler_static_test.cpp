#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "request_handler_static.hpp"
#include "reply.hpp"
#include "server_options.hpp"
#include "request.hpp"
#include "mime_types.hpp"

class RequestHandlerStatic : public ::testing::Test {
protected:
	void HandleStaticRequest() {
		http::server::request_handler_static handler(&server_options_);
		handler.handle_request(req, rep);
	}
	http::server::server_options server_options_; 
	http::server::request req;
	http::server::reply rep;
};

TEST_F(RequestHandlerStatic, SimpleRequest) {
	server_options_.static_files_map["/static1"] = "www";
	req.uri = "/static1/";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.content, "<!DOCTYPE html>\n"
		"<html>\n<body>\n\n<h1>This is default.html</h1>\n\n"
		"<p>The webserver successfully served this static file YAY</p>\n\n"
		"</body>\n</html>\n");
}

TEST_F(RequestHandlerStatic, EmptyRequest) {
	EXPECT_NO_THROW(HandleStaticRequest());
	EXPECT_EQ(rep.status, http::server::reply::bad_request);
}

TEST_F(RequestHandlerStatic, BadRequest) {
	req.uri = "thisWontWork.jpg";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::bad_request);
}

TEST_F(RequestHandlerStatic, URLNotFound) {
	req.uri = "/static1/thisWontWork.jpg";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::not_found);
}

TEST_F(RequestHandlerStatic, NoFileExtension) {
	req.uri = "a";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::bad_request);
}

TEST_F(RequestHandlerStatic, URLPoorlyFormed) {
	req.uri = "%";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::bad_request);

	req.uri = "%20";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::bad_request);

	req.uri = "+";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::bad_request);
}

TEST_F(RequestHandlerStatic, EscapeSpace) {
	server_options_.static_files_map["/static1"] = "www";
	req.uri = "/static1/test%20space.html";
	HandleStaticRequest();
	EXPECT_EQ(rep.status, http::server::reply::ok);
	EXPECT_EQ(rep.content, "<!DOCTYPE html><html><body>"
		"<h1>Testing space handling</h1></body></html>\n");
}