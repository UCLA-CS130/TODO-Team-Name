#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "response.hpp"

TEST(ResponseTest, OkResponse){
	std::string content = "Testing";
	http::server::Response response;
	response.SetStatus(http::server::Response::OK);
	response.AddHeader("Content-Length", "7");
	response.AddHeader("Content-Type", "text/plain");
	response.SetBody(content);

	std::string expected_response = "HTTP/1.0 200 OK\r\n"
									"Content-Length: 7\r\n"
									"Content-Type: text/plain\r\n\r\n" +
									content;
	EXPECT_EQ(response.ToString(), expected_response);
}

TEST(ResponseTest, BadRequestResponse){
	http::server::Response response;
	response.SetStatus(http::server::Response::BAD_REQUEST);
	std::string expected_response = "HTTP/1.0 400 Bad Request\r\n\r\n";
	EXPECT_EQ(response.ToString(), expected_response);
}

TEST(ResponseTest, NotFoundResponse){
	http::server::Response response;
	response.SetStatus(http::server::Response::NOT_FOUND);
	std::string expected_response = "HTTP/1.0 404 Not Found\r\n\r\n";
	EXPECT_EQ(response.ToString(), expected_response);
}

TEST(ResponseTest, InternalServerErrorResponse){
	http::server::Response response;
	response.SetStatus(http::server::Response::INTERNAL_SERVER_ERROR);
	std::string expected_response = "HTTP/1.0 500 Internal Server Error\r\n\r\n";
	EXPECT_EQ(response.ToString(), expected_response);
}