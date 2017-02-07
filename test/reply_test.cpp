#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "request_handler_echo.hpp"
#include "reply.hpp"
#include "request.hpp"

TEST(StockReplyTest, BadRequest){
	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
	std::string expectedHeader = "<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>";
	EXPECT_EQ(reply_.content, expectedHeader);
}

class ReplyTest : public ::testing::Test {
protected:
	void HandleEchoRequest() {
		request_handler_echo_.handle_request(req, rep);
	}
	http::server::request_handler_echo request_handler_echo_;
	http::server::reply rep;
	http::server::request req;
};

TEST_F(ReplyTest, HeadersCheck) {
	req.full_header = "testing";
	HandleEchoRequest();
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
 	EXPECT_EQ(rep.headers[0].value, "7");
  	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");
}

TEST_F(ReplyTest, EmptyRequest) {
	HandleEchoRequest();
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
 	EXPECT_EQ(rep.headers[0].value, "0");
 	EXPECT_EQ(rep.headers[1].name, "Content-Type");
  	EXPECT_EQ(rep.headers[1].value, "text/plain");

}

// TODO: test reply from static requests