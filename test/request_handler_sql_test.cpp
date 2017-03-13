#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "request_handler_sql.hpp"
#include "request.hpp"
#include "response.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"


class RequestHandlerSqlTest : public ::testing::Test {
protected:
    http::server::RequestHandler::Status HandleQuery(std::string request_string) {
        std::string query_string = "GET /sql/query?q=" + request_string + " HTTP/1.1\r\n"
                                     "User-Agent: curl/7.35.0\r\n"
                                     "Host: localhost:8080\r\n"
                                     "Accept: */*\r\n\r\n";
        req = http::server::Request::Parse(request_string);
        return handler.HandleRequest(*req, &resp);
    }

    http::server::RequestHandler::Status HandleUpdate(std::string request_string) {
        std::string update_string = "GET /sql/update?q=" + request_string + " HTTP/1.1\r\n"
                                     "User-Agent: curl/7.35.0\r\n"
                                     "Host: localhost:8080\r\n"
                                     "Accept: */*\r\n\r\n";
        req = http::server::Request::Parse(request_string);
        return handler.HandleRequest(*req, &resp);
    }

    void recreate(){
    HandleUpdate("DROP TABLE Test");
    HandleUpdate("CREATE TABLE Test(name)");
    }

    http::server::SqlHandler handler;
    std::unique_ptr<http::server::Request> req;
    http::server::Response resp;
};

TEST_F(RequestHandlerSqlTest, QueryTest) {
    recreate();
    std::string queryString = "SELECT * FROM Test";
    EXPECT_EQ(http::server::RequestHandler::OK, HandleQuery(queryString));
}

TEST_F(RequestHandlerSqlTest, UpdateTest) {
    recreate();
    std::string updateString = "INSERT INTO Test(name) VALUES(test)";
    EXPECT_EQ(http::server::RequestHandler::OK, HandleUpdate(updateString));
}