#include <string>
#include <cstring>

#include "gtest/gtest.h"
#include "request_parser.hpp"
#include "request.hpp"

class RequestParserTest : public ::testing::Test {
protected:
	boost::tribool ParseRequest(std::string http_request) {
		char * begin = strdup(http_request.c_str());
		char * end = begin + http_request.length();
		boost::tribool result;
		boost::tie(result, boost::tuples::ignore) = request_parser_.parse(request_, begin, end);
		return result;
	}
	http::server::request_parser request_parser_;
	http::server::request request_;
};

TEST_F(RequestParserTest, GoodRequest) {
	std::string good_request = "GET / HTTP/1.1\r\nContent-Type: text/plain\r\n\r\n";
	EXPECT_EQ(ParseRequest(good_request), true);
}