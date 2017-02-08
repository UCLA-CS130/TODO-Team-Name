#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "request_handler_echo.hpp"
#include "reply.hpp"
#include "request.hpp"

TEST(StockReplyTest, Ok){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::ok);
	std::string expectedHeader = "";
	EXPECT_EQ(reply_.content, expectedHeader);
}

TEST(StockReplyTest, Created){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::created);
	std::string expectedHeader = "<html><head><title>Created</title></head><body><h1>201 Created</h1></body></html>";
	EXPECT_EQ(reply_.content, expectedHeader);
}

TEST(StockReplyTest, Accepted){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::accepted);
	std::string expectedHeader = "<html><head><title>Accepted</title></head><body><h1>202 Accepted</h1></body></html>";
	EXPECT_EQ(reply_.content, expectedHeader);
}

TEST(StockReplyTest, NoContent){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::no_content);
	std::string expectedHeader = "<html><head><title>No Content</title></head><body><h1>204 Content</h1></body></html>";
	EXPECT_EQ(reply_.content, expectedHeader);
}

TEST(StockReplyTest, BadRequest){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
	std::string expectedHeader = "<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>";
	EXPECT_EQ(reply_.content, expectedHeader);
}

// TODO: finish rest of stock replies