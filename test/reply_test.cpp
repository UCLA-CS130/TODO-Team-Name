// #include <string>
// #include <iostream>

// #include "gtest/gtest.h"
// #include "request_handler.hpp"
// #include "reply.hpp"

// TEST(StockReplyTest, BadRequest){
// 	http::server::reply reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
// 	std::string expectedHeader = "<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>";
// 	EXPECT_EQ(reply_.content, expectedHeader);
// }

// class ReplyTest : public ::testing::Test {
// protected:
// 	void HandleIt() {
// 		request_handler_.handle_request(buf, rep);
// 	}
// 	http::server::request_handler request_handler_;
// 	http::server::reply rep;
// 	char buf[8192];
// };

// TEST_F(ReplyTest, HeadersCheck) {
// 	std::string s = "Echo... echo... echo.";
// 	std::strcpy(buf, s.c_str());
// 	HandleIt();

// 	std::string expectedHeader0 = "Content-Length";
// 	EXPECT_EQ(rep.headers[0].name, expectedHeader0);
// 	std::string expectedHeader1 = "21";
//  	EXPECT_EQ(rep.headers[0].value, expectedHeader1);
//  	std::string expectedHeader2 = "Content-Type";
//   	EXPECT_EQ(rep.headers[1].name, expectedHeader2);
//   	std::string expectedHeader3 = "text/plain";
//   	EXPECT_EQ(rep.headers[1].value, expectedHeader3);
// }

// TEST_F(ReplyTest, EmptyRequest) {
// 	std::string s = "";
// 	std::strcpy(buf, s.c_str());
// 	HandleIt();

// 	EXPECT_EQ(s, rep.content);
// 	std::string expectedHeader1 = "0";
//  	EXPECT_EQ(rep.headers[0].value, expectedHeader1);

// }