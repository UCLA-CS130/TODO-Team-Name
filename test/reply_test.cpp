#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "request_handler_echo.hpp"
#include "reply.hpp"
#include "request.hpp"

TEST(StockReplyTest, Ok){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::ok);
	std::string expected_content = "";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 200);
}

TEST(StockReplyTest, Created){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::created);
	std::string expected_content = "<html><head><title>Created</title></head><body><h1>201 Created</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 201);
}

TEST(StockReplyTest, Accepted){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::accepted);
	std::string expected_content = "<html><head><title>Accepted</title></head><body><h1>202 Accepted</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 202);
}

TEST(StockReplyTest, NoContent){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::no_content);
	std::string expected_content = "<html><head><title>No Content</title></head><body><h1>204 Content</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 204);
}

TEST(StockReplyTest, MultipleChoices){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::multiple_choices);
	std::string expected_content = "<html><head><title>Multiple Choices</title></head><body><h1>300 Multiple Choices</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 300);
}

TEST(StockReplyTest, MovedPermanently){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::moved_permanently);
	std::string expected_content = "<html><head><title>Moved Permanently</title></head><body><h1>301 Moved Permanently</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 301);
}

TEST(StockReplyTest, MovedTemporarily){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::moved_temporarily);
	std::string expected_content = "<html><head><title>Moved Temporarily</title></head><body><h1>302 Moved Temporarily</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 302);
}

TEST(StockReplyTest, NotModified){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::not_modified);
	std::string expected_content = "<html><head><title>Not Modified</title></head><body><h1>304 Not Modified</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 304);
}

TEST(StockReplyTest, BadRequest){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
	std::string expected_content = "<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 400);
}

TEST(StockReplyTest, Unauthorized){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::unauthorized);
	std::string expected_content = "<html><head><title>Unauthorized</title></head><body><h1>401 Unauthorized</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 401);
}

TEST(StockReplyTest, Forbidden){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::forbidden);
	std::string expected_content = "<html><head><title>Forbidden</title></head><body><h1>403 Forbidden</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 403);
}

TEST(StockReplyTest, NotFound){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::not_found);
	std::string expected_content = "<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 404);
}

TEST(StockReplyTest, InternalServerError){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::internal_server_error);
	std::string expected_content = "<html><head><title>Internal Server Error</title></head><body><h1>500 Internal Server Error</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 500);
}

TEST(StockReplyTest, NotImplemented){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::not_implemented);
	std::string expected_content = "<html><head><title>Not Implemented</title></head><body><h1>501 Not Implemented</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 501);
}

TEST(StockReplyTest, BadGateway){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::bad_gateway);
	std::string expected_content = "<html><head><title>Bad Gateway</title></head><body><h1>502 Bad Gateway</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 502);
}

TEST(StockReplyTest, ServiceUnavailable){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::service_unavailable);
	std::string expected_content = "<html><head><title>Service Unavailable</title></head><body><h1>503 Service Unavailable</h1></body></html>";
	EXPECT_EQ(reply_.content, expected_content);
	EXPECT_EQ(reply_.status, 503);
}