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
	void AddOptions(std::string path, std::string root) {
		server_options_.static_files_map[path] = root;
	}
	http::server::server_options server_options_; 
	http::server::request req;
	http::server::reply rep;
};

TEST_F(RequestHandlerStatic, EmptyConstructTest) {
	EXPECT_NO_THROW(http::server::request_handler_static handler(&server_options_));
}

TEST_F(RequestHandlerStatic, BadRequestTest) {
	http::server::request_handler_static handler(&server_options_);
	req.uri = "thisWontWork.jpg";
	handler.handle_request(req, rep);
	EXPECT_EQ(rep.content, "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>");
}

TEST_F(RequestHandlerStatic, URLNotFoundTest) {
	http::server::request_handler_static handler(&server_options_);
	req.uri = "/static1/thisWontWork.jpg";
	handler.handle_request(req, rep);
	EXPECT_EQ(rep.content, "<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>");
}

TEST_F(RequestHandlerStatic, IndexTest) {
	server_options_.static_files_map["/static1"] = "www";
	http::server::request_handler_static handler(&server_options_);
	req.uri = "/static1/";
	handler.handle_request(req, rep);
	EXPECT_EQ(rep.content, "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>This is default.html</h1>\n\n<p>The webserver successfully served this static file YAY</p>\n\n</body>\n</html>\n");
}

// TEST_F(RequestHandlerStatic, URLDecodeTest) {
// 	http::server::request_handler_static handler(&server_options_);
// 	std::string output;

// }

